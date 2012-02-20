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

#ifndef __MRCORE__LASERDATA_3D_H
#define __MRCORE__LASERDATA_3D_H

#include "data.h"
#include "../math/vector2d.h"
#include "../math/angle.h"
#include "laserdata.h"
#include "../math/transformation3d.h"
#include <iostream>
#include <vector>

using namespace std;

namespace mr{



class LaserData3D :public Data
{
	DECLARE_MR_OBJECT(LaserData3D)

public:
	LaserData3D();
	virtual ~LaserData3D(){}

	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);

	/** Drawing mode. 0=points, 1=rays, 2=contour **/
	int drawGLMode;
	//int drawGLColor;
	virtual void drawGL();//<OpenGL draw

	void set(const vector<LaserData>& las,const vector<Transformation3D>& tr)
	{
		if(las.size()!=tr.size())return;
		laserdata=las;
		transf=tr;
	}
		
protected:
	//the data basic parametrization, compulsory. The angle evolves ccw (positive direction)
	vector<LaserData> laserdata;
	vector<Transformation3D> transf;

};


}



#endif
