/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
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

#include "spherepart.h"
/*
#include "../math/mrmath.h"
#include "../math/interactions.h"*/
#include <fstream>
using namespace std;
namespace mr{

IMPLEMENT_MR_OBJECT(SpherePart)

void SpherePart::writeToStream(Stream& stream)
{
	PrimitiveSolidEntity::writeToStream(stream);
	stream<<radius;
	stream<<meridian;
	stream<<parallel;

}
void SpherePart::readFromStream(Stream& stream)
{
	PrimitiveSolidEntity::readFromStream(stream);
	double rad;
	int mer,par;
	stream>>rad;
	stream>>mer;
	stream>>par;
	setNumVertex(mer,par);
	setRadius(rad);

}

ostream& operator<<(ostream& os, const SpherePart& s)
{
	//os<<s.x<<" "<<s.y<<" "<<s.z;
	return os;
}

//constructors
SpherePart::~SpherePart(void)
{
}

//the object position have been modified... therefore the absolute mesh
//have to be recomputed when needed
void  SpherePart::locationUpdated(){
	//necessary operations to move a solid
	PrimitiveSolidEntity::locationUpdated();
	//specific operations:

/***
No need to upgrade any vertex of sphere because it does not anyone
Absolute position of centeris update by primitive classes
***/
}

void  SpherePart::setRadius(double _radius)
{
	if(_radius<2*EPS)
		return; //only change if positive and >= 2*EPS
	radius=_radius;
//Calculate relative box
	Vector3D maxV( radius, radius, radius);
	Vector3D minV(-radius,-radius,-radius);

	box=BoundingBox(minV,maxV);
//el box absoluto debera ser recalculado si se necesita
	setBoxNeedToBeUpdated();

	//wired model recomputed
	createWiredModel();
}

void SpherePart::setNumVertex(int _meridian, int _parallel)
{
	meridian=_meridian;//Meridians	- default 12
	parallel=_parallel;//Parallels	- default 9
}

void SpherePart::createWiredModel()
{
	vector<double> v_cos_theta;
	vector<double> v_sin_theta;
	vector<double> v_cos_alfa;
	vector<double> v_sin_alfa;

	vector<Vector3D> vertex;
	
/***
Up and low sphere vertex
***/
	Vector3D north_p(0,0, radius);
	Vector3D south_p(0,0,-radius);

/***
Theta -> Angle from the axis of sphere
	Angles defined for each line parallel
	Parallel = 9 (default)
***/
	for(int i=0;i<parallel;i++)
	{
		v_cos_theta.push_back(cos(PI*(i+0.5)/parallel));
		v_sin_theta.push_back(sin(PI*(i+0.5)/parallel));
	}
/***
Alfa -> Angle of each vertex of polygon
	Angles defined to meridians
	Meridian = 12 (default)
***/
	for(int i=0;i<meridian;i++)
	{
		v_cos_alfa.push_back(cos(2*PI*i/meridian));
		v_sin_alfa.push_back(sin(2*PI*i/meridian));
	}

/***
Generate all vertex
***/
	for(int i=0; i<parallel;i++)
	{
		for(int j=0; j<meridian; j++)
		{
			vertex.push_back(Vector3D(
				radius*v_sin_theta[i]*v_cos_alfa[j],
				radius*v_sin_theta[i]*v_sin_alfa[j],
				radius*v_cos_theta[i]));
			
		}
	}

	wiredModel.clear();

	int tam=(int)vertex.size();//meridian * parallel

	for(int j=0;j<(parallel-1);j++)
	{
		for(int i=0;i<meridian; i++)
		{
			wiredModel.push_back(Segment3D(vertex[i+meridian*j],vertex[i + meridian*(j+1)]));
			wiredModel.push_back(Segment3D(north_p,vertex[i]));
			wiredModel.push_back(Segment3D(south_p,vertex[tam-i-1]));
		}
	}
	for(int j=0;j<parallel;j++)
	{
		wiredModel.push_back(Segment3D(vertex[meridian*j],vertex[meridian*(j+1)-1]));
		for(int i=0;i<(meridian-1);i++)
		{
			wiredModel.push_back(Segment3D(vertex[i+meridian*j],vertex[i+1+meridian*j]));
		}
	}

	vertex.clear();
	v_sin_theta.clear();
	v_cos_theta.clear();
	v_sin_alfa.clear();
	v_cos_alfa.clear();
}

void SpherePart::drawGL()
{
	PrimitiveSolidEntity::drawGL();
	material.loadMaterial();
	glPushMatrix();

	location.transformGL();

//	GLTools::Color(GREY,0.55f);

	glEnable(GL_LIGHTING);
	GLUquadricObj * qobj=gluNewQuadric();
	gluQuadricDrawStyle(qobj,GLU_FILL);//Solid
//	gluQuadricDrawStyle(qobj,GLU_LINE);//Wires 
	gluSphere(qobj,radius*0.95,2*meridian,2*parallel);
	gluDeleteQuadric(qobj);

	glDisable(GL_LIGHTING);

	glPopMatrix();
}

bool SpherePart::rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)
{
	if(!intersectable)
		return false;

	if(getAbsoluteBoundingBox().checkMinMax(ori,dir)==false)
		return false;

/***
Ori:	represents laser focus
Dir:	unitary vector that represents direction laser beam
Dist:	distance which laser beam interacts with some solid
sc:		center sphere
***/
	Vector3D sc=location.getRelativePosition();
	Vector3D sc_ori = (sc - ori).getUnitaryVector();
/***
Checkout that sphere is inside field vision laser: for this, checks that scalar product
between sc_ori and dir, ie, their cosine, it is possitive. If it is positive, laser beam see
sphere; If it is negative, sphere is not inside field laser vision.
***/
	if(sc_ori*dir<0)
		return false;
	bool flag;
	double a = 1;
	double b = 2*(dir.x*(ori.x-sc.x) + dir.y*(ori.y-sc.y) + dir.z*(ori.z-sc.z));
	double c = sc.module()*sc.module();

	c += ori.module()*ori.module();

	c -= 2* (sc.x*ori.x + sc.y*ori.y + sc.z*ori.z);

	c -= radius*radius;

	double bb4ac = b*b - 4*c;

	if(bb4ac < 0)//Not intersection
		return false;

/***
Last case, there are two solutions: segment intersects sphere in two points.
But laser beam, only interested dist is more near than focus laser.
Other solutions is dist = (-b + sqrt(bb4ac))/(2*a);
***/
	else// if(bb4ac >= 0)
	{
		flag=true;
		dist = (-b - sqrt(bb4ac))/2;
	}

	return flag;
}

