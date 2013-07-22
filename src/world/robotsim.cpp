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

#include "robotsim.h"
#include "../world/world.h"
#include "../world/referencesystem.h"
#include <iostream>

namespace mr
{
bool RobotSim::checkRobotColision()
{
//checks the collision of robot and the rest of the objects
	bool prev=isIntersectable();
	this->setIntersectable(false);
	World *w=getWorld();
	int i;
	for(i=0;i<(int)(objects.size());i++)
	{	
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		//en cuanto hay colision devuelvo true
		if(aux){
			if(aux!=this->links[0])
			if(w->checkCollisionWith(*aux))return true;
		}
	}
//checks the collision of grasped objects with the world
	if(tcp!=0){
		ReferenceSystem *tcpr=tcp->getReferenciableLocation();
		int n=tcpr->getNumberOfDependents();

		for(int j=0;j<n;j++)
		{
			ReferenceSystem* raux=tcpr->getDependent(j);
			SolidEntity *aux=dynamic_cast<SolidEntity *>(raux->getOwner());
			if(aux){
				if(w->checkCollisionWith(*aux))return true;
			}
		}

	}

//checks the collision of robot links between them
	setIntersectable(prev);
return false;
}
bool RobotSim::checkJointValues(const vector<double> & _q) const
{
//check dimension
	if(_q.size()!=joints.size())return false;
	for(int i=0;i<(int)joints.size();i++){
		if(joints[i]->checkValue(_q[i])==false)return false;
	}
	return true;
}
bool RobotSim::forwardKinematicsAbs(vector<double> _q, Transformation3D& t)
{
	int i;
	if(tcp==0)return false;
	if(links.size()==0)return false;

	if(!checkJointValues(_q))return false; //limits checking
	
	vector<double> qold;
	//save current joint values and update the joint values
	for(i=0;i<(int)joints.size();i++){
		qold.push_back(joints[i]->getValue());
		joints[i]->setValue(_q[i]);
	}

	// Compute the relative transformation between the tcp and the robot base
	t=tcp->getAbsoluteT3D();

	// Move the robot to the qold values
	for(i=0;i<(int)joints.size();i++)joints[i]->setValue(qold[i]);
	return true;
}
bool RobotSim::inverseKinematicsAbs(Transformation3D t, vector<double> &_q, unsigned char conf)
{
	Transformation3D t_base=links[0]->getAbsoluteT3D();
	Transformation3D aux=t_base.inverted();
	Transformation3D t_p=(aux)*(t);
	return inverseKinematics(t_p,_q,conf);
	
}
bool  RobotSim::forwardKinematics(const vector<double> & _q, Transformation3D& t)//conf puntero salida
{

	// Compute the relative transformation between the tcp and the robot base
	Transformation3D _t;
	forwardKinematicsAbs(_q,_t);
	Transformation3D aux=links[0]->getAbsoluteT3D();
	t=(aux.inverted())*_t;

	return true;
}
unsigned char RobotSim::getCurrentConfiguration()
{
	vector<double> q;
	unsigned char conf;
	//save current joint values and update the joint values
	for(int i=0;i<(int)joints.size();i++)q.push_back(joints[i]->getValue());
	getConfigurationOf(q,conf);
	return conf;
	
}
bool  RobotSim::moveTo(Transformation3D t3d, unsigned char conf)
{
/***
the T3D is a relative transformation to the base (link[0])
***/

	if(conf==0x0)conf=getCurrentConfiguration();

	vector<double> q;
	if(inverseKinematics(t3d,q,conf))
	{
		for(int i=0;i<(int)joints.size();i++)
			joints[i]->setValue(q[i]);
		return true;
	}
	else return false;

}
bool RobotSim::moveTo(const vector<double> & _q)
{

	if(_q.size()!=joints.size())return false;
	if(!checkJointValues(_q))return false; //limits checking
	//for(int i=0;i<(int)joints.size();i++)joints[i]->setValue(_q[i]);
	goTo(_q);
	return true;
}
bool  RobotSim::moveToAbs(Transformation3D t3d, unsigned char conf)
{

	Transformation3D t_base=links[0]->getAbsoluteT3D();
	Transformation3D aux=t_base.inverted();
	Transformation3D t_p=(aux)*(t3d);

	return moveTo(t_p,conf);
}
Transformation3D RobotSim::getTcpAbsLocation()
{
	return tcp->getAbsoluteT3D();
}
Transformation3D RobotSim::getTcpLocation()
{
	Transformation3D _t=tcp->getAbsoluteT3D();
	Transformation3D aux=links[0]->getAbsoluteT3D();
	return (aux.inverted())*_t;
}
bool RobotSim::moveTo(double *_q)
{
	int num=(int)joints.size();
	vector<double> q;
	for(int i=0;i<num;i++)q.push_back(_q[i]);
	return moveTo(q);
}


void RobotSim::simulate(double delta_t)
{

	if((int)q_target.size() == (int)joints.size())
	{
		double error=0;
		for(int i=0;i<(int)joints.size();i++)
			error+=fabs(q_target[i]-joints[i]->getValue());//*

		if( (time+delta_t) >= targetTime)//*
		{
			for(int i=0;i<(int)joints.size();i++)
				actuators[i]->setTarget(q_target[i]);

			q_target.clear();
			time=0.0;
			targetTime=0.0;
			posInit=true;
			qInit.clear();
		}
		else
		{
			checkActivationTrajectory();

			if (activateTVP)
				calculateTargetTVP(time);
			else if (activateCPT)
				calculateTargetCPT(time);
			//calculateTargetSPLINE(0.1);

			time+=delta_t;
		}
	}

	ComposedEntity::simulate(delta_t);
}


void RobotSim::goTo(vector<double> q)
{

	q_target=q;//Target loaded

	double error=0;
	for(int i=0;i<(int)joints.size();i++)
		error+=fabs(q_target[i] - joints[i]->getValue());//*

	if(fabs(error) < EPS)
	{
		q_target.clear();
		return;
	}
	else
	{
		time=0.0;

		checkActivationTrajectory();

		if (activateTVP)
			calculateTargetTimeTVP();
		else if (activateCPT)
			calculateTargetTimeCPT();
		//calculateTargetTimeSPLINE();
	}

	return;
}

void RobotSim::checkActivationTrajectory()
{
	//Check if there are two trajectories activated/desactivated
	if((!activateTVP && !activateCPT) || (activateTVP && activateCPT))
	{//default selection TVP
		activateTVP=true;
		activateCPT=false;
	}
}

void RobotSim::goToAbs(Transformation3D t)
{
	vector<double> q;
	if(!inverseKinematicsAbs(t,q))return;
	goTo(q);
	return;
}

void RobotSim::goTo(Transformation3D t)
{
	vector<double> q;
	unsigned char conf=getCurrentConfiguration();
	if(!inverseKinematics(t,q,conf))return;
	goTo(q);
	return;
}

bool RobotSim::checkActuatorsIsMoving()
{
	for(int i=0;i<(int)actuators.size();i++)
	{
		if(actuators[i]->isMoving())
			return true;
	}
	return false;
}

/*************************************************************
	METHODS TO CALCULATE TARGET BY CUBIC POLINOMIAL TRAJECTORY
**************************************************************/
void RobotSim::calculateTargetTimeCPT()
{
	double lowestSpeed=100.0;
	double longestPath=0.0;
	vector<double> pathJoint;


	for(int i=0;i<(int)actuators.size();i++)
	{
		pathJoint.push_back(q_target[i] - joints[i]->getValue());//Path each coordinate: target minus current coordinates

		if(actuators[i]->getMaxSpeed() <= lowestSpeed)
			lowestSpeed = actuators[i]->getMaxSpeed();
		if(fabs(pathJoint[i])>= longestPath)
			longestPath =fabs(pathJoint[i]);//*
	}
	targetTime = longestPath / lowestSpeed;

	for(int i=0;i<(int)actuators.size();i++)
	{
		actuators[i]->setCubicPolinomialCoeficients(pathJoint[i],targetTime);
	}
}

void RobotSim::calculateTargetCPT(double _time)
{
	for(int i=0;i<(int)actuators.size();i++)
	{
		actuators[i]->setTargetCPT(_time);
	}
}

/******************************************************************
	METHODS TO CALCULATE TARGET BY TRAPEZOIDAL VELOCITY PROFILE
*******************************************************************/

void RobotSim::calculateTargetTimeTVP()
{
	if (joints.size()!=actuators.size())return;
	if (q_target.size()!=joints.size())return;

	posInit=true;

	double auxTargetTime,auxTa;
	double maxTa,maxTargetTime=0.0;
	double maxSpeed,maxAcceleration;
	double condition;
	vector<double> path;
	int index=0;

	/* Check which is the maximum time to get the target by all the joints.
	For this we obtain the max values of the speed and acceleration of the joints and check
	with their own path to travel  */

	for(int i=0;i<(int)actuators.size();i++)
	{
		maxSpeed=actuators[i]->getMaxSpeed();
		maxAcceleration=actuators[i]->getMaxAcceleration();

		condition=(maxSpeed*maxSpeed)/maxAcceleration;

		path.push_back(fabs(joints[i]->getValue()-q_target[i]));

		if (condition>path[i])//Bang Bang movement
		{
			auxTargetTime=2*sqrt(path[i]/maxAcceleration);//targetTime
			auxTa=0.5*auxTargetTime;//ta
		}
		else // speed and acceleration with maximus values
		{
			auxTargetTime=path[i]/maxSpeed + maxSpeed/maxAcceleration;//targetTime
			auxTa=maxSpeed/maxAcceleration;//ta
		}

		if (maxTargetTime<auxTargetTime)//choose the max values
		{
			index=i;
			maxTargetTime=auxTargetTime;
			maxTa=auxTa;
		}

	}

	targetTime=maxTargetTime;
	ta=maxTa;

	if(maxTa==(0.5*maxTargetTime))
		actuators[index]->setTypeTrajectoryTVP("BangBang");
	else
	{
		actuators[index]->setSpeed(actuators[index]->getMaxSpeed());
		actuators[index]->setAcceleration(actuators[index]->getMaxAcceleration());
		actuators[index]->setTypeTrajectoryTVP("MaximumSpeedAcceleration");
	}

	adjustMotionJointsTVP(path,maxTargetTime,maxTa,index);
}

void RobotSim::adjustMotionJointsTVP (vector<double> _path,double _targetTime,double _ta,int index)
{
	//we adjust the speed and acceleration to get all the joints are finishing the movement at the same time

	if(_targetTime<=0 && _ta<=0)return;

	double speed,acel,maxSp,maxAcel;
	vector<double> path=_path;

	for(int i=0;i<(int)actuators.size();i++)
	{
		if(i==index)continue;
		maxSp=actuators[i]->getMaxSpeed();
		maxAcel=actuators[i]->getMaxAcceleration();

		//movement with speed and acceleration values adjust to timeTarget global
		speed=path[i]/(_targetTime-_ta);
		acel=speed/_ta;

		if (speed>maxSp || acel>maxAcel)//if true, try to change the movement to Bang Bang
		{
			acel=path[i]/(_ta*_ta);
			speed=acel*_ta;

			if (speed>maxSp || acel>maxAcel)
				return; //we have to recalculate the ta time to this actuator
			actuators[i]->setAcceleration(acel);
			actuators[i]->setSpeed(speed);
			actuators[i]->setTypeTrajectoryTVP("BangBang");
		}
		else
		{
			actuators[i]->setSpeed(speed);
			actuators[i]->setAcceleration(acel);
			actuators[i]->setTypeTrajectoryTVP("MaximumSpeedAcceleration");
		}
	}
}


void RobotSim::calculateTargetTVP(double _time)
{

	double timeInit,timeFinal;
	//double target;
	timeInit=0.0;
	timeFinal=targetTime;
	int signMovement=1;

	if(q_target.size()!=actuators.size())return;
	if(joints.size()!=actuators.size())return;

	if (posInit) //we need the initial values of the joints
	{
		for (int i=0;i<actuators.size();i++)
		{
			qInit.push_back(joints[i]->getValue());

		}
		posInit=false;
	}

	for (int i=0;i<actuators.size();i++)
	{
		if ((q_target[i]-qInit[i])<0)
			signMovement=-1;//if we go to target in the negative cuadrant
		else
			signMovement=1;

		actuators[i]->setTargetTVP(qInit[i],q_target[i],signMovement,timeInit,timeFinal,ta,_time);

		//if(actuators[i]->getTypeTrajectoryTVP()=="MaximumSpeedAcceleration")
		//{
		//	//Acceleration phase
		//	if (_time<(timeInit+ta) && _time>=timeInit)
		//		target=qInit[i]+signMovement*((actuators[i]->getAcceleration()*0.5)*square(_time-timeInit));

		//	//Velocity constant phase
		//	if (_time>=(timeInit+ta) && _time<=(timeFinal-ta))
		//		target=qInit[i]+signMovement*(actuators[i]->getSpeed()*(_time-timeInit-ta*0.5));

		//	//Deceleration phase
		//	if (_time>(timeFinal-ta) && _time<=timeFinal)
		//		target=q_target[i]-signMovement*((actuators[i]->getAcceleration()*0.5)*square(timeFinal-_time));
		//}
		//else if(actuators[i]->getTypeTrajectoryTVP()=="BangBang")
		//{
		//	//Acceleration phase
		//	if (_time<(timeFinal*0.5) && _time>=timeInit)
		//		target=qInit[i]+signMovement*((actuators[i]->getMaxAcceleration()*0.5)*square(_time-timeInit));

		//	//Deceleration phase
		//	if (_time>=(timeFinal*0.5) && _time<=timeFinal)
		//		target=q_target[i]-signMovement*((actuators[i]->getMaxAcceleration()*0.5)*square(timeFinal-_time));
		//}

		//actuators[i]->setTarget(target);
	}

}


/******************************************************************
	METHODS TO CALCULATE TARGET BY SPLINE TRAJECTORY
*******************************************************************/

void RobotSim::calculateTargetTimeSPLINE()
{
	double lowestSpeed=100.0;
	double longestPath=0.0;
	vector<double> pathJoint;


	for(int i=0;i<(int)actuators.size();i++)
	{
		pathJoint.push_back(q_target[i] - joints[i]->getValue());//Path each coordinate: target minus current coordinates

		if(actuators[i]->getMaxSpeed() <= lowestSpeed)
			lowestSpeed = actuators[i]->getMaxSpeed();
		if(fabs(pathJoint[i])>= longestPath)
			longestPath =fabs(pathJoint[i]);//*
	}
	targetTime = longestPath / lowestSpeed;

	double Tk=0.1,stretch=0.1;
	int nIterations=(int)(targetTime/Tk);
	vector<double> a,b,c,d;
	for (int i=0;i<nIterations+2;i++)
	{
		a.push_back(Tk);
		b.push_back(4*Tk);//2*(Tk+Tk)
		c.push_back(Tk);
		d.push_back(6*stretch);//(3/(Tk^2))*(2*Tk^2*(qk+1-qk))
	}
	vector<double> veloc= TDMA(a,b,c,d,nIterations);
	for (int i=1;i<(int)actuators.size();i++)
	{
		actuators[i]->setVelocIntermediates(veloc);
	}
}

void RobotSim::calculateTargetSPLINE(double _time)
{
	double strech=0.1,Tk=0.1;
	for(int i=0;i<(int)actuators.size();i++)
	{
		actuators[i]->setCubicPolinomialCoeficientsSPLINE(strech,Tk);
		actuators[i]->setTargetSPLINE(_time);
	}
}


vector<double> RobotSim::TDMA (vector<double> a, vector<double> b, vector<double> c,vector<double> d, int nIterations)
{
	/*It's the Thomas algorithm to resolve tridiagonal matrix. It's useful
	to calculate the intermediate values of the velocity in the points to
	interpolate (vk).

	c'1=c1/b1
	c'i= ci/(bi-c'[i-1]*ai)		i=2...n-1

	d'1=d1/b1
	d'i=(di-d'[i-1]*ai)/(bi-c'[i-1]*ai)	i=2...n

	xn=d'n
	xi=d'i-c'i*x[i+1]	i=n-1....1
	*/



	double denom;
	vector<double> gamma,betta,x;
	gamma.push_back(0);
	betta.push_back(0);

	int i=0;

	//a=2*(Tk+Tk)=4*Tk; b=Tk; c=Tk;
	//dk = (3/(Tk*Tk))*((Tk*Tk*(path))+(Tk*Tk*(path)));
	//dk = (3*(2*path))

	//dk = 6*path; //more simplificate

	for (i=1;i<=nIterations;i++)
	{
		denom=a[i]*gamma[i-1]+b[i];
		if(i!=nIterations)
			gamma.push_back((-c[i])/denom);
		betta.push_back((d[i]-(a[i]*betta[i-1]))/denom);
	}

	//calculate the solutions
	x.push_back(betta[nIterations]);
	int index=0;

	for (i=nIterations-1;i>=1;i--)
	{
		x.push_back(gamma[i]*x[index++]+betta[i]);
	}

	//reordenate the vector  to return all the velocities in order
	vector<double> veloc;

	veloc.push_back(0.0);//veloc init

	for(i=0;i<(int)x.size();i++)
	{
		veloc.push_back(x[i]);
	}
	veloc.push_back(0.0);//veloc final

	return veloc;

}



};//Namespace mr