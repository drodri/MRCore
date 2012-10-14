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

#ifndef __MRCORE__CYLINDRICALPART__H
#define __MRCORE__CYLINDRICALPART__H


#include "prismaticpart.h"



using namespace std;

namespace mr
{
/*!
    \class CylindricalPart
    CylindricalPart	->	a very simple class. For everything except for painting,
	it is  a prismaticpart

	DESIGN INFO: latter modifications are allowed. this class will softten the lateral
	faces...
	
	*/

class CylindricalPart: public PrismaticPart
{
	DECLARE_MR_OBJECT(CylindricalPart)
	
	/**Text output**/
	friend ostream& operator<<(ostream& os, const CylindricalPart& p);

public:
//attributes
	
//constructors
	//Basic constructor
	CylindricalPart(double _height=1, double _radius=1, int numVertex=12){
		setHeightAndRadius(_height,_radius,numVertex);
		radius=_radius;
	}

	//Destructor
	virtual ~CylindricalPart(void);

	//operator =
	const CylindricalPart &operator=(const CylindricalPart &v){
		(*((PrismaticPart *)this))=v;
		return (*this);
	}
	CylindricalPart(const CylindricalPart &v){
		(*this)=v;
	}



//methods
	void setHeightAndRadius(double _height, double _radius, int numVertex=12){
		setHeight(_height);
		setRegularPolygonBase(_radius,numVertex);
		radius=_radius;
	}
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	double getRadius(){return radius;}
	void drawGL();

private:
	double radius;

};

};
#endif  //__MRCORE__PRISMATICPART__H
