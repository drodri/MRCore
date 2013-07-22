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

#ifndef __MRCORE__COMPOSEDENTITY__H
#define __MRCORE__COMPOSEDENTITY__H

#include <iostream>

#include "../base/object.h"
#include "../base/streamstring.h"
#include "../math/transformation3d.h"
#include "solidentity.h"
#include "entityset.h"

using namespace std;
namespace mr
{
/*!
    \class ComposedEntity
    The class ComposedEntity is a container for phisical objects. All the contained objects have to be 
	positionable entities. It is considered as a solid object, and it is able to clasify the objects
	based on their respective types
*/
class Tcp;
class ComposedEntity: public EntitySet, public SolidEntity
{
	DECLARE_MR_OBJECT(ComposedEntity)

	/**Text output**/
	friend ostream& operator<<(ostream& os, const ComposedEntity& w);

public:
//attributes

//constructors
	ComposedEntity(void);
	virtual ~ComposedEntity(void);

 //asign and copy

	const ComposedEntity &operator=(const ComposedEntity &c)
	{
		StreamString buf;
		(const_cast<ComposedEntity &>(c)).writeToStream(buf);
		readFromStream(buf);
	return *this;
	}
	ComposedEntity(const ComposedEntity &c){
		(*this)=c;
	}
//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	//Draw the composed object  
	void drawGL();
	//returns the bounding box...
	virtual BoundingBox getAbsoluteBoundingBox();
	//computes the boundingbox related to the base
	void computeBoundingBox();
	bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints){
		return EntitySet::segmentIntersection(s,ipoints);
		}
	bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist){
		return EntitySet::rayIntersection(ori, dir, dist);
		}
	Tcp *getTcp(int num=0);
	int getTcpIndex(Tcp *tcp);
	//it can be redefined chaeqs the evolving boxes, and gives the priority to
	//the descomposition in parts
	virtual bool checkCollisionWith(SolidEntity &solid);
	//redefinition of simulate (positionable Entity)
	virtual void simulate(double delta_t);
protected:
	bool linkToBase(PositionableEntity *p);
};

};
#endif  //__MRCORE__COMPOSEDENTITY__H
