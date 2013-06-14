/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
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

#ifndef __MRCORE__ACTUATOR__H
#define __MRCORE__ACTUATOR__H


#include "simplejoint.h"
#include "math/mrmath.h"


using namespace std;

namespace mr
{
	class SimpleJoint;
/**
    \class Actuator
    Actuator	->	Implementation to simulate simplejoints movement
*/

class Actuator: public PositionableEntity
{
	DECLARE_MR_OBJECT(Actuator)
	//friend class World;

protected:
	 //double jointVmax;
	 //double jointVmin;
	//kinematic simulation atributes
	double maxSpeed; // m/s rad/s
	double speed;    //m/s rad/sec
	double target;
	bool targetActive; //true if target have to be reached

	SimpleJoint* s_Joint;
public:
//constructors

	//Basic Constructor
	Actuator(SimpleJoint* _sjoint=0,double _maxSpeed=1,double _speed=1,double _target=0,bool _targetOn=false);
	//Destructor
	virtual ~Actuator(void);


	//simulation methods
   void setSimulationParameters(double _maxSpeed, double _speed=0);

   double setSpeed(double sp){
	   if(sp<0)return speed;
	   speed=sp>maxSpeed?maxSpeed:sp;
	   return speed;
   }

   double getSpeed(){return speed;}
   double getMaxSpeed(){return maxSpeed;}

   bool setTarget(double val);
   double getTarget(){return target;}
   bool isMoving(){return targetActive;}

   virtual void simulate(double delta_t);

};

};
#endif  //__MRCORE__ACTUATOR__H
