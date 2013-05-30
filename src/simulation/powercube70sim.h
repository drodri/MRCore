/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  the MRCore group
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

#ifndef __POWERCUBE70_SIM_H_
#define __POWERCUBE70_SIM_H_


#include "hw/powercube70.h"
#include "base/globject.h"
#include "../world/composedentity.h"
#include "../world/simplejoint.h"
namespace mr
{
class PowerCube70Sim : public PowerCube70, public ComposedEntity
{
	DECLARE_MR_OBJECT(PowerCube70Sim)

public:
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	//Constructor
	PowerCube70Sim();


	//expose the saving to log file service
	bool  saveDataTo(DataLogOut* log,string name)
	{
		return PowerCube70::saveDataTo(log,name);
	}


	virtual int Home(){return 1;}
	virtual int SetAccel(float acc_pan,float acc_tilt){return 1;}
	virtual int SetSpeed(float vel_pan,float vel_tilt){return 1;}
	virtual int Move(float pos_pan,float pos_tilt);
	virtual int GetPos(float& pan_angle,float& tilt_angle);
	//set joint positions directly : no emulation of joint speeds
	void setPos(double pan, double tilt);
	virtual void simulate(double delta_t);//time inteval in seconds
	virtual void drawGL();

protected:
	SimpleJoint *panJoint;
	SimpleJoint *tiltJoint;
	Tcp *tcp;

};

}; //end namespace mr

#endif
