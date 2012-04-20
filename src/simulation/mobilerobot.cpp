/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  the MRCore group
 *
 * MRcore is licenced under the Common Creative License,
 * Attribution-NonCommercial-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/

#include "mobilerobot.h"
#include "base/datalog.h"

namespace mr
{

MobileRobot::MobileRobot(string n)
{
	name=n;
	base=0;
	baseClient=0;
	baseFile=0;
	laser3d=0;
	laser=0;
	laserClient=0;
	laserFile=0;
	datalog=0;
}
MobileRobot::~MobileRobot()
{
	delete baseClient;
	for(unsigned int i=0;i<servers.size();i++)
	{
		servers[i]->close();
		delete servers[i];
	}

	delete laserClient;
	delete baseFile;
	delete laserFile;
	delete datalog;

//		delete base;
//		for(int i=0;i<lasers.size();i++)
//			delete lasers[i];	
}
bool MobileRobot::startLogging(string folder)
{
	datalog=new DataLogOut();
	if(!datalog->open(folder))
	{
		LOG_ERROR("Unable to open folder with log data");
		return false;
	}

	baseFile=new WheeledBaseFile();
	baseFile->saveDataTo((DataLogOut*)datalog,"base");
	
	laserFile=new LaserSensorFile;
	laserFile->saveDataTo((DataLogOut*)datalog,"laser");
//FIXME	s->saveDataTo(&datalog,name);
	
	return true;
}
bool MobileRobot::connectLog(string folder)
{
	datalog= new DataLogIn();;
	if(!datalog->open(folder))
	{
		LOG_ERROR("Unable to open folder with log data");
		return false;
	}
	baseFile=new WheeledBaseFile;
	baseFile->loadDataFrom((DataLogIn*)datalog,"base");
	
	laserFile=new LaserSensorFile;
	laserFile->loadDataFrom((DataLogIn*)datalog,"laser");
}
void MobileRobot::setLocation(const Transformation3D &p)
{
	base->setLocation(p);
}

bool MobileRobot::getOdometry(Odometry& odom)
{
	if(baseClient)//Connected to a server
	{
		if(baseClient->getOdometry(odom)){
			base->setLocation(odom.pose);
			if(baseFile)baseFile->setOdometry(odom);
			return true;
		}
		return false;
	}
	if(baseFile)
	{
		bool b=baseFile->getOdometry(odom);
		base->setLocation(odom.pose);
		return b;
	}
	return false;
}
bool MobileRobot::getPose3D(Pose3D& odom)
{
	if(baseClient)//Connected to a server
	{
		if(baseClient->getPose3D(odom)){
			return true;
		}
		return false;
	}
	return base->getPose3D(odom);
}
//void MobileRobot::invalidateLaserData()
//{
//	LaserData laser;
//	for(int i=0;i<(int)lasers.size();i++)
//	{
//		lasers[i]->setData(laser);
//	}
//	if(laser3d)
//	{
//		LaserData3D laser3;
//		laser3d->setData(laser3);
//	}
//}
bool MobileRobot::getLaserData(LaserData& laserD)
{
	if(laserClient!=0)//if connected to server
	{
		if(laserClient->getData(laserD))//try to get it
		{
			laser->setData(laserD);
			if(laserFile)
				laserFile->setData(laserD);
			return true;
		}
		return false;
	}
	if(laserFile)
	{
		bool b=laserFile->getData(laserD);
		laser->setData(laserD);
		return b;
	}
	return false;

}
bool MobileRobot::getLaserData3D(LaserData3D& laser)
{
	if(!laser3d)return false;

	if(laser3DClient)
	{
		if(laser3DClient->getData(laser))//try to get it
		{
			laser3d->setData(laser);
			return true;
		}
		return false;
	}
	else
		return laser3d->getData(laser);
}

bool MobileRobot::move(double speed, double rot)
{
	if(baseClient)
	{
		return baseClient->move(speed,rot);
	}
	
	if(base)
	{
		return base->move(speed,rot);
	}

	return false;
}
//void MobileRobot::updateSensorsData()
//{
//UPDATE EVERYTING
//	for(int i=0;i<(int)lasers.size();i++)
//		lasers[i]->updateSensorData();

//	if(laser3d)
//	{
//		laser3d->updateSensorData();
//	}
//}

///If this robot is to be served remotely 
void MobileRobot::startServers(int port)
{
	Server* s=new WheeledBaseServer(base,name+"Base");
	s->init(port++);
	servers.push_back(s);
	if(laser)
	{
		s=new LaserSensorServer(laser,name+"Laser");
		s->init(port++);
		servers.push_back(s);
	}
	if(laser3d)
	{
		s=new LaserSensor3DServer(laser3d,name+"Laser3D");
		s->init(port++);
		servers.push_back(s);
	}
}	
void MobileRobot::connectClients(string ip,int port)
{
	WheeledBaseClient* cl=new WheeledBaseClient;
	cl->connect(ip.c_str(),port++);
	baseClient=cl;
	if(laser)
	{
		LaserSensorClient* las=new LaserSensorClient;
		las->connect(ip.c_str(),port++);
		laserClient=las;
	}
	if(laser3d)
	{
		LaserSensor3DClient* laserClient=new LaserSensor3DClient;
		laserClient->connect(ip.c_str(),port++);
		laser3DClient=laserClient;	
	}
}


}; //end namespace mr


