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

#ifndef __PUMA_560_SIM_H_
#define __PUMA_560_SIM_H_


#include "../../world/robotsim.h"


#define	INIT_PUMA			0x80	//	0x1000.0000
#define	SHOULDERRIGHT	0x01	//	0x0000.0001
#define ELBOWDOWN		0x02	//	0x0000.0010
#define	WRISTDOWN		0x04	//	0x0000.0100

namespace mr
{

class Puma560Sim : public RobotSim
{
	DECLARE_MR_OBJECT(Puma560Sim)

public:
	//Serializers
	virtual void writeToStream(Stream& stream){}
	virtual void readFromStream(Stream& stream){}

	//Constructor
	Puma560Sim(void);

//Inverse PUMA 560 kinematics rel and abs
	virtual bool inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf=NULL);

//Return robot configuration
	virtual bool getConfigurationOf(const vector<double> &_q, unsigned char &_conf);
	bool configuration(unsigned char _conf, double& _s, double& _e, double& _w);
	bool configuration(double _s, double _e, double _w,unsigned char &_conf);

//simulate
	void simulate(double delta_t);

//Aux. methods
	void setFlash();
	bool getCoordinatesOf(vector<double> &_q);// delete?
	bool Pruebas();//delete

protected:
//Specific inverse methode of Puma 560
	bool PUMA560inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf=0x00);

	void goTo(vector<double> _q);

/***
Ranges coordiantes:
	joints[0]	->	320º	->[8*PI/9 , -8*PI/9]
	joints[1]	->	250º	->[25*PI/36 , -25*PI/36]  125º / -125º
	joints[2]	->	270º	->[3*PI/4 , -3*PI/4]
	joints[3]	->	280º	->[7*PI/9 , -7*PI/9]
	joints[4]	->	200º	->[5*PI/9 , -5*PI/9]
	joints[5]	->	532º	->[133*PI/45 ,  -133*PI/45]
***/

private:
	CylindricalPart *light;
	unsigned char conf;
	vector<double> q_init;
	vector<double> q_target;

	//cinematic simulation atributes
	vector<double> a0,a1,a2,a3;
	double time;
	double targetTime;

	int f;//setFlash()
};

};//end namespace mr

#endif