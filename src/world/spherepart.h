/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
 * 
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

#ifndef __MRCORE__SPHEREPART__H
#define __MRCORE__SPHEREPART__H

//#include "mrcore.h"

#include "primitivesolidentity.h"
#include "../math/face.h"

using namespace std;

namespace mr
{
/*!
    \class SpherePart
    SpherePart	->

	DESIGN INFO:
	
	*/

class SpherePart: public PrimitiveSolidEntity
{
	DECLARE_MR_OBJECT(SpherePart)
	
	/**Text output**/
	friend ostream& operator<<(ostream& os, const SpherePart& s);
protected:

	double radius;
	int meridian;//Meridians	- default 12
	int parallel;//Parallels	- default 9
	
	bool sphereNeedToBeUpdated;

public:
//attributes
	
//constructors
	//Basic constructor
	SpherePart(double _radius=1.0, int _meridian=12, int _parallel=9)
	{
		setNumVertex(_meridian,_parallel);
		setRadius(_radius);
	}

	//Destructor
	virtual ~SpherePart(void);

	//operator =
	const SpherePart &operator=(const SpherePart &v){
		(*((PrimitiveSolidEntity *)this))=v;
		radius=v.radius;
		meridian=v.meridian;
		parallel=v.parallel;
		sphereNeedToBeUpdated=true;
		return (*this);
	}
	SpherePart(const SpherePart &v){
		(*this)=v;
	}

//overwriten virtual methods
	virtual void locationUpdated();

//methods

/***
Not necessary updateSphere() because it is only necessary update position center, and that are already
dealing with primitive classes.
***/
	//void updateSphere();

	void setNumVertex(int meridian, int parallel);
	void setRadius(double _radius);
	double getRadius(){return radius;}
//specific operations due to its primitive condition
	void createWiredModel();

	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	void drawGL();

//specific operations that have to be implemented due to its solid entity condition
	bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints);
	bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist); 
	

};

};
#endif  //__MRCORE__SPHEREPART__H