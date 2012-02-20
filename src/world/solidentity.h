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

#ifndef __MRCORE__SOLIDENTITY__H
#define __MRCORE__SOLIDENTITY__H


#include "positionableentity.h"
#include "../math/boundingbox.h"
#include "material.h"

using namespace std;

namespace mr
{
/*!
    \class SolidEntity
    SolidEntity	->	An abstract class for Solid Parts.
	Its responsability is to include all the information needed for collision detection
	and intersection cheking. Guives a common interface for this operations, through pure
	polimorfic methods. Mainly it includes a evolving-axis-oriented box, and a evolving sphere 
	
	DESIGN INFO:
	Initially all the atributes are consecuence of the derived data... so it is not
	necesary to serialize its information
	*/

class SolidEntity: public PositionableEntity
{
	friend class ComposedEntity;
	
	/**Text output**/
	friend ostream& operator<<(ostream& os, const SolidEntity& p);
protected:
	//box referred to the intrinsic ref system
	BoundingBox box;
	//box updated to the absolute ref system
	BoundingBox absoluteBox;
	//flag that indicates if the box have to be updated due to movements
	bool boxNeedToBeUpdated;
	bool drawBox;
	Material material;
	//special atribute to allow segment and ray intersection
	bool intersectable; //true as default
public:
//attributes
	
//constructors
	//Basic constructor
	SolidEntity(void);

	//Destructor
	virtual ~SolidEntity(void);
	//serialization
	void writeToStream(Stream& stream)
	{
		PositionableEntity::writeToStream(stream);
		box.writeToStream(stream);
		material.writeToStream(stream);
		stream<<intersectable;
	}
	void readFromStream(Stream& stream)
	{
		PositionableEntity::readFromStream(stream);
		box.readFromStream(stream);
		material.readFromStream(stream);
		stream>>intersectable;
		setBoxNeedToBeUpdated();
	}
	//operator =
	const SolidEntity &operator=(const SolidEntity &s){
		*((PositionableEntity *)this)=s;
		//proper atributes
		box=s.box;
		boxNeedToBeUpdated=true;
		drawBox=s.drawBox;
		material=s.material;
		intersectable=s.intersectable;
		return (*this);
	}
	//copy constructor
	SolidEntity(const SolidEntity &s){
		(*this)=s;
	}
//methods
	void setColor(double r, double g, double b){material.setColor(r,g,b);}
	void getColor(double &r,double &g, double &b){
		material.getColor(r,g,b);
	}
	virtual void locationUpdated();
	void setBoxNeedToBeUpdated(){boxNeedToBeUpdated=true;}
	virtual BoundingBox getAbsoluteBoundingBox(){
		if(boxNeedToBeUpdated){absoluteBox=getAbsoluteT3D()*box;
			boxNeedToBeUpdated=false;
		}
		return absoluteBox;
	}
	void setDrawBox(bool v=true){drawBox=v;}
	bool getDrawBox(){return drawBox;}
	void setIntersectable(bool v=true){intersectable=v;}
	void drawGL();
	bool isIntersectable(){return intersectable;}
//virtual methods that have to be implemented by each derived solid entity
	//returns true if the segment intersects. Si el vector no es nulo, entonces
	//computa todas las intersecciones y las guarda en ipoints
	virtual bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints)=0;
	virtual bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)=0;
	//return true if there is collision between this object and the parameter

	//it have to be redefined chaeqs the evolving boxes, and gives the priority to
	//the descomposition in parts
	virtual bool checkCollisionWith(SolidEntity &solid)=0;
	bool checkBoundingBoxes(SolidEntity &solid);
	
};

};
#endif  //__MRCORE__SOLIDENTITY__H
