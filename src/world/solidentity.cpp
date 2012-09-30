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


#include "solidentity.h"
#include "composedentity.h"
#include <math.h>

namespace mr{


ostream& operator<<(ostream& os, const SolidEntity& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
SolidEntity::SolidEntity(void)
{
boxNeedToBeUpdated=true;
drawBox=false;
intersectable=true;
}



SolidEntity::~SolidEntity(void)
{

}

void SolidEntity::locationUpdated()
{
PositionableEntity::locationUpdated();
boxNeedToBeUpdated=true;
//si pertenece a un SolidEntity... informa que debe recalcular el box
ComposedEntity *aux=dynamic_cast<ComposedEntity *>(owner);
if(aux)aux->setBoxNeedToBeUpdated();
}
void SolidEntity::drawGL()
{
PositionableEntity::drawGL();
if(drawBox)getAbsoluteBoundingBox().drawGL();
}

/*bool SolidEntity::checkCollisionWith(SolidEntity &solid)
{
return(checkBoundingBoxes(solid));

}*/
//it is not virtual, all the colligion checkers sould use it before doing a specific check
bool SolidEntity::checkBoundingBoxes(SolidEntity &solid)
{
//lo primero: si es el mismo objeto contra si mismo->false
if(this==&solid)return false;
//si no son intersectables tampoco
if(this->isIntersectable()==false)return false;
if(solid.isIntersectable()==false)return false;
if(boxNeedToBeUpdated)getAbsoluteBoundingBox();
return(absoluteBox.checkMinMax(solid.getAbsoluteBoundingBox()));
}

}//mr
