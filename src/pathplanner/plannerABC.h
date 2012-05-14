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

#ifndef __MRCORE__PLANNER_ABC__H
#define __MRCORE__PLANNER_ABC__H


#include "genericsamplers.h"
#include "../simulation/wheeledbasesim.h"

namespace mr{

#define RESOLUTION 0.2
//base class for representing the state and common operations between states

class RobotState
{
	//The derived clases have to have the atributes that includes the
	//robot, world, etc... needed for their correct working
public:
	virtual bool isEqual(RobotState *n)=0;
	//check if it is a valid state
	virtual bool isValid()=0;
	//metric used to evaluate de distances: it could be any measure, but
	//the less the better
	virtual double distanceTo(RobotState *p)=0;
	//returns true if the robot state is equal to the state
	virtual bool isEqualToCurrentRobotState()=0;
	//create a new state from current robot position
	virtual RobotState *createStateFromCurrentRobotState()=0;
	//creates a robot state with num gdl doubles. if invalid return false
	virtual RobotState *createStateFromSample(vector<double> values)=0;
	//moves the robot one step from the current position towards
	//the state. Local planner
	virtual bool moveRobotCloser(double stepSize)=0;
	//place the robot in a state
	virtual void placeRobot()=0;
	//place the robot towards
	virtual void placeRobotTowards(RobotState *target)
	{
		//usually it is not important where is going except for 
		//differential robots
		placeRobot();
	}
	//returns the sample use to generate the state, or
	//a sample equivalent to the state
	virtual vector<double> getSample()=0;
	virtual RobotState *clone()=0;
	virtual ~RobotState(){}
	virtual void drawGL(){}

};
//base class that represents a succesion of RobotStates. Base class for common operations
class RobotPath
{
public:
	vector<RobotState *> path;
	const RobotPath& operator=(const RobotPath &p){path=p.path;return *this;}
	RobotPath(const RobotPath &p){*this=p;}
	RobotPath(){}
	virtual void drawGL();
	virtual void filterLoops();
	virtual void filter();
	//an standard local path planner. New Robot States are placed into the new Path.
	//if the path is complete success will be true... if nt, the path would contain 
	//the tried trayectory until an invalid state is reached
	static RobotPath *createPath(RobotState *init, RobotState *end, bool &success);
	void eraseStates(){
		for(int i=0;i<(int)path.size();i++)delete path[i];
		path.clear();
	}
	RobotState *operator[](int i){return path[i];} 
	int size(){return (int)path.size();}
	void add(RobotState *n){path.push_back(n);}
	RobotState *last(){if(path.size()==0)return 0; return path[path.size()-1];}
};

//abstract class for any pathplanner... just an interface
class PathPlanner
{
protected:
	RobotState *start;
	RobotState *goal;
	RobotPath *path;
	bool solved;
public:
	PathPlanner(){
		start=0;goal=0;
		path=0;
		solved=false;
	};
	virtual bool setStartAndGoalStates(RobotState *start_, RobotState *goal_);
	
	virtual bool computePlan(int maxiterations)=0;
	virtual const RobotPath *getPlan(){if(solved)return path;else return 0;}
	bool isSolved(){return solved;}
	virtual ~PathPlanner(){
		delete start;
		delete goal;
		delete path;
	}
	bool isValid(){
		if(start==0)return false;
		if(goal==0)return false;
		return true;
	}
	//a function for painting the planner information if needed
	virtual void drawGL(){}
};

//abstract class for any Sampled Based pathplanner
class SBPathPlanner:public PathPlanner
{
protected:
	Sampler *sampler;

public:
	SBPathPlanner():PathPlanner(){
		sampler=0;
	}
	//be careful.. the sampler have to be compatible with the robotstate
	void setSampler(Sampler *s){
		sampler=s;
	}
	bool setStartAndGoalStates(RobotState *start_, RobotState *goal_);

	RobotState *getNextSample(){
		if((sampler==0)||(start==0))return 0;
		RobotState *ret=start->createStateFromSample(sampler->getNextSample());
		if(ret==0)return 0;
		if(ret->isValid())return ret;
		delete ret;
		return 0;
	}
};

}
#endif  //__MRCORE__PLANNER_TEMPLATES__H
