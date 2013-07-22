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

#ifndef __ASEA_IRB_2000_SIM_H_
#define __ASEA_IRB_2000_SIM_H_


#include "../world/robotsim.h"




namespace mr
{
const int ASEAIRB_INIT=0x80;
const int ASEAIRB_FACEBACK=0x01;
const int ASEAIRB_ELBOWDOWN=0x02;
const int ASEAIRB_WRISTDOWN=0x04;
class AseaIRB2000Sim : public RobotSim
{
	DECLARE_MR_OBJECT(AseaIRB2000Sim)

public:
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	//Constructor
	AseaIRB2000Sim(void);

//Inverse ASEA IRB 2000 kinematics rel and abs
	virtual bool inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf=NULL);

//Return robot configuration
	virtual bool getConfigurationOf(const vector<double> &_q, unsigned char &_conf);

//Aux. methods
private:
	bool configuration(unsigned char _conf, double& _f, double& _e, double& _w);
	bool configuration(double _f, double _e, double _w,unsigned char &_conf);
	void ASEAIRB2000Mechanism();

//simulation methods specific
public:
	void simulate(double delta_t);



protected:
//Specific inverse methode of Asea IRB 2000
	bool ASEAIRB2000inverseKinematics(Transformation3D t06, vector<double> &_q, unsigned char _conf=0x00);

/***
Ranges coordiantes:
	joints[0]	->	360º	->[PI , -PI]
	joints[1]	->	200º	->[5*PI/9,-5*PI/9]
	joints[2]	->	120º	->[PI/3 , -PI/3]
	joints[3]	->	400º	->[10*PI/9 , -10*PI/9]
	joints[4]	->	240º	->[2*PI/3, -2*PI/3]
	joints[5]	->	400º	->[10*PI/9 , -10*PI/9]
***/

private:
	SimpleJoint *q_bar[3];

};

};//end namespace mr

#endif





