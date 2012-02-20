/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -------------
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
#ifndef __MRCORE__TRANSFORMATION3D_H
#define __MRCORE__TRANSFORMATION3D_H

#include <iostream>
#include "vector3d.h"
#include "orientationmatrix.h"
#include "../base/globject.h"
using namespace std;

namespace mr
{
/*!
    \class Transformation3d
    \brief Transformation3d - basic mr object for locating objects in 6d space: 
	position+orientation. The localization data is stored in two different atributes: position
	and orientation (a vector3d and an orientation matrix)
	The class includes all the stuff required for kinematic chains, like transformation compositions
	or relative transformations.
	By default the orientation angles are pitch roll and yaw.
*/
class Transformation3D{

	/**Text output**/
	friend ostream& operator<<(ostream& os, const Transformation3D& p);
protected:
	
public:
//attributes. 
	//because the two atributes are internally protected... there it is safe 
	//to declare them as public
	OrientationMatrix orientation;
	Vector3D position;

//constructors
	Transformation3D(double xi=0, double yi=0, double zi=0):position(xi,yi,zi){}
	Transformation3D(const Vector3D &pos):position(pos){}
	Transformation3D(double xi, double yi, double zi,double roll, double pitch, double yaw)
		:position(xi,yi,zi),orientation(roll, pitch, yaw){}
	Transformation3D(double xi, double yi, double zi,Axis axis, double angle)
		:position(xi,yi,zi),orientation(axis,angle){}
	

//operators

	//!Transformation concatenation
	inline Transformation3D operator * (const Transformation3D &t)
	{
		Transformation3D aux(*this);
		aux.position=orientation*(t.position)+position;
		aux.orientation*=t.orientation;
		return (aux);
	}
	
	inline Vector3D operator * (const Vector3D &v)
	{
		Vector3D aux;
		aux = orientation*v + position;	
		return (aux);
		
	}
//methods
	//!obtains the vector v relative to the transformation
	inline Vector3D obtainRelativeCoordinates (const Vector3D &v)
	{
		Vector3D aux;
		aux = orientation.transposed()*(v-position);	
		return (aux);
		
	}

	inline Vector3D getVectorU(){return orientation.getVectorU();}
	inline Vector3D getVectorV(){return orientation.getVectorV();}
	inline Vector3D getVectorW(){return orientation.getVectorW();}
	Transformation3D inverted();
	void transformGL();
	void drawGL();

	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);
};

}
#endif  //__MRCORE__TRANSFORMATION3D_H
