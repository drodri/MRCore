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

#ifndef __MRCORE__WORLD__H
#define __MRCORE__WORLD__H

#include <iostream>

#include "../base/object.h"

#include "../math/transformation3d.h"
#include "positionableentity.h"
#include "entityset.h"
using namespace std;
namespace mr
{
/*!
    \class World
    The class world is a container for phisical objects. All the contained objects have to be 
	positionable entities.
*/

class World: public EntitySet
{
	DECLARE_MR_OBJECT(World)

	/**Text output**/
	friend ostream& operator<<(ostream& os, const World& w);

public:
//attributes

//constructors
	World(void);
	virtual ~World(void);


//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);
	//Draw the world 
	//void drawGL();
	//Collision checking
	bool checkCollisionWith(SolidEntity &solid);
	//retrieves/computes the world bounding box
	BoundingBox getBoundingBox();
	//Simulates simulable objects contained into the set
	void simulate(double t);



};

};
#endif  //__MRCORE__WORLD__H
