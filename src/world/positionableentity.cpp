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

#include "positionableentity.h"
#include "tcp.h" //needed for checking the special way of working of tcp's in linkto operations.
#include "world.h"
#include "composedentity.h"
#include "gl/gltools.h"

#include <math.h>

namespace mr{
IMPLEMENT_MR_OBJECT(PositionableEntity)
void PositionableEntity::writeToStream(Stream& stream)
{
	stream<<name;
	getRelativeT3D().writeToStream(stream);
}
void PositionableEntity::readFromStream(Stream& stream)
{
	Transformation3D aux;
	stream>>name;
	aux.readFromStream(stream);
	setRelativeT3D(aux);
}

void PositionableEntity::writeToXML(XMLElement* parent)
{

	XMLVariable* nam= new XMLVariable("name",name.c_str());
	parent->AddVariable(nam);

	getRelativeT3D().writeToXML(parent);


}

void PositionableEntity::readFromXML(XMLElement* parent)
{
	//XMLElement* posi;
	//if(parent->FindElementZ("positionableEntity"))
	//	posi=parent->FindElementZ("positionableEntity");
	//else
	//	posi=parent;

	char cad[50]={0};
	if(parent->FindVariableZ("name"))
	{
		name=XMLAux::GetValueCadena(parent->FindVariableZ("name"));
	}

	Transformation3D aux;
	aux.readFromXML(parent);
	setRelativeT3D(aux);
}


ostream& operator<<(ostream& os, const PositionableEntity& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
PositionableEntity::PositionableEntity(void):owner(0)
{
	//First relative position
	drawRefSystem=false;
	drawSelected=false;
	LinkTo(0);
	location.setOwner(this);
}

PositionableEntity::PositionableEntity(string n, Vector3D pos_rel):owner(0)
{
	//Set first relative position
	drawRefSystem=false;
	drawSelected=false;
	name=n;
	location.setRelativePosition(pos_rel);
	location.setOwner(this);
}

PositionableEntity::~PositionableEntity(void)
{
	if(owner){
		EntitySet *aux=owner;
		owner=0;
		aux->remove(this);
	}
}
const PositionableEntity & PositionableEntity::operator =(const PositionableEntity &pe){
		name+="Copy of ";
		name+=pe.name;
		//important. The copy, initially dont belongs to a world
		owner=0;
		//and its location is 0,0,0 and not referred to any one
		location.setOwner(this);
		LinkTo(0);
		return (*this);
	}
//Methods
//protected method, it is responsible of the posible propagation of
//belonging to a set for entities that include other entities
//the basic one is just a "set" method
void PositionableEntity::setOwner(EntitySet *w){

if(w==0)return; //tal vez hay que plantearse que hacer para desagregar :S
if(owner)return;

if(w->hasObject(this)==false)w->addObject(this);
owner=w;
//the owner have to be propagated to all the dependent systems whose owner is 0
//it is not posible to have an owner at this point
ReferenceSystem *outputLocation=getReferenciableLocation();
int i,n=outputLocation->getNumberOfDependents();
PositionableEntity *aux;
for(i=0;i<n;i++){
	aux=(outputLocation->getDependent(i))->getOwner();
	if(aux==this)aux->setOwner(w); //reference system associated into the same object (e.g.: joints)
	if(aux->owner==0)aux->setOwner(w);
	}

}
void PositionableEntity::drawGL()
{
	//Draw axis
	if(drawRefSystem)location.Draw();
	if(drawSelected)glDisable(GL_LIGHTING);
	else glEnable(GL_LIGHTING);


//only for test purposes actually
	//Draw arms
/*	if(location.getBase())//getPadre()
	{
		Vector3D aux=location.getBase()->getAbsolutePosition();//Referencia absoluta del elemento padre
		Vector3D aux2=location.getAbsolutePosition();

		glDisable(GL_LIGHTING);
		glLineWidth(1.5);
		glBegin(GL_LINES);
			GLTools::Color(ORANGE);
			glVertex3f(aux2.x,aux2.y,aux2.z);
			glVertex3f(aux.x,aux.y,aux.z);
		glEnd();
		glEnable(GL_LIGHTING);
	}*/
}
ReferenceSystem * PositionableEntity::getReferenciableLocation()
{
	return &location;
}

void PositionableEntity::LinkTo(PositionableEntity *parent)
{
	/*links are only allowed between elements
	 of the same entity set. links among reference systems of two diferent entity sets are
	 only allowed if the spring is the whole entity set, and the father has a external plug 
	 for external references (TCP). this last is not managed in this class but in the entityclass */

	/*if the parent has an owner and this->owner=0, the object will be included automatically in the same collection
	*/

	if(parent){
		//tcp special case
		if(dynamic_cast<Tcp *>(parent)){
			
			ComposedEntity *auxOwner=dynamic_cast<ComposedEntity *>(parent->owner);
			//condition 1: tcp have to have a ComposedEntity as owner
			if(auxOwner==0)return;
			//condition 2: it is not posible to link a tcp into the same ComposedEntity
			if(parent->owner==owner)return;
			//condition 3: it is not posible to link to a tcp that belong to an object without owner (IMPORTANT)
		//	if(auxOwner->owner==0)return;
			//condition 4: 
			//if I have no owner, and the tcp composedEntity has owner.. I add the object to that entityset
			if((owner==0)&&(auxOwner->owner))setOwner(auxOwner->owner);
			//condition 5: //my owner have to be the same owner of my entityset
			if(auxOwner->owner==owner)location.linkTo(parent->getReferenciableLocation());

			return;
			}
		if((owner==0)&&(parent->owner))setOwner(parent->owner);
		if(parent->owner!=owner)return;
		location.linkTo(parent->getReferenciableLocation());
	}
	else location.linkTo(0);
	
}
World *PositionableEntity::getWorld()
{
World *aux;
aux=dynamic_cast<World *>(owner);
if(aux)return aux;

PositionableEntity *paux;
paux=dynamic_cast<PositionableEntity *>(owner);
if(paux)return paux->getWorld();
return 0;
}
void PositionableEntity::setAbsoluteT3D(const Transformation3D &m)
{
ReferenceSystem *base=location.getBase();
	if(base){
		Transformation3D aux=((base->getAbsoluteT3D()).inverted())*m;
		setRelativeT3D(aux);
	}else setRelativeT3D(m);
}
}//mr