bool SpherePart::segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints)
{
/***
Interaction between sphere and segment.
This method return the interaction points with sphere.
In this case, there are 6 solution:
	1º		Segment is inside sphere.
	2º-3º	One of two extremes is inside and other outside.
	4º		Not intersection.
	5º		Segment is tangent to sphere - Only solution point.
	6º		Segment interaction in two points.
***/
	if(!intersectable)
		return false;
	if(getAbsoluteBoundingBox().checkMinMax(s)==false)
		return false;
/***
Ori:	represents laser focus
Dir:	unitary vector that represents direction laser beam
Dist:	distance which laser beam interacts with some solid
sc:		center sphere
***/
	bool flag=false;
	double dist=(s.p2-s.p1).module();
	const double dista=dist;
	Vector3D dir=s.p2 - s.p1;
	Vector3D ori=s.p1;
	Vector3D sc=location.getRelativePosition();
	Vector3D sc_ori = sc - s.p1;

	double dist1=(ori-sc).module();
	double dist2=(s.p2-sc).module();

//Case 1
	if((dist1<radius) && (dist2<radius))//Not intersection. Segment is inside sphere
		return false;
//Case 2 & case 3 -> One of two extremes is inside and other outside.
	else if((dist1<radius) || (dist2<radius))
	{
		flag=rayIntersection(ori,dir,dist);
		//setDrawBox(); //this was for debug purposes.
		if(ipoints)ipoints->push_back(ori+dir*dist);
		if((ipoints==0) && (flag))return true;
	}

//Other cases
	else//Distances between ends to center is greater than radius
	{
		if(rayIntersection(ori,dir,dist))//If TRUE, there are colision
		{
			flag=true;
			//setDrawBox(); //this was for debug purposes.
			double aux=dist*dist + radius*radius - sc_ori.module()*sc_ori.module();//it is dist because it is new distance that intersection laser beam
			if(aux < 2*EPS)//Next to zero - Tangent - Case 5
			{
				if(ipoints)ipoints->push_back(ori + dir*dist);
				if((ipoints==0) && (flag))return flag;
			}
			else//Case 6 - Not tangent - Two intersection
			{
				if(ipoints)
				{
					ipoints->push_back(ori+dir*dist);
					Vector3D aux1=s.p2, aux2=(s.p1-s.p2);
					flag=rayIntersection(aux1,aux2,dist);
					ipoints->push_back(ori+dir*(dista-dist));
				}
				if((ipoints==0)&&(flag))return flag;
			}
		}
		else//Case 4 - Not intersection
		{
			//setDrawBox(false);//Delete bounding box
			return false;
		}
	}
	return flag;
}

}//mr