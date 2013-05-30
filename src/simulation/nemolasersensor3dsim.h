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

#ifndef __NEMOLASERSENSOR3DSIM_H_
#define __NEMOLASERSENSOR3DSIM_H_


#include "lasersensor3dsim.h"
#include "lms200sim.h"
#include "powercube70sim.h"

namespace mr
{
class NemoLaserSensor3DSim :public LaserSensor3DSim
{
	DECLARE_MR_OBJECT(NemoLaserSensor3DSim)
	PowerCube70Sim *arm;
	LMS200Sim *laser;
	Mutex m;
public:
	NemoLaserSensor3DSim();
	LaserSensor *getLaserSensor(){return laser;}
	PowerCube70	*getPowerCube70(){return arm;}
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	virtual void drawGL();

	//laserSensor Methods
	virtual void updateSensorData();
	virtual bool getData(LaserData3D& d){
		m.Lock();
		d=data;
		m.Unlock();
		return true;
	}
	//using laserSensorSim for displaying other laserSensor data
	virtual void setData(const LaserData3D& d){
		m.Lock();
		data=d;
		m.Unlock();
	}
};

}; //end namespace mr

#endif //__NEMOLASERSENSOR3DSIM_H_
