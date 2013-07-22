/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio &Miguel Hernando Gutierrez
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


#include "actuator.h"

#include <math.h>

namespace mr{
IMPLEMENT_MR_OBJECT(Actuator)

Actuator::Actuator(double _speed,double _maxSpeed,double _acceleration, double _maxAcceleration)
{
	name="Actuator";
	s_Joint=0;
	maxSpeed=_maxSpeed;
	speed=_speed;
	target=0;
	targetActive=false;
	acceleration=_acceleration;
	maxAcceleration=_maxAcceleration;

	a0=a1=a2=a3=0.0;

	activateTVP=true;activateCPT=false;
	typeTrajectoryTVP="MaximumSpeedAcceleration";

	//
	index=0;
}


Actuator::~Actuator(void)
{

}

void  Actuator::setSimulationParameters(double _maxSpeed,  double _maxAcceleration)
{
	if(_maxSpeed<0)
		return;
	maxSpeed=_maxSpeed;
	if(_maxAcceleration<=0)
		return;
	maxAcceleration=_maxAcceleration;
}


double Actuator::setSpeed(double sp)
{
	if(sp<0)return speed;
	speed=sp>maxSpeed?maxSpeed:sp;
	return speed;
}

double Actuator::setAcceleration(double ac)
{
	if(ac<0)return acceleration;
	acceleration=ac>maxAcceleration?maxAcceleration:ac;
	return acceleration;
}

bool Actuator::setTarget(double val)
{

	if (s_Joint)
	{

		double valMax,valMin;
		s_Joint->getMaxMin(valMax,valMin);

		if((val<valMin)||(val>valMax))
			return false;
		target=val;
		targetActive=true;
		return true;
	}
	else
		return false;
}

//bool Actuator::setTargetIntermediate(double _time)
//{
//
//	if (s_Joint)
//	{
//		double _targetInterm = a0 + a1*_time + a2*_time*_time + a3*_time*_time*_time;
//		double valMax,valMin;
//		s_Joint->getMaxMin(valMax,valMin);
//		if((_targetInterm<valMin)||(_targetInterm>valMax))
//		{
//			targetActive=false;
//			return false;
//		}
//		targetIntermediate=_targetInterm;
//		targetActive=true;
//		return true;
//	}
//	else
//		return false;
//}



void Actuator::simulate(double delta_t)
{
	if (s_Joint)
	{
		if(activateCPT)
			simulateCPT(delta_t);
		if(activateTVP)
			simulateTVP(delta_t);
		//simulateSPLINE(delta_t);
	}
	else
		return;
}


void Actuator::linkTo (PositionableEntity *p)
{
	SimpleJoint* simpJoint = dynamic_cast <SimpleJoint*>(p);

	if (simpJoint)
	{
		s_Joint=simpJoint;
		PositionableEntity::LinkTo(p);
	}
}


/********************************************************
	METHODS TO CALCULATE TARGET BY CUBIC POLINOMIAL
*********************************************************/
void Actuator::setCubicPolinomialCoeficients(double path,double targetTime)
{
	a0=s_Joint->getValue();//Current coordiantes
	a1=0.0;
	a2=( 3*(path)/(targetTime*targetTime));
	a3=(-2*(path)/(targetTime*targetTime*targetTime));
}

bool Actuator::setTargetCPT(double _time)
{
	if (_time<0) return false;

	double val=a0 + a1*_time + a2*_time*_time + a3*_time*_time*_time;
	double sp=a1+ 2*a2*_time + 3*a3*_time*_time;

	setSpeed(sp);

	return setTarget(val);
}

void Actuator::simulateCPT(double delta_t)
{
	if(targetActive==false)return;

	double value=s_Joint->getValue();
	double d=target-value;
	double inc=delta_t*speed;

	if(d<0)inc=((-inc<d)?d:(-inc));
	else inc=(inc>d?d:inc);
	if(d<EPS){
		targetActive=false;
		s_Joint->setValue(target);
	}
	else s_Joint->setValue(value+inc);

}

/******************************************************************
	METHODS TO CALCULATE TARGET BY TRAPEZOIDAL VELOCITY PROFILE
*******************************************************************/

bool Actuator::setTargetTVP(double qInit,double q_target,int signMovement,double timeInit,double timeFinal,double ta, double _time)
{
	double val;

	if(getTypeTrajectoryTVP()=="MaximumSpeedAcceleration")
	{
		//Acceleration phase
		if (_time<(timeInit+ta) && _time>=timeInit)
			val=qInit+signMovement*((getAcceleration()*0.5)*square(_time-timeInit));

		//Velocity constant phase
		if (_time>=(timeInit+ta) && _time<=(timeFinal-ta))
			val=qInit+signMovement*(getSpeed()*(_time-timeInit-ta*0.5));

		//Deceleration phase
		if (_time>(timeFinal-ta) && _time<=timeFinal)
			val=q_target-signMovement*((getAcceleration()*0.5)*square(timeFinal-_time));
	}
	else if(getTypeTrajectoryTVP()=="BangBang")
	{
		//Acceleration phase
		if (_time<(timeFinal*0.5) && _time>=timeInit)
			val=qInit+signMovement*((getMaxAcceleration()*0.5)*square(_time-timeInit));

		//Deceleration phase
		if (_time>=(timeFinal*0.5) && _time<=timeFinal)
			val=q_target-signMovement*((getMaxAcceleration()*0.5)*square(timeFinal-_time));
	}

	return setTarget(val);
}


bool Actuator::setTypeTrajectoryTVP(string _type)
{
	typeTrajectoryTVP=string();

	if(_type=="BangBang")
	{
		typeTrajectoryTVP=_type;
		return true;
	}
	if(_type=="MaximumSpeedAcceleration")
	{
		typeTrajectoryTVP=_type;
		return true;
	}

	return false;
}

void Actuator::simulateTVP(double delta_t)
{
	double value=s_Joint->getValue();

	if(targetActive==false)return;
	double d=fabs(target-value);
	if(d<EPS)
		targetActive=false;
	else
		s_Joint->setValue(target);
}



/******************************************************************
	METHODS TO CALCULATE TARGET WITH SPLINE TRAJECTORY
*******************************************************************/
void Actuator::setVelocIntermediates (vector<double> veloc)
{
	double auxsp;
	for(int i=0;i<(int)veloc.size();i++)
	{
		if(veloc[i]<0)auxsp=0.0;
		else
			auxsp=veloc[i]>maxSpeed?maxSpeed:veloc[i];
		velocInter.push_back(auxsp);
	}
}


void  Actuator::simulateSPLINE(double delta_t)
{
	double value=s_Joint->getValue();

	if(targetActive==false)return;
	double d=fabs(target-value);
	if(d<EPS)
		targetActive=false;
	else
		s_Joint->setValue(target);


}

bool  Actuator::setTargetSPLINE(double _time)
{
	if (_time<0) return false;

	double val=a0 + a1*_time + a2*_time*_time + a3*_time*_time*_time;
//	double sp=a1+ 2*a2*_time + 3*a3*_time*_time;

//	setSpeed(sp);

	return setTarget(val);

}


void  Actuator::setCubicPolinomialCoeficientsSPLINE(double strech,double Tk)
{
	if(index>=(int)velocInter.size())return;
	a0=s_Joint->getValue();//Current coordiantes
	a1=velocInter[index];
	a2=( 1/Tk)*((3*(strech)/(Tk))-2*velocInter[index]-velocInter[index+1]);
	a3=(1/(Tk*Tk))*(((-2*(strech))/Tk)+velocInter[index]+velocInter[index+1]);

	index++;
}

}//mr
