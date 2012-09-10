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

#ifndef __SIMULATOR_INCLUDED_H_
#define __SIMULATOR_INCLUDED_H_

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

#include <vector>
#include "net/server.h"
#include "world/world.h"
#include "mobilerobot.h"
using namespace std;

namespace mr
{

//A mobile robot class represents a terrestial mobile base plus several sensors that
///can be attached to it, as lasers, laser3d, etc.

class Simulator
{
public:
	Simulator(){;}
	virtual ~Simulator();
	World* getWorld(){return &world;}//for painting

	bool load(string environment);
	MobileRobot* getRobot(int index){return robots[index];}
	int numRobots(){return robots.size();}
protected:
	vector<Server*> servers;
	vector<MobileRobot*> robots;
	World world;
};


}; //end namespace mr


#endif
