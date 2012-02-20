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
#ifndef __MRCORE__BOUNDINGBOX_H
#define __MRCORE__BOUNDINGBOX_H

#include "../base/object.h"
#include "vector3d.h"
#include "transformation3d.h"
#include "segment3d.h"
#include "face.h"
namespace mr
{
/*!
    \class BoundingBox
	A axis oriented box with the aditional functionallity of computing the evolving box of a set of
	points.
*/
class BoundingBox: public Object
{
	DECLARE_MR_OBJECT(BoundingBox)
public:
	friend ostream& operator<<(ostream& os, const BoundingBox& p);
//Attributes
protected:
	Vector3D minVertex;
	Vector3D maxVertex;
	bool validData;	
public:
//constructors
	BoundingBox();
	BoundingBox(Vector3D _minVertex, Vector3D _maxVertex);
	virtual ~BoundingBox(void);
//assignation
	BoundingBox& operator = (const BoundingBox& p){minVertex=p.minVertex;maxVertex=p.maxVertex;validData=p.validData;return *this;}
//copy constructor
	BoundingBox(const BoundingBox &p){(*this)=p;}
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void drawGL();
//methods
	void reset(){validData=false;}
	void includePoint(Vector3D p);
	void includeBox(BoundingBox b);
	Vector3D getMinVertex(){return minVertex;}
	Vector3D getMaxVertex(){return maxVertex;}
	//segment
	bool checkMinMax(const Segment3D &s);
	//point
	bool checkMinMax(const Vector3D &p);
	//ray
	bool checkMinMax(Vector3D &o, Vector3D &u);
	//box
	bool checkMinMax(const BoundingBox &b) const;
	//face
	bool checkMinMax(const Face &f) ;
	//plane defined T (n=z_axis)
	bool checkMinMax(Transformation3D t, double range=-1);
//operator
	//special operator that obtains the evolving oriented box defined relative to the in T reference system
	friend BoundingBox operator*(Transformation3D T, BoundingBox& b);
	//special operator that obtains a evolving oriented box but now relative to the  T reference system
	friend BoundingBox operator/( BoundingBox& b,Transformation3D T);

};

}
#endif  //__MRCORE__BOUNDINGBOX_H
