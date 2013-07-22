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
#include <vector>

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
	SimpleJoint* s_Joint;

	//kinematic simulation atributes
	double speed, maxSpeed; // m/s rad/s
	double acceleration,maxAcceleration; //rad/sec^2

	double target, targetIntermediate;
	bool targetActive; //true if target have to be reached
	string typeTrajectoryTVP;
	//spline algorithm
	double a0,a1,a2,a3; //polinomial coeficients

	bool activateTVP,activateCPT;

	int index;
	vector<double> velocInter;


public:
//constructors

//Basic Constructor
	Actuator(double _speed=1,double _maxSpeed=1,double _acceleration=0.1, double _maxAcceleration=0.5);
	//Destructor
	virtual ~Actuator(void);


//simulation methods
	void setSimulationParameters(double _maxSpeed, double _maxAcceleration=PI/6);//max acelerate default=30º/sec^2

	double setSpeed(double sp);
	double setAcceleration(double ac);

	double getSpeed(){return speed;}
	double getMaxSpeed(){return maxSpeed;}
	double getAcceleration(){return acceleration;}
	double getMaxAcceleration(){return maxAcceleration;}

	bool isMoving(){return targetActive;}
	void linkTo (PositionableEntity *p);

	virtual void simulate(double delta_t);

	void activateCubicPolynomialTrajectory(){activateTVP=false;activateCPT=true;}
	void activateTrapezoidalVelocityTrajectory(){activateTVP=true;activateCPT=false;}

	bool setTarget(double val);
	double getTarget(){return target;}

//Cubic Polinomial Trajectory (CPT)

	void simulateCPT(double delta_t);
	bool setTargetCPT(double _time);
	void setCubicPolinomialCoeficients(double path,double targetTime);


//Trapezoidal Velocity Profile tarjectory (TVP)

	void simulateTVP(double delta_t);
	bool setTargetTVP(double qInit,double q_target,int signMovement,double timeInit,double timeFinal,double ta, double _time);
	bool setTypeTrajectoryTVP(string _type);
	string getTypeTrajectoryTVP(){return typeTrajectoryTVP;}

// Spline trajetory (interpolation points)
	void setVelocIntermediates (vector<double> veloc);
	void simulateSPLINE(double delta_t);
	bool setTargetSPLINE(double _time);
	void setCubicPolinomialCoeficientsSPLINE(double strech,double Tk);



};

};
#endif  //__MRCORE__ACTUATOR__H
