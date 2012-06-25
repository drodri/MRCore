/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
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

#ifndef __MRCORE__ODOMETRY_H
#define __MRCORE__ODOMETRY_H

#include "data.h"
#include "../math/vector2d.h"
#include "../math/angle.h"
#include "../math/transformation3d.h"

#include <iostream>
#include <vector>

using namespace std;

namespace mr{


class Odometry :public Data
{
	///Text stream serializers for files and cout, cerr (std)
	friend ostream& operator << (ostream& os,const Odometry & odom);
	
	DECLARE_MR_OBJECT(Odometry )

public:
	Odometry ();
	virtual ~Odometry (){}
	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void drawGL();//<OpenGL draw
	
	Transformation3D getIncrement(const Odometry& lastOdometry,float noise=0);
//protected:
	Transformation3D pose;//Just for test purposes
};


}



#endif
