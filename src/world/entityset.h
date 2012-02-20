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

#ifndef __MRCORE__ENTITYSET__H
#define __MRCORE__ENTITYSET__H

#include <iostream>

#include "../base/globject.h"
#include "../math/transformation3d.h"
#include "solidentity.h"

using namespace std;
namespace mr
{
/*!
    \class Entity Set
     A set of positionable entities. Its a container, but it is at the same
	 time the main hierarchical base class for the simulation. Everything have
	 to be included in an entity set, and the links are only allowed between elements
	 of the same entity set. links among reference systems of two diferent entity sets are
	 only allowed if the spring is the whole entity set, and the father has a external plug 
	 for external references (TCP)
	 This way of working is important for classifying objects and being able to iterate across all
	 elements in an orderly manner.
	 It has dependencies to all the basic abstract classes like ^Positionable, Solid, Joints, etc..
*/

class EntitySet:virtual public GLObject
{
	DECLARE_MR_OBJECT(EntitySet)
protected:	
	vector<PositionableEntity*> objects;

public:
//attributes

//constructors
	EntitySet(void);
	virtual ~EntitySet(void);

//operators
	inline PositionableEntity * operator [] (int index){
		return objects[index];}
	inline bool operator+=(PositionableEntity *p){
		return addObject(p);
		}
//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	//number of objects included
	int getNumObjects(){return (int)objects.size();}
	//method for adding an object to the EntitySet
	bool addObject(PositionableEntity * p);
	//verifies if an object belong to the entity set
	bool hasObject(PositionableEntity *p){
		for(int i=0;i<(int)(objects.size());i++)
			if(objects[i]==p)return true;
		return false;
	}
	//Delete all entities. Carefull, because this function calls to the
	//destructor of the object
	void destroyContent();

	//Delete only the object at the index position of the entities vector
	//Calls the destructor of the object
	void erase(int index);
	//removes the object but does not call the destructor
	void remove(PositionableEntity *p);
	//get the index of one object stored in the set. -1 if it is not in this set
	int getIndexOf(PositionableEntity *ref){
		for(int i=0;i<(int)(objects.size());i++)
			if(ref==objects[i])return i;
		return -1;
	}
	//Draw the set 
	void drawGL();

	PositionableEntity *getObjectByName(string _name);
	//aplies to each solid entity their methods for basic intersections
	virtual bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints);
	virtual bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist);

	//filters: getting a subset of object with some criteria. Just for optimizing some massive operations like
	//ray tracing

	//Retrieves a list of primitives whose bounding boxes are cut by a plane defined by a ref system
	void getPrimitivesBoundingBoxesCutByPlane(const Transformation3D &t, vector<SolidEntity *> &subset, double range=-1);
protected:
	//virtual method for linking objects to the base if the entityset is an object
	virtual bool linkToBase(PositionableEntity *p){return true;}
};

};
#endif  //__MRCORE__ENTITYSET__H
