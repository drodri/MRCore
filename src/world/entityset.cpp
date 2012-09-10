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

#include "entityset.h"

#include <math.h>
#include "tcp.h"
#include "composedentity.h"
namespace mr{
IMPLEMENT_MR_OBJECT(EntitySet)
void EntitySet::writeToStream(Stream& stream)
{
	//primero se almacenan los objetos que pertencen al sistema 
 int i,num=(int)objects.size();
 int internalRelations=0;
 int tcpRelations=0;
 stream<<num;
   for(i=0;i<num;i++){
	 stream.write(objects[i]);
	 //compruebo si su parent es otro objeto de la colección
	 if(objects[i]->getLinkedTo()){
		 if(this==objects[i]->getLinkedTo()->getOwner())internalRelations++;
		 Tcp *check=dynamic_cast<Tcp *>(objects[i]->getLinkedTo());
		 if(check){
			 ComposedEntity *aux=dynamic_cast<ComposedEntity *>(check->getOwner());
			 if(aux){
				 if(this==aux->getOwner())tcpRelations++;
			 }
		 }
	 }
   }
   //transmito el numero de las relaciones que hay entre objetos
	 stream<<internalRelations;
   //transmito los emparejamientos
	for(i=0;i<num;i++){
		if(objects[i]->getLinkedTo())
			if(this==objects[i]->getLinkedTo()->getOwner()){
				int a=getIndexOf(objects[i]->getLinkedTo());
				stream<<i<<a;
			
			}
	}
	//transmito uniones a tcp´s
	stream<<tcpRelations;
	for(i=0;i<num;i++){
		if(objects[i]->getLinkedTo())
		{
				Tcp *check=dynamic_cast<Tcp *>(objects[i]->getLinkedTo());
				if(check){
					 ComposedEntity *aux=dynamic_cast<ComposedEntity *>(check->getOwner());
					 if(aux){ 
						 if(this==aux->getOwner()){
							int a=getIndexOf(aux);
							int b=aux->getTcpIndex(check);
							stream<<i<<a<<b;
						 }
					 }
				 }
			
		}
	}

  

}

void EntitySet::readFromStream(Stream& stream)
{
int i,num;
 stream>>num;
 //PositionableEntity *aux;
 objects.clear();
 for(int i=0;i<num;i++){
	 PositionableEntity * aux=dynamic_cast<PositionableEntity *>(stream.read());
	 addObject(aux);
  }
stream>>num;
int a,b;
for(i=0;i<num;i++){
	stream>>a>>b;
	if(b>=0)objects[a]->LinkTo(objects[b]);
}
stream>>num;
int c;
for(i=0;i<num;i++){
	stream>>a>>b>>c;
	if(b>=0){
		ComposedEntity *aux=dynamic_cast<ComposedEntity *>(objects[b]);
		if(aux)objects[a]->LinkTo(aux->getTcp(c));
	}
}
}


EntitySet::EntitySet(void)
{
}

EntitySet::~EntitySet(void)
{
	for(unsigned int i=0;i< objects.size();i++)
		delete objects[i];
	objects.clear();
}

bool EntitySet::addObject(PositionableEntity * p)
{
	//if it is added, it is not posible to add it again
    if(hasObject(p))return false;
	//if belongs to another entityset it has to be removed from it before
	if(p->owner)return false;
	//addition
	objects.push_back(p);
	p->setOwner(this);
	//if this entity set is linkable: not world, and p is not linked it is linked
	linkToBase(p);
	
	return true;
}
PositionableEntity *EntitySet::getObjectByName(string _name)
{
for(int i=0;i<int(objects.size());i++)
	if(_name==objects[i]->getName())return objects[i];
return 0;
}
void EntitySet::destroyContent()
{
	//Delete each object one to one
	for(int i=0;i<int(objects.size());i++)
		delete objects[i];
	//Clean the vector 
	objects.clear();
}

void EntitySet::erase(int index)
{
	if((index>=0)&&(index<(int)(objects.size()))){
		delete(objects[index]);
		objects.erase(objects.begin()+index);
		}
}

void EntitySet::remove(PositionableEntity *p)
{
	for(int i=0;i<(int)(objects.size());i++)
		if(p==objects[i]){
			objects.erase(objects.begin()+i);
			//a trick for avoiding cyclic deletion
			if(p->owner)p->setOwner(0);
			return;
			}
	//a trick for avoiding cyclic deletion
	if(p->owner)p->setOwner(0);
}
void EntitySet::drawGL()
{
	for(int i=0;i<(int)(objects.size());i++)
		objects[i]->drawGL();
}
bool EntitySet::segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints)
{
	SolidEntity *me=dynamic_cast<SolidEntity *>(this);
	if((me)&&(!(me->isIntersectable())))return false;

	bool flag=false;
	for(int i=0;i<(int)(objects.size());i++)
	{
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		if(aux){
			if(aux->segmentIntersection(s,ipoints))flag=true;
		}
		if((ipoints==0)&&(flag))return true;
	}
return flag;
}

bool EntitySet::rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)
{
	SolidEntity *me=dynamic_cast<SolidEntity *>(this);
	if((me)&&(!(me->isIntersectable())))return false;

	bool flag=false;
	for(int i=0;i<(int)(objects.size());i++)
	{
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		double distAux;
		if(aux){
			if(aux->rayIntersection(ori,dir,distAux)){
				if(!flag)dist=distAux;
				else dist=distAux<dist?distAux:dist;
				flag=true;
			}
		}
	}
return flag;

}

//filters

void EntitySet::getPrimitivesBoundingBoxesCutByPlane(const Transformation3D &t, vector<SolidEntity *> &subset,double range)
{
	for(int i=0;i<(int)(objects.size());i++)
	{
		SolidEntity *aux=dynamic_cast<SolidEntity *>(objects[i]);
		if(aux){//si es solido verifico el bounding box
			if(aux->getAbsoluteBoundingBox().checkMinMax(t,range)==true)
				{//si es un set, lo descompongo y si no ... lo agrego
					ComposedEntity *aux2=dynamic_cast<ComposedEntity *>(aux);
					if(aux2)aux2->getPrimitivesBoundingBoxesCutByPlane(t,subset, range);
					else{
						subset.push_back(aux);
					}

				}


			}
		
	}
}
}//mr
