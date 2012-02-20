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

#ifndef __MRCORE__JOINT__H
#define __MRCORE__JOINT__H


#include "positionableentity.h"



using namespace std;

namespace mr
{
/*!
    \class Joint
    Joint	->	An abstract class for joints.
*/

class Joint: public PositionableEntity
{
	

	/**Text output**/
	friend ostream& operator<<(ostream& os, const Joint& p);
protected:
	//this transformation should be modified by the joint parameters
	ReferenceSystem outputLocation; 
public:
//attributes
	
//constructors
	//Basic constructor
	Joint(void);

	//Destructor
	virtual ~Joint(void);

//methods
	//serialization
	virtual void writeToStream(Stream& stream){PositionableEntity::writeToStream(stream);};
	virtual void readFromStream(Stream& stream){PositionableEntity::readFromStream(stream);};
	//returns the location that would be used for objects referred to this positionable object
	//this method have to be overwritten by joints and tcp´s
	virtual ReferenceSystem *getReferenciableLocation(){return &outputLocation;}
};

};
#endif  //__MRCORE__JOINT__H
