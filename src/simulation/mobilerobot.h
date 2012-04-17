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


#include "wheeledbasesim.h"
#include "hw/wheeledbase/wheeledbaseserver.h"
#include "hw/wheeledbase/wheeledbaseclient.h"
#include "hw/wheeledbase/wheeledbasefile.h"
#include "hw/lasersensor/lasersensorserver.h"
#include "hw/lasersensor/lasersensorclient.h"
#include "lasersensorsim.h"
#include "hw/lasersensor/lasersensorfile.h"
#include "hw/lasersensor3d/lasersensor3dserver.h"
#include "nemolasersensor3dsim.h"
#include "hw/lasersensor3d/lasersensor3dclient.h"
#include "lasersensor3dsim.h"
#include "hw/lasersensor3d/lasersensor3dfile.h"
#include "lms200sim.h"
#include "lms100sim.h"
#include "pioneer3atsim.h"
#include "patrolbotsim.h"
#include "hw/quadrotor/quadrotorserver.h"
#include "hw/quadrotor/quadrotorclient.h"
#include "quadrotorsim.h"
#include "camerasim.h"
#include "hw/camera/cameraserver.h"
#include "hw/camera/cameraclient.h"

#include "net/server.h"

#include "base/globject.h"
#include "../world/composedentity.h"
#include "pioneer3atsim.h"
#include "lms100sim.h"

namespace mr
{

//A mobile robot class represents a terrestial mobile base plus several sensors that
///can be attached to it, as lasers, laser3d, etc.

class MobileRobot : public ComposedEntity
{
public:
	MobileRobot(string name);
	virtual ~MobileRobot();

	bool startLogging(string folder);
	void setLocation(const Transformation3D &p);

	//
	bool getOdometry(Odometry& odom);
	bool getLaserData(LaserData& laser);
	bool getPose3D(Pose3D& pose);
	bool getLaserData3D(LaserData3D& laser);
//	void invalidateLaserData();

	bool move(double speed, double rot);
///If this robot is to be served remotely 
	void startServers(int port);
	void connectClients(string ip,int port);
	bool connectLog(string folder);

protected:
	string name;
//Basic configuration (simulated, drawable)
	WheeledBaseSim* base;
	LaserSensorSim* laser;
	LaserSensor3DSim* laser3d;

//Data origin of this robot
	WheeledBase* baseClient;//can be client or datalog
	LaserSensor* laserClient;//can be client or datalog
	LaserSensor3D* laser3DClient;
	
//Data logging of this robot
	WheeledBaseFile* baseFile;//can be client or datalog
	LaserSensorFile* laserFile;//can be client or datalog

//if we want to remotely serve this robot, we use this servers
	vector<Server*> servers;
	Pose3D pose;
	DataLog* datalog;
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
		laser=new LMS200Sim;
		//laser->setDrawGLMode(0);
		laser->LinkTo(base);
		laser->setColor(100,0,150);
		laser->setRelativePosition(Vector3D(0.1,0,0.5));
		(*this)+=base;

	}
	virtual ~Doris()
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
	virtual ~Nemo()
	{
		
	}
};


class Neo: public MobileRobot
{
public: 
	Neo():MobileRobot("Neo")
	{
		base=new Pioneer3ATSim;
		laser=new LMS100Sim;
		laser->LinkTo(base);
		laser->setColor(150,150,0);
		laser->setRelativePosition(Vector3D(0.1,0,0.4));
		(*this)+=base;
	}
	virtual ~Neo()
	{
		
	}
protected:
	
};



}; //end namespace mr


#endif
