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

#ifndef __QUADROTOR_H_
#define __QUADROTOR_H_

#include "data/quadrotordata.h"

namespace mr
{

class Quadrotor
{
public:
	virtual bool takeOff()=0;
	virtual bool land()=0;
	
	///\return true if success
	virtual bool getData(QuadrotorData& data)=0;
	/**move uses 4 speeds (in m/sec or rad/sec)
	\param up: Vertical speed, positive means up, negative down
	\param forward: positive forward, negative backwards
	\param lateral: positive rigth, negative left, proportional to the quadrotor roll angle
	\param rot: rotation, positive CW, negative CCW
	\return true if succesful, false otherwise */
	virtual bool move(double up, double forward,double lateral,double rot)=0;


};

}; //end namespace mr

#endif
