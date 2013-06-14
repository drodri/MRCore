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

void EntitySet::writeToXML(XMLElement* parent)
{
		//primero se almacenan los objetos que pertencen al sistema
	//XMLfile xmlfile;
 int i,num=(int)objects.size();
 int internalRelations=0;
 int tcpRelations=0;
 XMLfile file(parent);

   for(i=0;i<num;i++){

	file<<objects[i];  //save all the objects in XML_FILE
	file<<i; //put one ID in every xml objects
	}

	vector<XMLElement*> pObj=file.getXMLElementsObjects();

	 //compruebo si su parent es otro objeto de la colección
    for(i=0;i<num;i++){
		 if(objects[i]->getLinkedTo())
		   {
			 if(this==objects[i]->getLinkedTo()->getOwner())
			   {
					int a=getIndexOf(objects[i]->getLinkedTo());

					string _name=XMLAux::GetValueCadena(pObj[a]->FindVariableZ("name",true));
					XMLVariable* linkedTo;
					bool repeatedOrEmptyName=false;
					//linked one object with another one (i,a)
					if (_name.empty())
						repeatedOrEmptyName=true;

					else
					{
						for (int j=0;j<num;j++)
						{
							if (_name==XMLAux::GetValueCadena(pObj[j]->FindVariableZ("name")) && a!=j)
								repeatedOrEmptyName=true;

						}
					}

					if (repeatedOrEmptyName)
						linkedTo= new XMLVariable ("linkTo",XMLAux::setLinkTo("noName",a).c_str());

					else
						linkedTo= new XMLVariable ("linkTo",XMLAux::setLinkTo(_name,a).c_str());


					pObj[i]->AddVariable(linkedTo);

			   }
			}
		}

		//write unions to tcp's
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


							string _name=XMLAux::GetValueCadena(pObj[a]->FindVariableZ("name",true));
							string _nameTcp=string();
							XMLVariable* linkedToTcp;
							bool repeatedName=false;
							if(_name.empty())
							{
								stringstream str;
								str<<"Composed Entity"<<a;
								_name=str.str();
								pObj[a]->FindVariableZ("name")->SetValue(_name.c_str());
							}
							else
							{
								for (int j=0;j<num;j++)
								{
									if (_name==XMLAux::GetValueCadena(pObj[j]->FindVariableZ("name")) && a!=j)
										repeatedName=true;

								}
							}
							if (XMLAux::GetNameElement(pObj[a])=="ComposedEntity")
							{
								_nameTcp=XMLAux::GetValueCadena(pObj[a]->FindElementZ("Tcp")->FindVariableZ("name",true));
								if(_nameTcp.empty())
								{
									stringstream str;
									str<<"tcp"<<b;
									_nameTcp=str.str();
									pObj[a]->FindElementZ("Tcp")->FindVariableZ("name")->SetValue(_nameTcp.c_str());
								}
								if (repeatedName)
									linkedToTcp= new XMLVariable ("linkToTcp",XMLAux::setLinkToTcp("noName",_nameTcp,b,a).c_str());
								else
									linkedToTcp= new XMLVariable ("linkToTcp",XMLAux::setLinkToTcp(_name,_nameTcp,b,a).c_str());
							}
							else
							{
								if(repeatedName)
									linkedToTcp= new XMLVariable ("linkToTcp",XMLAux::setLinkToTcpDefault("noName",b,a).c_str());
								else
									linkedToTcp= new XMLVariable ("linkToTcp",XMLAux::setLinkToTcpDefault(_name,b,a).c_str());

							}

							pObj[i]->AddVariable(linkedToTcp);

						 }
					 }
				 }

		}
	}

}

