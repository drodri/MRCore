/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:   Miguel Hernando Gutierrez
 * 
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

#ifndef __MRCORE__WB_STATE__H
#define __MRCORE__WB_STATE__H

#include "plannerABC.h"


//////////////////////////WB_STATE
 // Especificationtion of a robot state for a wheeled base robot
 // The main features are: 
	//rain-drop state transforming a 3D sample in a set of allowed poses.
	//Local planning: a simple but fast planner
	//Distance: includes the consideration of allowable rotations and visibility
using namespace std;

namespace mr{

//short name for Wheeled Based Robot State
class WBState : public RobotState
{


public:
//Common interface
	virtual bool isEqual(RobotState *n);
	//check if it is a valid state
	virtual bool isValid();
	//metric used to evaluate de distances: it could be any measure, but
	//the less the better
	virtual double distanceTo(RobotState *p);
	//returns true if the robot state is equal to the state
	virtual bool isEqualToCurrentRobotState();
	//create a new state from current robot position
	virtual RobotState *createStateFromCurrentRobotState();
	//creates a robot state with num gdl doubles. if invalid return false
	virtual RobotState *createStateFromSample(vector<double> values);
	//moves the robot one step from the current position towards
	//the state. Local planner
	virtual bool moveRobotCloser(double stepSize);
	//place the robot in a state
	virtual void placeRobot();
	//place the robot in a state but prepared for achieving the target state
	virtual void placeRobotTowards(RobotState *target);

	virtual RobotState *clone();
	virtual ~WBState();
protected:
	//Specific data and methods
		class Sector{
		public:
		double min;
		double max;
		Transformation3D pose;
		void drawGL();
	};
	WheeledBaseSim *robot;
	World *world;
	Vector3D pose; //medium value
	vector<Sector> sectors;
public:
	double getSize(); //angulo total cubierto

	WBState(WheeledBaseSim *r,World *w){
		world=w;
		robot=r;
	}
	
	//using t as starting pose... drops the robot and check if there is a valid pose in that place
	//modifies t in that sense and returns true if there is such position
	bool checkPose(Transformation3D &t);
	

	//copy constructor and operador = 
	const WBState &operator=(const WBState &n){
		sectors=n.sectors;
		pose=n.pose;
		world=n.world;
		robot=n.robot;
		return *this;
	}
	WBState(const WBState &n){
		(*this)=n;
	}
	WBState *createStateFromPoint3D(double x,double y,double z);
	bool specialize(Transformation3D &t);
	int getNumSectors(){return (int)sectors.size();}
	double getClosestAngle(double ang);
	virtual vector<double> getSample();
	//Draw a RobotState representation
	 virtual void drawGL();

};

}
#endif  //__MRCORE__WB_STATE__H
