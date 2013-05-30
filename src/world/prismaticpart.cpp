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

#include "prismaticpart.h"
#include "../math/mrmath.h"
#include "../math/interactions.h"
#include <fstream>
using namespace std;
namespace mr{

IMPLEMENT_MR_OBJECT(PrismaticPart)

void PrismaticPart::writeToStream(Stream& stream)
{
	PrimitiveSolidEntity::writeToStream(stream);
	stream<<height;
	polygonalBase.writeToStream(stream);
}
void PrismaticPart::readFromStream(Stream& stream)
{
	PrimitiveSolidEntity::readFromStream(stream);
	stream>>height;
	Face f;
	f.readFromStream(stream);
	setPolygonalBase(f); //this function implyies the updating of all aux models
}

void PrismaticPart::writeToXML(XMLElement* parent)
{
	if (height!=1.0) //it is its default value
	{
		//XMLAux aux;
		XMLVariable* alt= new XMLVariable("height",XMLAux::string_Convert<double>(height).c_str());
		parent->AddVariable(alt);
	}

		PrimitiveSolidEntity::writeToXML(parent);
		polygonalBase.writeToXML(parent);

}

void PrismaticPart::readFromXML(XMLElement* parent)
{
	double h=height;

	if(parent->FindVariableZ("height"))
			h=XMLAux::GetValueDouble(parent->FindVariableZ("height"));

	setHeight(h);
	Face f;
	f.readFromXML(parent);
	setPolygonalBase(f);
	PrimitiveSolidEntity::readFromXML(parent);

}

ostream& operator<<(ostream& os, const PrismaticPart& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
PrismaticPart::PrismaticPart(void)
{
height=1.0;
setRegularPolygonBase(height,4);
setPolygonNeedToBeUpdated();
}



PrismaticPart::~PrismaticPart(void)
{

}
//the object position have been modified... therefore the absolute mesh
//have to be recomputed when needed
void  PrismaticPart::locationUpdated(){
//realizo las operaciones genericas necesarias por mover un solido
PrimitiveSolidEntity::locationUpdated();
//operaciones especificas:
//la posicion absoluta de los vertices no esta actualizada
setPolygonNeedToBeUpdated();
}

//methods
void  PrismaticPart::setHeight(double _height)
{
	if(_height<2*EPS)return; //only change if positive and > 2*EPS
	height=_height;
	//calculo el box relativo
	Vector3D maxV=polygonalBase.getMaxVertex();
	Vector3D minV=polygonalBase.getMinVertex();
	maxV.z=height;
	minV.z=0;
	box=BoundingBox(minV,maxV);
	//el box absoluto debera ser recalculado si se necesita
	setBoxNeedToBeUpdated();
	//la posicion absoluta de los vertices no esta actualizada
	setPolygonNeedToBeUpdated();
	//wired model recomputed
	createWiredModel();
}
Face PrismaticPart::getPolygonalBase(){return polygonalBase;}
void PrismaticPart::setPolygonalBase(const Face &fin)
{
// base creation
Transformation3D t;
Face f(fin);
f.setBase(t);
polygonalBase=f;
	//caution: the base is defined with the normal upwards
	lateralNormals.clear();
	int num=f.getNumVertex();
	for(int i=0;i<num;i++){
		Vector3D n=f.getAbsoluteVertex((i+1)%num)-f.getAbsoluteVertex(i);
		n.normalize();
		if(f.isClockwise())lateralNormals.push_back(Vector3D(-n.y,n.x,0));
		else lateralNormals.push_back(Vector3D(n.y,-n.x,0));
	}
	absPolygonalBase=f; //a copy that have to be updated
	absPolygonalTop=f;
	absLateralNormals=lateralNormals;
	//calculo el box relativo
	Vector3D maxV=f.getMaxVertex();
	Vector3D minV=f.getMinVertex();
	maxV.z=height;
	minV.z=0;
	box=BoundingBox(minV,maxV);
	//el box absoluto debera ser recalculado si se necesita
	setBoxNeedToBeUpdated();
	//la posicion absoluta de los vertices no esta actualizada
	setPolygonNeedToBeUpdated();
	//wired model recomputed
	createWiredModel();
}
void PrismaticPart::setPolygonalBase(const vector<Vector2D> &list_of_vertex)
{
	// base creation
	Transformation3D t;
	Face f(t,list_of_vertex);
	setPolygonalBase(f);
}
void PrismaticPart::setRegularPolygonBase(double radius,int numVertex)
{
vector<Vector2D> list_of_vertex;
 for(int i=0;i<numVertex;i++)
 {
	list_of_vertex.push_back(Vector2D(radius*cos((2*i*PI)/numVertex),radius*sin((2*i*PI)/numVertex)));
 }
setPolygonalBase(list_of_vertex);
}
void PrismaticPart::updatePolygon()
{
if(polygonNeedToBeUpdated==false)return;
Transformation3D t=getAbsoluteT3D();
absPolygonalBase.setBase(t);
//normals transformation
for(int i=0;i<(int)lateralNormals.size();i++)
	absLateralNormals[i]=t.orientation*lateralNormals[i];
Vector3D h(0,0,height);
absHeight=(t.orientation)*h;
t.position=t.position+absHeight;
absPolygonalTop.setBase(t);

polygonNeedToBeUpdated=false;
}
void PrismaticPart::createWiredModel()
{
	int i,num;
	wiredModel.clear();
	Face &f=polygonalBase;
	
	num=f.getNumVertex();
	Vector3D z(0,0,height);
	for(i=0;i<num;i++){
		Vector3D a=f.getAbsoluteVertex(i);
		Vector3D b=f.getAbsoluteVertex((i+1)%num);
		wiredModel.push_back(Segment3D(a,b));
		wiredModel.push_back(Segment3D(a,a+z));
		wiredModel.push_back(Segment3D(a+z,b+z));
	}
setWiredModelNeedToBeUpdated();
}
void PrismaticPart::drawGL()
{
	PrimitiveSolidEntity::drawGL();
	material.loadMaterial();
	//en este caso hay que aplicar la transformacion y dibujar las caras sin 
	//transformar... el mesh transformado es solo ncesario para calculos de
	//colision etc... en el dibujo no se recalcula
	glPushMatrix();
	location.transformGL();
	glNormal3d(0,0,-1);
	polygonalBase.drawPrimitive(false); //base
	int num=(int)lateralNormals.size();
	for(int i=0;i<num;i++){ //lateral faces
		glNormal3dv(lateralNormals[i].values);
		glBegin(GL_POLYGON);
		Vector3D up1=polygonalBase.getAbsoluteVertex(i)+Vector3D(0,0,height);
		Vector3D up2=polygonalBase.getAbsoluteVertex((i+1)%num)+Vector3D(0,0,height);
		glVertex3dv(polygonalBase.getAbsoluteVertex(i).values);
		glVertex3dv(up1.values);
		glVertex3dv(up2.values);
		glVertex3dv(polygonalBase.getAbsoluteVertex((i+1)%num).values);
		glEnd();
	}
	glTranslated(0,0,height);
	glNormal3d(0,0,1);
	polygonalBase.drawPrimitive(); //top
	//mesh.drawGL();
	glPopMatrix();
	
}
bool PrismaticPart::rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)
{
	if(!intersectable)return false;
	//chequeo min max
	if(getAbsoluteBoundingBox().checkMinMax(ori,dir)==false)return false;
	//actualizo si es necesario la posición absoluta de los datos necesarios para tener el prisma
	updatePolygon();

	bool flag=false;
	double aux;
	//base
	if(Interactions::faceRay(absPolygonalBase,ori,dir,aux)){
		if(!flag)dist=aux;
		else if(aux<dist)dist=aux;
		flag=true;
	}
	//top
	if(Interactions::faceRay(absPolygonalTop,ori,dir,aux)){
		if(!flag)dist=aux;
		else if(aux<dist)dist=aux;
		flag=true;
	}
	//optimized lateral faces

	for(int i=0;i<absPolygonalBase.getNumVertex();i++)
	{
		if(rayLateralIntersection(i,ori,dir,aux)){
			if(!flag)dist=aux;
			else if(aux<dist)dist=aux;
			flag=true;
		}
	}
return flag;

}

bool  PrismaticPart::segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints)
{
	if(!intersectable)return false;
	//chequeo min max
	if(getAbsoluteBoundingBox().checkMinMax(s)==false)return false;
	updatePolygon();  
	
	bool flag=false;

	double dist;
	Vector3D dir=s.p2-s.p1;
	Vector3D ori=s.p1;

	//base
	if(Interactions::faceRay(absPolygonalBase,ori,dir,dist)){
		if(dist<1.0){
			if(ipoints)ipoints->push_back(ori+dir*dist);
			flag=true;
		}
	}
	if((ipoints==0)&&(flag))return true;
	//top
	if(Interactions::faceRay(absPolygonalTop,ori,dir,dist)){
		if(dist<1.0){
			if(ipoints)ipoints->push_back(ori+dir*dist);
			flag=true;
		}
	}
	if((ipoints==0)&&(flag))return true;
	//optimized lateral faces

	for(int i=0;i<absPolygonalBase.getNumVertex();i++)
	{
		if(rayLateralIntersection(i,ori,dir,dist)){
			if(dist<1.0){
				if(ipoints)ipoints->push_back(ori+dir*dist);
				flag=true;
			}
		if((ipoints==0)&&(flag))return true;
		}
	}
return flag;

	
}
bool PrismaticPart::rayLateralIntersection(int i,Vector3D &ori, Vector3D &dir, double &dist)
{
	Vector3D pori=absPolygonalBase.getAbsoluteVertex(i);
	Vector3D &u=dir;
	Vector3D w=ori-pori;
	Vector3D n=absLateralNormals[i];


    double     D = n*u;
    double    N = -(n*w);

    if (fabs(D) < EPS) {          // ray is parallel to plane
		if (N == 0)	// ray lies in plane
			return false;
        else
            return false;                   // no intersection
    }
    // they are not parallel
    // compute intersect param
    dist = N / D;
    if (dist < 0 )
        return false;                       // no intersection
    // compute ray intersect point and
	//transform to face coordinates
	Vector3D planePoint=ori + u*dist-pori;
	int num=absPolygonalBase.getNumVertex();
	Vector3D vv=absPolygonalBase.getAbsoluteVertex((i+1)%num)-pori;
	double uc,vc;
	vc=absHeight*planePoint;
	if((vc<0)||(vc>height*height))return false;
	uc=vv*planePoint;
	if((uc<0)||(uc>vv*vv))return false;

	return true;
	
}
}//mr
