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

#include "simulator.h"
#include "mobilerobot.h"
#include "personsim.h"
#include "../world/facesetpart.h"
#include "../world/prismaticpart.h"
#include "../world/spherepart.h"
#include "hw/wheeledbase/wheeledbaseserver.h"
#include "hw/lasersensor/lasersensorserver.h"
#include "base/streamfile.h"
#include "system/filesystem.h"

#include <fstream>

using namespace std;

namespace mr
{
Simulator::~Simulator()
{
	LOG_INFO("Closing servers");
	for(unsigned int i=0;i<servers.size();i++)
		servers[i]->close();

	LOG_INFO("Closed servers");
}

bool Simulator::load(string environment)
{
//	generateWorld();
	string fold=environment.substr(0,environment.find_last_of('/'));
	ifstream file(environment.c_str());
	string current=currentDirectory();
	changeDirectory(fold);
	string command,worldName;
	file>>command>>worldName;
	if(command!="world:")
	{
		LOG_ERROR("Bad config file, expected \"world:\"");
		changeDirectory(current);
		return false;
	}
	StreamFile worldFile(worldName,true);
	if(!worldFile.good())
	{
		LOG_ERROR("Bad world file "<<worldName);
		changeDirectory(current);
		return false;
	}
	if(!worldFile.read(&world))
	{
		LOG_ERROR("Error while loading world file "<<worldName);
		changeDirectory(current);
		return false;
	}
	//world.readFromStream(worldFile);
	
	int numRobots;
	file>>command>>numRobots;
	if(command!="robots:" || numRobots<0 ||numRobots>100)
	{
		LOG_ERROR("Bad number of robots, expected \"robots: #\"");
		changeDirectory(current);
		return false;
	}
	for(int i=0;i<numRobots;i++)
	{
		string robotName;
		int port;
		float x,y,z,yaw;
		file>>robotName>>port>>x>>y>>z>>yaw;
		MobileRobot* robotAux=0;
		if(robotName=="neo")
			robotAux=new Neo();
		else if(robotName=="nemo")
			robotAux=new Nemo();
		else if(robotName=="doris")
			robotAux=new Doris();
		if(robotAux)
		{
			Transformation3D giro(0,0,0,0,0,yaw);
			robotAux->setLocation(Transformation3D(x,y,z)*giro);
			robots.push_back(robotAux);
			world+=robotAux;
			robotAux->startServers(port);
		}
	}
	
	int numPeople;
	file>>command>>numPeople;
	if(command!="persons:" || numPeople<0 ||numPeople>100)
	{
		LOG_ERROR("Bad number of persons, expected \"persons: #\"");
		changeDirectory(current);
		return false;
	}
	for(int i=0;i<numPeople;i++)
	{
		string robotName;
		float x,y,z,h;
		file>>robotName>>h>>x>>y>>z;
		PersonSim* person=new PersonSim(h);
		person->setAbsoluteT3D(Transformation3D(x,y,z,0,0,-3+6*rand()/(float)RAND_MAX));

		world+=person;		
	}
	changeDirectory(current);
	return true;
}

};
/*
#include "wheeledbase/wheeledbasesim.h"
#include "wheeledbase/wheeledbaseserver.h"
#include "wheeledbase/wheeledbaseclient.h"
#include "wheeledbase/wheeledbasefile.h"
#include "lasersensor/lasersensorserver.h"
#include "lasersensor/lasersensorclient.h"
#include "lasersensor/lasersensorsim.h"
#include "lasersensor/lasersensorfile.h"
#include "lasersensor3d/lasersensor3dserver.h"
#include "lasersensor3d/lasersensor3dclient.h"
#include "lasersensor3d/lasersensor3dsim.h"
#include "lasersensor3d/lasersensor3dfile.h"
#include "lms200/lms200sim.h"
#include "pioneer3at/pioneer3atsim.h"
#include "patrolbot/patrolbotsim.h"
#include "net/server.h"
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
}
MobileRobot::~MobileRobot()
{
	delete baseClient;
	for(unsigned int i=0;i<servers.size();i++)
	{
		servers[i]->close();
		delete servers[i];
	}
	for(unsigned int i=0;i<laserClients.size();i++)
	{
		delete laserClients[i];
	}
	delete baseFile;
	for(unsigned int i=0;i<laserFiles.size();i++)
	{
		delete laserFiles[i];
	}
//		delete base;
//		for(int i=0;i<lasers.size();i++)
//			delete lasers[i];	
}
bool MobileRobot::startLogging(DataLogOut& datalog)
{
	if(!datalog.isOpen())
		return false;

	baseFile=new WheeledBaseFile();
	baseFile->saveDataTo(&datalog,"base");
	
	for(unsigned int i=0;i<lasers.size();i++)
	{
		LaserSensorFile* s=new LaserSensorFile;
		stringstream str;str<<"laser"<<i;
		string name=str.str();
		s->saveDataTo(&datalog,name);
		laserFiles.push_back(s);
	}
	
	return true;
}
void MobileRobot::setLocation(const Transformation3D &p)
{
	base->setRelativeT3D(p);
}

bool MobileRobot::getOdometry(Odometry& odom)
{
	if(baseClient)//Connected to a server
	{
		if(baseClient->getOdometry(odom)){
			base->setAbsoluteT3D(odom.pose);
			return true;
		}
		return false;
	}
	return base->getOdometry(odom);
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
bool MobileRobot::getLaserData(LaserData& laser,int index)
{
	if(index<0 ||index >= (int)lasers.size())return false;

	if(laserClients.size()==lasers.size())//client connected
	{
		if(laserClients[index]->getData(laser))//try to get it
		{
			lasers[index]->setData(laser);
			return true;
		}
		return false;
	}

	return lasers[index]->getData(laser);
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
	for(int i=0;i<(int)lasers.size();i++)
	{
		s=new LaserSensorServer(lasers[i],name+"Laser");
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
	for(int i=0;i<(int)lasers.size();i++)
	{
		LaserSensorClient* laserClient=new LaserSensorClient;
		laserClient->connect(ip.c_str(),port++);
		laserClients.push_back(laserClient);
	}
	if(laser3d)
	{
		LaserSensor3DClient* laserClient=new LaserSensor3DClient;
		laserClient->connect(ip.c_str(),port++);
		laser3DClient=laserClient;	
	}
}


}; //end namespace mr


*/
