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

Actuator::Actuator(SimpleJoint* _sjoint,double _maxSpeed,double _speed,double _target,bool _targetOn)
{
	s_Joint=_sjoint;
	maxSpeed=_maxSpeed;
	speed=_speed;
	target=_target;
	targetActive=_targetOn;
}


Actuator::~Actuator(void)
{

}

void  Actuator::setSimulationParameters(double _maxSpeed, double _speed)
{
	if(_maxSpeed<0)
		return;
	maxSpeed=_maxSpeed;
	if(_speed<=0)
		speed=_maxSpeed;

	speed=speed>maxSpeed?maxSpeed:speed;
}


bool Actuator::setTarget(double val)
{
	double valMax,valMin;
	s_Joint->getMaxMin(valMax,valMin);
	if((val<valMin)||(val>valMax))
		return false;
	target=val;
	targetActive=true;
	return true;
}



void Actuator::simulate(double delta_t)
{
	double value=s_Joint->getValue();

	if(targetActive==false)return;
	double d=target-value;
	double inc=delta_t*speed;
	if(d<0)inc=((-inc<d)?d:(-inc));
	else inc=(inc>d?d:inc);
	if(fabs(inc)<EPS){
		targetActive=false;
		s_Joint->setValue(target);
	}
	else s_Joint->setValue(value+inc);
}






}//mr
