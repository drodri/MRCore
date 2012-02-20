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

#include "facesetpart.h"
#include "math/interactions.h"
#include <math.h>

namespace mr{

IMPLEMENT_MR_OBJECT(FaceSetPart)

void FaceSetPart::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
	int num=(int)faces.size();
	stream<<num;
	for(int i=0;i<num;i++)
		faces[i].writeToStream(stream);

}
void FaceSetPart::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
	int num;
	stream>>num;
	for(int i=0;i<num;i++){
		Face aux;	
		aux.readFromStream(stream);
		addFace(aux);
	}

}
ostream& operator<<(ostream& os, const FaceSetPart& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
FaceSetPart::FaceSetPart(void)
{

}



FaceSetPart::~FaceSetPart(void)
{

}
//the object position have been modified... therefore the absolute faces
//have to be recomputed
void  FaceSetPart::locationUpdated(){
PrimitiveSolidEntity::locationUpdated();
for(int i=0;i<(int)(absolutefaces.size());i++)
	absolutefaces[i].setBase(getAbsoluteT3D()*faces[i].getBase());
}

//methods
void FaceSetPart::addFace(Face face)
{
	faces.push_back(face);
	//change of geometry actualization of the evolving box
	for(int i=0;i<face.getNumVertex();i++){
		box.includePoint(face.getAbsoluteVertex(i));
	}
	setBoxNeedToBeUpdated();

	face.setBase(getAbsoluteT3D()*face.getBase());
	absolutefaces.push_back(face);
	createWiredModel();
}

void FaceSetPart::drawGL()
{
	PrimitiveSolidEntity::drawGL();
	for(int i=0;i<(int)(absolutefaces.size());i++)
		absolutefaces[i].drawGL();
}
bool FaceSetPart::rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)
{
//chequeo min max
if(!intersectable)return false;
if(getAbsoluteBoundingBox().checkMinMax(ori,dir)==false)return false;

bool flag=false;
double aux;
	for(int i=0;i<(int)absolutefaces.size();i++){
		if(Interactions::faceRay(absolutefaces[i],ori,dir,aux)){
			if(!flag)dist=aux;
			else if(aux<dist)dist=aux;
			flag=true;
		}
	}
return flag;

}

bool  FaceSetPart::segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints)
{
if(!intersectable)return false;
//chequeo min max
if(getAbsoluteBoundingBox().checkMinMax(s)==false)return false;
//se podrían hacer más test de descarte pero de momento implemento el cara a cara. 
bool flag=false;
Vector3D auxp;
for(int i=0;i<(int)absolutefaces.size();i++)
	{
	if(Interactions::faceSegment(absolutefaces[i],s,auxp)){
		if(ipoints==0)return true;
		flag=true;
		ipoints->push_back(auxp);
		}
	}
return flag;
}
/*bool FaceSetPart::checkCollisionWith(SolidEntity &solid)
{
	if(checkBoundingBoxes(solid)==false)return false;
	//antes de proceder a la detección concreta... procedo a ver
	//pared y bounding box
	BoundingBox bb=solid.getAbsoluteBoundingBox();
	for(int i=0;i<(int)absolutefaces.size();i++){
		if(bb.checkMinMax(absolutefaces[i]))
		{
			//detección concreta y exacta face contra solid
			//tendría que detectar si es un basic solid en cuyo caso
			//solicito el modelo en alambre y detecto colisión contra él
			return true;
		}
	}
	return false;
}*/
void FaceSetPart::createWiredModel()
{
int i,j,num;
wiredModel.clear();
	for(i=0;i<(int)faces.size();i++){
		num=faces[i].getNumVertex();
		for(j=0;j<num;j++){
			wiredModel.push_back(Segment3D(faces[i].getAbsoluteVertex(j),faces[i].getAbsoluteVertex((j+1)%num)));
		}
	}
	setWiredModelNeedToBeUpdated();
}
}//mr