void EntitySet::readFromXML(XMLElement* parent)
{
	int i,num=parent->GetChildrenNum();
	XMLElement** pObj=parent->GetChildren();
	XMLfile file(parent);
	objects.clear();
		for(int i=0;i<num;i++){
			PositionableEntity * aux=dynamic_cast<PositionableEntity *>(file.read(pObj[i]));
			addObject(aux);
		}


		for(i=0;i<num;i++){
			if(pObj[i]->FindVariableZ("linkTo"))
			{
				XMLVariable* link=pObj[i]->FindVariableZ("linkTo");
				string type=XMLAux::getTypeConectionLink(link,false);
				if (type=="conectionId")
				{
					int id_linked=link->GetValueInt();
					for (int j=0;j<num;j++)
					{
						if (pObj[j]->FindVariableZ("id"))
						{
							if (id_linked==pObj[j]->FindVariableZ("id")->GetValueInt())
								objects[i]->LinkTo(objects[j]);
						}
					}
				}
				else if(type=="conectionNames")
				{
					string nameLink=XMLAux::GetNameLinkTo(link);
					for (int j=0;j<num;j++)
					{
						if (nameLink==XMLAux::GetValueCadena(pObj[j]->FindVariableZ("name")))
							objects[i]->LinkTo(objects[j]);
					}

				}
			}
		}

		for(i=0;i<num;i++)
		{
			if(pObj[i]->FindVariableZ("linkToTcp"))
			{
				XMLVariable* linkTcp=pObj[i]->FindVariableZ("linkToTcp");
				string type=XMLAux::getTypeConectionLink(linkTcp);

				if (type=="conectionId")
				{
					int ids[2]={0};
					XMLAux::GetValueOwnerAndTcp(linkTcp,ids);
					int id_OwnerTcp=ids[0], id_linkTcp=ids[1];

						if(id_OwnerTcp>=0)
						{
							for (int j=0;j<num;j++)
							{
								if (pObj[j]->FindVariableZ("id"))
								{
									if (id_OwnerTcp==pObj[j]->FindVariableZ("id")->GetValueInt())
									{
										ComposedEntity *aux=dynamic_cast<ComposedEntity *>(objects[j]);

										if(aux)
										{
											if (XMLAux::GetNameElement(pObj[j])=="ComposedEntity")
											{
												int numChildCompos=pObj[j]->GetChildrenNum();
												if (numChildCompos)
												{
													XMLElement** childsComposed=pObj[j]->GetChildren();
													int indexTcp=0;
													for (int z=0;z<numChildCompos;z++)
													{
														if (XMLAux::GetNameElement(childsComposed[z])=="Tcp")
														{

															if (id_linkTcp==childsComposed[z]->FindVariableZ("id",true)->GetValueInt())
																objects[i]->LinkTo(aux->getTcp(indexTcp));
															else if (indexTcp==id_linkTcp)
																objects[i]->LinkTo(aux->getTcp(indexTcp));

															indexTcp++;

														}
													}
												}
											}
											else
												objects[i]->LinkTo(aux->getTcp(id_linkTcp));//it's supposed that user knows id_linkTcp=Tcp's index

										}
									}
								}
							}
						}
				}
				else if (type=="conectionNames")
				{
					vector<string> names=XMLAux::GetNameOwnerAndTcp(linkTcp);
					int idTcp=XMLAux::getIndTcp(names[1]);
					string nameOnlyTcp=XMLAux::getOnlyNameTcp(names[1]);
					for (int j=0;j<num;j++)
					{
						if (names[0]==XMLAux::GetValueCadena(pObj[j]->FindVariableZ("name")))
						{
							ComposedEntity *aux=dynamic_cast<ComposedEntity *>(objects[j]);

							if(aux)
							{

								if (XMLAux::GetNameElement(pObj[j])=="ComposedEntity")
								{
									int numChildCompos=pObj[j]->GetChildrenNum();
									if(numChildCompos)
									{
										XMLElement** childsComposed=pObj[j]->GetChildren();
										int indexTcp=0;

										for (int z=0;z<numChildCompos;z++)
										{

											if (XMLAux::GetNameElement(childsComposed[z])=="Tcp")
											{

												if (nameOnlyTcp==XMLAux::GetValueCadena(childsComposed[z]->FindVariableZ("name")) &&
														idTcp==childsComposed[z]->FindVariableZ("id")->GetValueInt())
													objects[i]->LinkTo(aux->getTcp(indexTcp));
												else if (nameOnlyTcp==XMLAux::GetValueCadena(childsComposed[z]->FindVariableZ("name")) &&
														idTcp==indexTcp)
													objects[i]->LinkTo(aux->getTcp(indexTcp));
												else if(idTcp==childsComposed[z]->FindVariableZ("id")->GetValueInt())
													objects[i]->LinkTo(aux->getTcp(indexTcp));
												else if (idTcp==indexTcp)
													objects[i]->LinkTo(aux->getTcp(indexTcp));

												indexTcp++;
											}
										}
									}
								}
								else
									objects[i]->LinkTo(aux->getTcp(idTcp));

							}
						}
					}

				}
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
	if((index>=0)&&(index<(int)(objects.size())))	
	{
		objects.erase(objects.begin()+index);
		delete objects[index];
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
