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

#include "world.h"

#include <math.h>

namespace mr{
IMPLEMENT_MR_OBJECT(World)

void World::writeToStream(Stream& stream)
{
	EntitySet::writeToStream(stream);

}
void World::readFromStream(Stream& stream)
{
	EntitySet::readFromStream(stream);

}


World::World(void)
{
}

World::~World(void)
{
}
bool World::checkCollisionWith(SolidEntity &solid)
{
	for(int i=0;i<(int)(objects.size());i++)
	{	
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		//en cuanto hay colision devuelvo true
		if(aux){
			if(aux!=&solid)
			if(aux->checkCollisionWith(solid))return true;
		}
	}
	return false;
}

void World::simulate(double t)
{
for(int i=0;i<(int)(objects.size());i++)
	objects[i]->simulate(t);
}
BoundingBox World::getBoundingBox()
{
	BoundingBox nbox;
	for(int i=0;i<(int)(objects.size());i++)
	{
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		if(aux)nbox.includeBox(aux->getAbsoluteBoundingBox());
	}
return nbox;
}
}//mr