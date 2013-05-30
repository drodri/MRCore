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

#ifndef __MRCORE__POSITIONABLEENTITY__H
#define __MRCORE__POSITIONABLEENTITY__H

#include <iostream>
#include <string.h>
#include "../base/globject.h"
#include "referencesystem.h"
#include "../base/xmlaux.h"
#include "../base/xml.h"



using namespace std;

namespace mr
{
/*!
    \class PositionableEntity
    PositionableEntity	->	Any object, whether physical or not, which has
		the attribute a position and an orientation.
*/
class World;
class EntitySet;
class ComposedEntity;
class PositionableEntity: virtual public GLObject
{
	DECLARE_MR_OBJECT(PositionableEntity)

	friend class EntitySet;
	friend class ComposedEntity;
	/**Text output**/
	friend ostream& operator<<(ostream& os, const PositionableEntity& p);
	bool drawRefSystem;
	bool drawSelected;
protected:
	EntitySet *owner;
	virtual void setOwner(EntitySet *w);
	
	//attributes
	ReferenceSystem location;	
	string name;
public:
	void setDrawReferenceSystem(bool v=true){drawRefSystem=v;}
	bool getDrawReferenceSystem(){return drawRefSystem;}
	void setSelected(bool v=true){drawSelected=v;}
	bool setSelected(){return drawSelected;}
	void setName(string _name){name=_name;}
	const string &getName(){return name;} 
	EntitySet *getOwner(){return owner;}
//constructors
	//Basic constructor
	PositionableEntity(void);

	//Constructor by name and one relative vector: relative position
	PositionableEntity(string n,Vector3D pos_rel);
	
	//Destructor
	virtual ~PositionableEntity(void);

	//operator =
	const PositionableEntity & operator =(const PositionableEntity &pe);
	//copy constructor
	PositionableEntity (const PositionableEntity &pe){
		(*this)=pe;
		drawRefSystem=pe.drawRefSystem;
		location.setOwner(this);
	}
	PositionableEntity *getLinkedTo(){
		if(location.getBase())return location.getBase()->getOwner();
		else return 0;
	}
//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	//Draw positional entity
	 void drawGL();
	//virtual function for time simulation. time inteval in seconds
	 virtual void simulate(double delta_t){}//do nothing
	//Associate an entity with other
	void LinkTo(PositionableEntity *pe);
	//returns the location that would be used for objects referred to this positionable object
	//this method have to be overwritten by joints and tcp´s
	virtual ReferenceSystem *getReferenciableLocation();

	//it is important to report the changes if somethig have to be recomputed
	virtual void locationUpdated(){};

	Transformation3D getAbsoluteT3D(){
		return location.getAbsoluteT3D();
	}
	//set the absolute transformation, and computes the correspondent relative
	void setAbsoluteT3D(const Transformation3D &m);

	Transformation3D getRelativeT3D(){
		return location.getRelativeT3D();
	}
	void setRelativeT3D(const Transformation3D &m){
			location.setRelativeT3D(m);
			locationUpdated();
	}

//Modify relative positions/orientations
	//Set/Get relative position by vector
	void setRelativePosition(Vector3D rp){
		location.setRelativePosition(rp);
		locationUpdated();
	}
	Vector3D getRelativePosition(){
		return location.getRelativeT3D().position;
	}

	//Set relative orientation
	void setRelativeOrientation(double rox, double roy, double roz){
		location.setRelativeOrientation(rox,roy,roz);
		locationUpdated();
	}
	void setRelativeOrientation(Axis ax, double rot){
		location.setRelativeOrientation(ax,rot);
		locationUpdated();
	}
	void getRelativeOrientation(double &rox, double &roy, double &roz)
	{
		((location.getRelativeT3D()).orientation).getRPY(rox,roy,roz);
	}
	World *getWorld();
};

};
#endif  //__MRCORE__POSITIONABLEENTITY__H
