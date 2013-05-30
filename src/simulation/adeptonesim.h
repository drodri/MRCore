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

#ifndef __SCARA_ADEPT_ONE_SIM_H_
#define __SCARA_ADEPT_ONE_SIM_H_




#include "../world/robotsim.h"

#define ELBOW_LEFT 0x81 //0x1000-0001	-	-1
#define ELBOW_RIGHT 0x80//0x1000-0000	-	1

namespace mr
{

class AdeptOneSim : public RobotSim
{
	DECLARE_MR_OBJECT(AdeptOneSim)

public:
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	//Constructor
	AdeptOneSim(void);

//Return elbow configuration
	
	virtual bool getConfigurationOf(const vector<double> &_q, unsigned char &conf);


//Forward and inverse SCARA_ADEPT_ONE kinematics Relative
	virtual bool inverseKinematics(Transformation3D t, vector<double> &_q, unsigned char conf=0);//Not move robot



protected:
//Specific inverse methode of SCARA_ADEPT_ONE
	bool ADEPTONEinverseKinematics(double yaw,Vector3D p,vector<double> &_q,unsigned char conf=0);

};

};//end namespace mr

#endif