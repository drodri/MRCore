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

#ifndef __MRCORE__PRIMITIVESOLIDENTITY__H
#define __MRCORE__PRIMITIVESOLIDENTITY__H


#include "solidentity.h"


using namespace std;

namespace mr
{
/*!
    \class PrimitiveSolidEntity
    PrimitiveSolidEntity	->	An abstract class for the basic Solid components.
	Its responsability is to include the interfaces needed for basic part to basic 
	part collision detection.
	It includes the posibility to obtain a generic wired representation of any component
	It allows the exact collision detection between two PrimitiveSolidEntityes
	
	*/

class PrimitiveSolidEntity: public SolidEntity
{
protected:

	vector<Segment3D> wiredModel;
	vector<Segment3D> absoluteWiredModel;
	bool wiredModelNeedToBeUpdated;
public:
//attributes
	
//constructors
	//Basic constructor
	PrimitiveSolidEntity(void);

	//Destructor
	virtual ~PrimitiveSolidEntity(void);

	//operator =
	const PrimitiveSolidEntity &operator=(const PrimitiveSolidEntity &s){
		*((SolidEntity *)this)=s;

		return (*this);
	}
	//copy constructor
	PrimitiveSolidEntity(const PrimitiveSolidEntity &s){
		(*this)=s;
	}
//methods
	//serialization
	virtual void writeToStream(Stream& stream){SolidEntity::writeToStream(stream);};
	virtual void readFromStream(Stream& stream){SolidEntity::readFromStream(stream);};
	virtual void writeToXML(XMLElement* parent){SolidEntity::writeToXML(parent);}
	virtual void readFromXML(XMLElement* parent){SolidEntity::readFromXML(parent);}

	void locationUpdated();

//virtual methods that have to be implemented by each derived primitive solid entity
protected:
	void setWiredModelNeedToBeUpdated(){wiredModelNeedToBeUpdated=true;}
	const vector<Segment3D> &getWiredModel();
	//just to force to have a wired model in the primitives
	virtual void createWiredModel() = 0;
	//it is defined as a default function that makes use of the wired model
	virtual bool checkWiredModelCollisionWith(PrimitiveSolidEntity &sol);
	virtual void drawGL();
public:
	//important: it can be redefined but be carefull. 
	//it works as follows: 1.- Cheq boxes
	//2.- cheq if is primitive vs primitive, in such case, use wired models
	//if not: change order of collision detection
	//it is posible (and sometimes recommended to overwrite it... but always follow the mentioned structure at least
	virtual bool checkCollisionWith(SolidEntity &solid);
	//virtual bool cheqEdgesCollisionWith(const PrimitiveSolidEntity &sol) const =0;

};

};
#endif  //__MRCORE__PRIMITIVESOLIDENTITY__H
