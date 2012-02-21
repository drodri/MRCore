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
#include "../world/facesetpart.h"
#include "../world/prismaticpart.h"
#include "../world/spherepart.h"
#include "hw/wheeledbase/wheeledbaseserver.h"
#include "hw/lasersensor/lasersensorserver.h"

namespace mr
{
Simulator::~Simulator()
{
	LOG_INFO("Closing servers");
	for(int i=0;i<servers.size();i++)
		servers[i]->close();

	LOG_INFO("Closed servers");
}
void Simulator::load(string environment)
{
	if(environment=="simple")
	{
		Face suelo(Transformation3D(0,0,0),-20,-20,20,20);
		Face tablon_fino1(Transformation3D(8,3,2,X_AXIS,-1.53),0,0,5.2,3.95);
		FaceSetPart *building=new FaceSetPart; 
		building->addFace(suelo);
		building->addFace(tablon_fino1);
		world+=building;

		Neo* neo=new Neo();
		world+=neo;
		neo->startServers(13000);
		
		return;
	}
	
	if(environment=="rampas")
	{
	//Intializing test environment Faces included in a FacePart
	Face suelo(Transformation3D(0,0,0),0,-10,10,10);
	Face tablon_fino1(Transformation3D(8,3,2,X_AXIS,-0.53),0,0,0.2,3.95);
	Face tablon_fino2(Transformation3D(8.5,3,2,X_AXIS,-0.53),0,0,0.2,3.95);
	Face tablon_grueso(Transformation3D(2,3,2,X_AXIS,-0.53),0,0,1,3.95);
	Face plataforma(Transformation3D(2,0,2),0,0,8,3);
	Face paredfondo1(Transformation3D(0,0,0,Y_AXIS,PI/2),-4,-10,0,10);
	Face paredfondo2;

	paredfondo2.setBase(Transformation3D(0,0,0,X_AXIS,-PI/2));
	paredfondo2.addVertex(0,-4);
	paredfondo2.addVertex(10,-4);
	paredfondo2.addVertex(10,0);
	paredfondo2.addVertex(6,0);
	paredfondo2.addVertex(6,-1.5);
	paredfondo2.addVertex(4,-1.5);
	paredfondo2.addVertex(4,0);
	paredfondo2.addVertex(0,0);

	FaceSetPart *building=new FaceSetPart; 
	building->addFace(suelo);
	building->addFace(tablon_fino1);
	building->addFace(tablon_fino2);
	building->addFace(tablon_grueso);
	building->addFace(plataforma);
	building->addFace(paredfondo1);
	building->addFace(paredfondo2);
	
	world+=building;

	FaceSetPart *paux2=new FaceSetPart(*building);
	paux2->setRelativeOrientation(0,0,PI);
	paux2->setRelativePosition(Vector3D(20,0,0));
	world+=paux2;
	
//probando el prismaticpart
	PrismaticPart *mypart=new PrismaticPart;
	vector<Vector2D> list;
	list.push_back(Vector2D(0,0));
	list.push_back(Vector2D(0,1));
	list.push_back(Vector2D(1,1));
	list.push_back(Vector2D(1.5,0.5));
	list.push_back(Vector2D(1,0));
	mypart->setPolygonalBase(list);
	mypart->setRelativePosition(Vector3D(7,7,3));
	mypart->setRelativeOrientation(PI/2,0,0);
	mypart->setHeight(4);

	SpherePart *esfera=new SpherePart;
	esfera->setRelativePosition(Vector3D(7,7,6));
	world+=mypart;
	world+=esfera;

	return;
	}

	LOG_ERROR("Environment not found: "<<environment);
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