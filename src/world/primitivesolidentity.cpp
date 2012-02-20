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


#include "primitivesolidentity.h"

#include <math.h>

namespace mr{



//constructors
PrimitiveSolidEntity::PrimitiveSolidEntity(void)
{
wiredModelNeedToBeUpdated=true;
}



PrimitiveSolidEntity::~PrimitiveSolidEntity(void)
{

}

void PrimitiveSolidEntity::locationUpdated(){
	SolidEntity::locationUpdated();
	wiredModelNeedToBeUpdated=true;
}
const vector<Segment3D> &PrimitiveSolidEntity::getWiredModel()
{
	Transformation3D T=getAbsoluteT3D();
	if(wiredModelNeedToBeUpdated){
		absoluteWiredModel.clear();
		for(int i=0;i<(int)wiredModel.size();i++){
			absoluteWiredModel.push_back(T*(wiredModel[i]));
		}
		wiredModelNeedToBeUpdated=false;
	}
	return absoluteWiredModel;
}

bool PrimitiveSolidEntity::checkCollisionWith(SolidEntity &solid)
{
	if(checkBoundingBoxes(solid)==false)return false;
	PrimitiveSolidEntity *aux=dynamic_cast<PrimitiveSolidEntity *>(&solid);
	if(aux){
		if(checkWiredModelCollisionWith(*aux))return true;
		if(aux->checkWiredModelCollisionWith(*this))return true;
		return false;
		}
	
	return solid.checkCollisionWith(*this);
	
}
bool PrimitiveSolidEntity::checkWiredModelCollisionWith(PrimitiveSolidEntity &sol)
{
//test bounding boxes:
	//return true;

	//my wired model against sol faces
	getWiredModel();//make sure it is updated
	int i,num=absoluteWiredModel.size();
	for(i=0;i<num;i++)if(sol.segmentIntersection(absoluteWiredModel[i],0))return true;
	return false;
}
void PrimitiveSolidEntity::drawGL()
{
SolidEntity::drawGL();
if(drawBox)	{
	getWiredModel();
	int i,num=absoluteWiredModel.size();
	for(i=0;i<num;i++)absoluteWiredModel[i].drawGL();
	}
}
}//mr
