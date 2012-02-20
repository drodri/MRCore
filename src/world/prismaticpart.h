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

#ifndef __MRCORE__PRISMATICPART__H
#define __MRCORE__PRISMATICPART__H


#include "primitivesolidentity.h"
#include "../math/face.h"


using namespace std;

namespace mr
{
/*!
    \class PrismaticPart
    PrismaticPart	->	A generic class for implementing a Z axis oriented prismatic part. Geometry is
	described through a polygonal base, and the part height. 
	It is intuitive for fast modelling and it is efficient for ray intersections and collision detection 

	DESIGN INFO:
	
	*/

class PrismaticPart: public PrimitiveSolidEntity
{
	DECLARE_MR_OBJECT(PrismaticPart)
	
	/**Text output**/
	friend ostream& operator<<(ostream& os, const PrismaticPart& p);
protected:
	//invariant polygonal base, and height 
	Face polygonalBase;
	double height;
	//invariant but derived from polygonal base data
	vector<Vector3D> lateralNormals;
	//data computed in absolute coordinates
	Face absPolygonalBase;
	Face absPolygonalTop;
	vector<Vector3D> absLateralNormals;
	Vector3D absHeight;
	
	bool polygonNeedToBeUpdated;
public:
//attributes
	
//constructors
	//Basic constructor
	PrismaticPart(void);

	//Destructor
	virtual ~PrismaticPart(void);

	//operator =
	const PrismaticPart &operator=(const PrismaticPart &v){
		(*((PrimitiveSolidEntity *)this))=v;
		polygonalBase=v.polygonalBase;
		absPolygonalBase=v.absPolygonalBase;
		height=v.height;
		lateralNormals=v.lateralNormals;
		absLateralNormals=v.absLateralNormals;
		absHeight=v.absHeight;
		polygonNeedToBeUpdated=true;
		return (*this);
	}
	PrismaticPart(const PrismaticPart &v){
		(*this)=v;
	}

//overwriten virtual methods
	virtual void locationUpdated();

//methods
	void setPolygonNeedToBeUpdated(){polygonNeedToBeUpdated=true;}
	void updatePolygon();
	void setPolygonalBase(const Face &fin);
	void setPolygonalBase(const vector<Vector2D> &list_of_vertex);
	void setRegularPolygonBase(double radius,int numVertex);
	void setHeight(double _height);
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	void drawGL();
//specific operations due to its primitive condition
	void createWiredModel();
//specific operations that have to be implemented due to its solid entity condition
	bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints);
	bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist); 
	bool rayLateralIntersection(int i,Vector3D &ori, Vector3D &dir, double &dist);

};

};
#endif  //__MRCORE__PRISMATICPART__H
