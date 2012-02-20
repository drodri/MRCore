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

#ifndef __MRCORE__CameraSIM__H
#define __MRCORE__CameraSIM__H

#include "hw/camera.h"
#include "../world/composedentity.h"
#include <vector>
#include "../data/laserdata.h"
//#include "../math/segment3d.h"
#include "../world/world.h"
#include "../system/mutex.h"


using namespace std;

namespace mr
{
/*!
    \class CameraSim
    CameraSim	->	A generic class for a positionable laser scanner sensor.
	Its reference is as follows: X axis is the 0 rad laser beam, while PI/2 is the Y axis. Z is
	oriented perpendicular tu the laser beam plane.
*/

class CameraSim : public Camera, public ComposedEntity
{
	/**Text output**/
	friend ostream& operator<<(ostream& os, const CameraSim& p);
protected:
	Image image; 
	
	//virtual methos executed each time the laser is moved
	void locationUpdated();

public:
//attributes
	
//constructors
	//Basic constructor
	CameraSim(void);

	//Destructor
	virtual ~CameraSim(void);

//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	//set laser properties

	void drawGL();

	//Camera Methods
	virtual bool getImage(Image& data)
	{
		data=image;
		return true;
	}
	//using CameraSim for displaying other Camera data
	virtual void setImage(const Image& data){
		image=data;
	}
};

};
#endif  //__MRCORE__LASERSCANNERVIRTUAL__H
