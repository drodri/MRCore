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

#ifndef __POWERCUBE70_H_
#define __POWERCUBE70_H_


#include "base/datasource.h"

namespace mr
{

class PowerCube70 :public DataSource
{
public:
	//virtual bool getData(LaserData& data)=0;
	virtual int Home()=0;
	virtual int SetAccel(float acc_pan,float acc_tilt)=0;
	virtual int SetSpeed(float vel_pan,float vel_tilt)=0;
	virtual int Move(float pos_pan,float pos_tilt)=0;
	virtual int GetPos(float& pan_angle,float& tilt_angle)=0;

};

}; //end namespace mr

#endif
