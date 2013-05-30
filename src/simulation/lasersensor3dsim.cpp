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


#include "lasersensor3dsim.h"
#include "../gl/gltools.h"

#include <math.h>

namespace mr{
	
IMPLEMENT_MR_OBJECT(LaserSensor3DSim)

void LaserSensor3DSim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);


}
void LaserSensor3DSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);

}
void LaserSensor3DSim::writeToXML(XMLElement* parent)
 {SolidEntity::writeToXML(parent);}
void LaserSensor3DSim::readFromXML(XMLElement* parent)
 {SolidEntity::readFromXML(parent);}


//constructors
LaserSensor3DSim::LaserSensor3DSim(void)
{
setDrawReferenceSystem(); //by default the refence system is drawn
setColor(1,1,0);
}



LaserSensor3DSim::~LaserSensor3DSim(void)
{

}
void LaserSensor3DSim::drawGL()
{
	//Draw axis
	
	PositionableEntity::drawGL();
	//DrawData
	glPushMatrix();
	location.getAbsoluteT3D().transformGL();
	material.loadMaterial();
	data.drawGL();
	glPopMatrix();
}

}//mr
