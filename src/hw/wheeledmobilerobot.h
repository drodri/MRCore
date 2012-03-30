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

#ifndef __WHEELED_MOBILE_ROBOT_H_
#define __WHEELED_MOBILE_ROBOT_H_

#include "data/odometry.h"
#include "wheeledbase.h"
#include "lasersensor.h"

namespace mr
{
class WheeledMobileRobot
{
public:
	WheeledMobileRobot(string name);
	~WheeledMobileRobot();

	bool load(string configFile);
	void setLocation(const Transformation3D &p);

	WheeledBase* getBase(){return base;}
	int numLasers(){return lasers.size();}
	LaserSensor* getLaser(int i){return lasers[i];}


protected:
	string name;
//Basic configuration (simulated, drawable)
	WheeledBase* base;
	vector<LaserSensor*> lasers;
	vector<Transformation3D> laserOffsets;//the location of the lasers within the robot
};

}; //end namespace mr

#endif
