/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio &Miguel Hernando Gutierrez
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


#include "camerasim.h"
#include "../../gl/gltools.h"
#include "../../world/cylindricalpart.h"
#include <math.h>

namespace mr{

void CameraSim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
}
void CameraSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
}

ostream& operator<<(ostream& os, const CameraSim& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
CameraSim::CameraSim(void)
{
setDrawReferenceSystem(); //by default the refence system is drawn
setColor(1,1,0);

CylindricalPart *glass=new CylindricalPart(0.110,0.053,10);
glass->setColor(0.1,0.1,1.0);
(*this)+=glass;
}

CameraSim::~CameraSim(void)
{

}
void CameraSim::locationUpdated()
{
PositionableEntity::locationUpdated();
//the laser beam is only updated under request
}

void CameraSim::drawGL()
{
	//Draw axis
	PositionableEntity::drawGL();
	//DrawData
	glPushMatrix();
	location.getAbsoluteT3D().transformGL();
	material.loadMaterial();

	glPopMatrix();
}

}//mr
