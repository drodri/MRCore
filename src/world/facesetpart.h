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

#ifndef __MRCORE__FACESETPART__H
#define __MRCORE__FACESETPART__H


#include "primitivesolidentity.h"
#include "../math/face.h"
#include <vector>

using namespace std;

namespace mr
{
/*!
    \class FaceSetPart
    FaceSetPart	->	A generic class for implementing a set of walls and floors referred 
	to the part reference system. Is just a set 
	of face objects. Not efficient, because it has to recalculate the data of each face
	each time the object is being moved, and the evolving sphere and box.
	It is useful because, any part can be modeled at the end as a face set

	DESIGN INFO:
	Initially all the atributes are consecuence of the derived data... so it is not
	necesary to serialize its information
	*/

class FaceSetPart: public PrimitiveSolidEntity
{
	DECLARE_MR_OBJECT(FaceSetPart)
	
	/**Text output**/
	friend ostream& operator<<(ostream& os, const FaceSetPart& p);
protected:
	//faces in relative coordinates
	vector<Face> faces;
	//faces computed in absolute coordinates
	vector<Face> absolutefaces;
public:
//attributes
	
//constructors
	//Basic constructor
	FaceSetPart(void);

	//Destructor
	virtual ~FaceSetPart(void);

	//operator =
	const FaceSetPart &operator=(const FaceSetPart &v){
		(*((PrimitiveSolidEntity *)this))=v;
		faces=v.faces;
		absolutefaces=v.faces;
		return (*this);
	}
	FaceSetPart(const FaceSetPart &v){
		(*this)=v;
	}

//overwrittn virtual methods
	virtual void locationUpdated();

//methods
	//serialization
	void addFace(Face face);
	void operator+=(const Face &face){addFace(face);}
//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	void drawGL();
//specific operations that have to be implemented due to its solid entity condition
	bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints);
	bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist); 
//specific operations due to its primitive condition
	void createWiredModel();
//specific realization of collision detection
	//virtual bool checkCollisionWith(SolidEntity &solid);
};

};
#endif  //__MRCORE__FACESETPART__H
