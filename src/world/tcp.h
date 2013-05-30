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

#ifndef __MRCORE__TCP__H
#define __MRCORE__TCP__H


#include "positionableentity.h"



using namespace std;

namespace mr
{
/*!
    \class tcp
    tcp	->	is just a PositionableEntity. It is needed for definition of plug externally referenciables
	by objects that ton belong to the same entityset. The type is checked wenever this kind of relation
	is required
*/

class Tcp: public PositionableEntity
{
	DECLARE_MR_OBJECT(Tcp)


	/**Text output**/
	friend ostream& operator<<(ostream& os, const Tcp& p);
	
protected:
	
public:


//constructors
	//Basic constructor
	Tcp(void){}

	
	//Destructor
	virtual ~Tcp(void){};

	//operator =
	const Tcp & operator =(const Tcp &pe){
		PositionableEntity::operator =(pe);	
	}
	//copy constructor
	Tcp (const Tcp &pe){
		(*this)=pe;
	}
//methods
	//serialization
	virtual void writeToStream(Stream& stream){PositionableEntity::writeToStream(stream);}
	virtual void readFromStream(Stream& stream){PositionableEntity::readFromStream(stream);}
	virtual void writeToXML(XMLElement* parent){PositionableEntity::writeToXML(parent);}
	virtual void readFromXML(XMLElement* parent){PositionableEntity::readFromXML(parent);}


};

};
#endif  //__MRCORE__POSITIONABLEENTITY__H
