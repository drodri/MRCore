/**********************************************************************
 *
 * This code is part of the MRcore project
 * Authors:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
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

#include "composedentity.h"
#include "tcp.h"
#include <math.h>

namespace mr{
IMPLEMENT_MR_OBJECT(ComposedEntity)

void ComposedEntity::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
	EntitySet::writeToStream(stream);

}
void ComposedEntity::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
	EntitySet::readFromStream(stream);

}


ComposedEntity::ComposedEntity(void)
{
//prueba
//drawBox=true;

}

ComposedEntity::~ComposedEntity(void)
{
}

bool ComposedEntity::linkToBase(PositionableEntity *p)
{
	if(p==0)return false;
	//if linked... do nothing
	if(p->location.getBase())return false;
	//else directly link
	p->location.linkTo(getReferenciableLocation());
	computeBoundingBox();
return true;
}
void ComposedEntity::drawGL()
{
	EntitySet::drawGL();
	SolidEntity::drawGL();
}
BoundingBox  ComposedEntity::getAbsoluteBoundingBox()
{
	//check if some of the contained solid objects have to update their bounding box
	if(boxNeedToBeUpdated)computeBoundingBox();
	return SolidEntity::getAbsoluteBoundingBox();

}
void ComposedEntity::computeBoundingBox()
{
	BoundingBox nbox;
	for(int i=0;i<(int)(objects.size());i++)
	{
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		if(aux)nbox.includeBox(aux->getAbsoluteBoundingBox());
	}
	box=nbox/getAbsoluteT3D();
	absoluteBox=nbox;
	boxNeedToBeUpdated=false;
}
int ComposedEntity::getTcpIndex(Tcp *tcp)
{
	int index=0;
	for(int i=0;i<(int)(objects.size());i++)
	{
		Tcp *aux=dynamic_cast<Tcp *>(objects[i]);
		if(aux==tcp)return index;
		if(aux)index++;
	}
	return -1;
}
Tcp *ComposedEntity::getTcp(int num)
{
	int index=0;
	for(int i=0;i<(int)(objects.size());i++)
	{
		Tcp *aux=dynamic_cast<Tcp *>(objects[i]);
		if(aux){if(index==num)return aux; else index++;}
	}
	return 0;
}
//Collision detection
bool ComposedEntity::checkCollisionWith(SolidEntity &solid)
{
	//chequeo los bounding boxes globales y si no voy elemento a elemento
	if(checkBoundingBoxes(solid)==false)return false;
	for(int i=0;i<(int)(objects.size());i++)
	{	
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		//en cuanto hay colision devuelvo true
		if(aux){
			if(aux->checkCollisionWith(solid))return true;
		}
	}
	return false;
}
void ComposedEntity::simulate(double t)
{
for(int i=0;i<(int)(objects.size());i++)
	objects[i]->simulate(t);
}
}//mr