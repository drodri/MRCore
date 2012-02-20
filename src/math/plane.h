/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Miguel Hernando & Diego Rodríguez-Losada
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
#ifndef __MRCORE__PLANE_H
#define __MRCORE__PLANE_H

#include "../base/object.h"
#include "vector3d.h"
#include <vector>

namespace mr
{
/*!
    \class Plane
	A geometric plane (infinite)
*/
class Plane: public Object
{
	DECLARE_MR_OBJECT(Plane)
public:
	friend ostream& operator<<(ostream& os, const Plane& p);
public:
//constructors
	Plane();
	Plane(Vector3D cg, Vector3D nor);
	Plane(Vector3D p1, Vector3D p2, Vector3D p3);
	virtual ~Plane(void);
//assignation
	Plane& operator = (const Plane& p){origin=p.origin;normal=p.normal;return *this;}
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);

	virtual void drawGL();
	
	double distance(const Vector3D& point) const;
	
	void LSQFit(const vector<Vector3D>& points, double* residual);
//get &set
	void setPosition(const Vector3D &pos){origin=pos;}
	const Vector3D &getPosition(){return origin;}
//Attributes
protected:
	Vector3D origin;
	Vector3D normal;//must be unitary vector
};

}
#endif  //__MRCORE__VECTOR3D_H
