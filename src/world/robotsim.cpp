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
#include <iostream>

namespace mr
{

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
	int i;
	if(_q.size()!=joints.size())return false;
	if(!checkJointValues(_q))return false; //limits checking
	for(i=0;i<(int)joints.size();i++)joints[i]->setValue(_q[i]);
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
void RobotSim::simulate(double delta_t)//time interval in seconds
{
	//de momento.. simulación en base a lo que hace cada componente
	ComposedEntity::simulate(delta_t);
}
};//Namespace mr