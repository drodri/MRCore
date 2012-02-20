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

#ifndef __MOBILE_ROBOT_INCLUDED_H_
#define __MOBILE_ROBOT_INCLUDED_H_

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
#include "nemolasersensor3d/nemolasersensor3dsim.h"
#include "lasersensor3d/lasersensor3dclient.h"
#include "lasersensor3d/lasersensor3dsim.h"
#include "lasersensor3d/lasersensor3dfile.h"
#include "lms200/lms200sim.h"
#include "lms100/lms100sim.h"
#include "pioneer3at/pioneer3atsim.h"
#include "patrolbot/patrolbotsim.h"
#include "quadrotor/quadrotorserver.h"
#include "quadrotor/quadrotorclient.h"
#include "quadrotor/quadrotorsim.h"
#include "camera/camerasim.h"
#include "camera/cameraserver.h"
#include "camera/cameraclient.h"

#include "net/server.h"*/

#include "base/globject.h"
#include "../world/composedentity.h"
#include "pioneer3atsim.h"
#include "lms100sim.h"
namespace mr
{
class MobileRobot: public ComposedEntity
{
public:
	MobileRobot(string name);
	~MobileRobot();

	void setLocation(const Transformation3D &p);

	WheeledBaseSim* getBase(){return base;}
	int numLasers(){return lasers.size();}
	LaserSensorSim* getLaser(int i){return lasers[i];}
	bool move(double speed, double rot);

protected:
	string name;
//Basic configuration (simulated, drawable)
	WheeledBaseSim* base;
	vector<LaserSensorSim*> lasers;
};
class Neo: public ComposedEntity
{
public: 
	Neo()
	{
		base=new Pioneer3ATSim;
		laser=new LMS100Sim;
		laser->LinkTo(base);
		laser->setColor(150,150,0);
		laser->setRelativePosition(Vector3D(0.1,0,0.4));
		(*this)+=base;
	}
	~Neo()
	{
		
	}
	Pioneer3ATSim* getBase(){return base;}
	LMS100Sim* getLaser(){return laser;}
protected:
	Pioneer3ATSim* base;
	LMS100Sim* laser;
};
}
/*
//A mobile robot class represents a terrestial mobile base plus several sensors that
///can be attached to it, as lasers, laser3d, etc.

class MobileRobot
{
public:
	MobileRobot(string name);
	~MobileRobot();

	bool startLogging(DataLogOut& datalog);
	void setLocation(const Transformation3D &p);

	///method useful for adding this robot to a world
	WheeledBaseSim* getBase(){return base;}
	//
	bool getOdometry(Odometry& odom);
	bool getLaserData(LaserData& laser,int index=0);
	bool getLaserData3D(LaserData3D& laser);
//	void invalidateLaserData();

	bool move(double speed, double rot);
///If this robot is to be served remotely 
	void startServers(int port);
	void connectClients(string ip,int port);

protected:
	string name;
//Basic configuration (simulated, drawable)
	WheeledBaseSim* base;
	vector<LaserSensorSim*> lasers;
	LaserSensor3DSim* laser3d;

//Data origin of this robot
	WheeledBase* baseClient;//can be client or datalog
	vector<LaserSensor*> laserClients;//can be client or datalog
	LaserSensor3D* laser3DClient;
	
//Data logging of this robot
	WheeledBaseFile* baseFile;//can be client or datalog
	vector<LaserSensorFile*> laserFiles;//can be client or datalog

//if we want to remotely serve this robot, we use this servers
	vector<Server*> servers;
};
class QuadrotorRobot
{
public:
	QuadrotorRobot(string n)
	{
		client=0;
		server=0;
		name=n;

		quadrotor=new QuadrotorSim;
		frontCamera=new CameraSim;
		frontCamera->LinkTo(quadrotor);
		frontCamera->setColor(100,0,150);
		frontCamera->setRelativePosition(Vector3D(0.1,0,0.5));
	
		downCamera=new CameraSim;
		downCamera->LinkTo(quadrotor);
		downCamera->setColor(0,255,0);
		downCamera->setRelativePosition(Vector3D(0.5,0,0.1));	
	}
	virtual ~QuadrotorRobot()
	{
		if(server)server->close();
		if(client)client->close();
		delete client;
		delete server;
	}
	void setLocation(const Transformation3D &p)
	{
		quadrotor->setRelativeT3D(p);
	}
	bool move(double up,double forward,double lateral,double rot)
	{
		if(client)
		{
			return client->move(up,forward,lateral,rot);
		}	
		return quadrotor->move(up,forward,lateral,rot);
	}
	bool takeOff()
	{
		if(client)
		{
			return client->takeOff();
		}	
		return quadrotor->takeOff();
	}
	bool land()
	{
		if(client)
		{
			return client->land();
		}	
		return quadrotor->land();
	}
	void startServers(int port)
	{
		server=new QuadrotorServer(quadrotor,name);
		server->init(port++);
	}
	void connectClients(string ip,int port)
	{
		client=new QuadrotorClient;
		client->connect(ip.c_str(),port++);
	}
	QuadrotorSim* getBase(){return quadrotor;}
protected:
	string name;
	QuadrotorSim* quadrotor;
	QuadrotorClient* client;
	QuadrotorServer* server;
	CameraSim* frontCamera;
	CameraSim* downCamera;


};

class Doris: public MobileRobot
{
public: 
	Doris():MobileRobot("Doris")
	{
		base=new PatrolbotSim;
		LaserSensorSim* las=new LMS200Sim;
		las->setDrawGLMode(0);
		las->LinkTo(base);
		las->setColor(100,0,150);
		las->setRelativePosition(Vector3D(0.1,0,0.5));
		lasers.push_back(las);
	}
	~Doris()
	{
		
	}
};

class Nemo: public MobileRobot
{
public: 
	Nemo():MobileRobot("Nemo")
	{
		base=new Pioneer3ATSim;
		//LaserSensorSim* las=new LMS200Sim;
		//las->LinkTo(base);
		//las->setColor(150,0,150);
		//las->setRelativePosition(Vector3D(0.1,0,0.5));
		//las->setRelativeOrientation(0,30*DEG2RAD,0);
		//lasers.push_back(las);
		laser3d=new NemoLaserSensor3DSim;
		laser3d->LinkTo(base);
		laser3d->setRelativePosition(Vector3D(0.1,0,0.3));
	}
	~Nemo()
	{
		
	}
};




}; //end namespace mr
*/

#endif
