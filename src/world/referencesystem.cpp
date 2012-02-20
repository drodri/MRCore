/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio
 * Tuthor:	Miguel Hernando Gutierrez
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

#include "referencesystem.h"
#include "positionableentity.h"
#include <math.h>

namespace mr{

ostream& operator<<(ostream& os, const ReferenceSystem& r)
{
	//os<<r.x<<" "<<r.y<<" "<<r.z;
	return os;
}

ReferenceSystem::ReferenceSystem(PositionableEntity *_owner):base(0),owner(_owner)
{
}

ReferenceSystem::~ReferenceSystem(void)
{
}

void ReferenceSystem::setRelativePosition(Vector3D rp)
{
	//Update the relative position of PositionableEntity
	relative.position=rp;
	//necesita actualizarse == SI
	setNeedToUpdate();//Necesita actualizarse
}

void ReferenceSystem::doUpdate()//Actualizate
{	
	if(!needToUpdate)return;
	if(base)//Si tiene padre...
		absolute=(base->getAbsoluteT3D())*relative;
	else//Si no tiene padre...
		absolute=relative;//Coinciden
	needToUpdate = false;

}

void ReferenceSystem::setNeedToUpdate()//¿Necesita actualizarse?
{
	//if it needs to update, transmission downwards is not needed;
	if(needToUpdate == true)return; 	
	needToUpdate=true;
	//informo al owner si es que lo hay de que se ha modificado la posicion
	if(owner)owner->locationUpdated();

	//needToUpdate Propagation to springs
	for(int i=0;i<(int)(dependentSystem.size());i++)
				dependentSystem[i]->setNeedToUpdate();
		
}

bool ReferenceSystem::linkTo(ReferenceSystem *rs)
{
	if(rs!=base)//if it is not currently dependant
	{   //it si posible to link if this is not parent grand parent ...etc) of rs	
		if(isAncestorOf(rs)==false){
			unLink();
			base=rs;//rs es el padre de *this
			if(rs)rs->dependentSystem.push_back(this);//set *this as dependant of 
			setNeedToUpdate();
			return true;
			}
		return false;
	}
	return true;

}
void ReferenceSystem::unLink()
{
	if(base==0)return;

	for(int i=0;i<(int)(base->dependentSystem.size());i++)
		if(this==base->dependentSystem[i]){
			base->dependentSystem.erase(base->dependentSystem.begin()+i);
			base=0;
			return;
		}
	base=0;
}
bool ReferenceSystem::isAncestorOf(ReferenceSystem *rs)
{
	if(rs==0)return false;
	while(rs->base){
		if(rs->base==this)return true;
		rs=rs->base;
		}
	return false;
}
void ReferenceSystem::Draw()
{
	doUpdate();
	absolute.drawGL();
	
}
void ReferenceSystem::transformGL()
{
	doUpdate();
	absolute.transformGL();
}
}//mr
