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

#ifndef __PersonSim_H_
#define __PersonSim_H_

#include "world/composedentity.h"
#include "math/transformation3d.h"

namespace mr
{
class PersonSim: public ComposedEntity
{
	DECLARE_MR_OBJECT(PersonSim)

public:
	PersonSim(float height=1.75f);
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void simulate(double delta_t);
	virtual void drawGL();

protected:
	bool computeGroundedLocation(Transformation3D &p);
	float speed,rotSpeed; 
	float radius, height;
//	Transformation3D pose;
	
};

}; //end namespace mr

#endif
