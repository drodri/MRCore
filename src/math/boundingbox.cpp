/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Miguel Hernando & Diego Rodríguez-Losada
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
 *     by the author ori licensor (but not in any way that suggests that
 *     they endorse you ori your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, ori build upon this work,
 *     you may distribute the resulting work only under the same ori
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs ori
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY ori FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/
#include "boundingbox.h"
#include "mrmath.h"

#include "gl/gltools.h"

namespace mr{
IMPLEMENT_MR_OBJECT(BoundingBox)


ostream& operator<<(ostream& os, const BoundingBox& p)
{
	os<<"max: "<<p.maxVertex<<endl;
	os<<"min: "<<p.minVertex<<endl;
	return os;
}
BoundingBox::BoundingBox()
{
validData=false;
}

BoundingBox::BoundingBox(Vector3D _minVertex, Vector3D _maxVertex)
{
validData=true;
minVertex=_minVertex;
includePoint(_maxVertex);
}
BoundingBox::~BoundingBox()
{

}

void BoundingBox::includePoint(Vector3D p)
{
//first point
if(!validData){minVertex=maxVertex=p;
	validData=true;
	return;
	}
if(p.x>maxVertex.x)maxVertex.x=p.x;
if(p.y>maxVertex.y)maxVertex.y=p.y;
if(p.z>maxVertex.z)maxVertex.z=p.z;

if(p.x<minVertex.x)minVertex.x=p.x;
if(p.y<minVertex.y)minVertex.y=p.y;
if(p.z<minVertex.z)minVertex.z=p.z;

}

void BoundingBox::writeToStream(Stream& stream)
{
	stream<<maxVertex.x<<maxVertex.y<<maxVertex.z;
	stream<<minVertex.x<<minVertex.y<<minVertex.z;
}
void BoundingBox::readFromStream(Stream& stream)
{
	stream>>maxVertex.x>>maxVertex.y>>maxVertex.z;
	stream>>minVertex.x>>minVertex.y>>minVertex.z;
}

BoundingBox operator*(Transformation3D T, BoundingBox& b)
{
	BoundingBox ret;
	Vector3D ori=T*b.minVertex;
	Vector3D dif=b.maxVertex-b.minVertex;
	Vector3D ux=(T.getVectorU())*dif.x;
	Vector3D uy=(T.getVectorV())*dif.y;
	Vector3D uz=(T.getVectorW())*dif.z;
	//compute all the box vertexes in order to obtaind the absolute oriented bounding box
	ret.includePoint(ori);
	ret.includePoint(ori+ux);
	ret.includePoint(ori+uy);
	ret.includePoint(ori+uz);
	ret.includePoint(ori+ux+uy);
	ret.includePoint(ori+ux+uz);
	ret.includePoint(ori+uy+uz);
	ret.includePoint(ori+ux+uy+uz);
	return ret;
}
BoundingBox operator/(BoundingBox& b,Transformation3D T)
{
	BoundingBox ret;
	Vector3D ori=T.obtainRelativeCoordinates(b.minVertex);
	Vector3D dif=b.maxVertex-b.minVertex;
	Vector3D ux=T.orientation.transposed()*(Vector3D(dif.x,0,0));
	Vector3D uy=T.orientation.transposed()*(Vector3D(0,dif.y,0));
	Vector3D uz=T.orientation.transposed()*(Vector3D(0,0,dif.z));
	//compute all the box vertexes in order to obtaind the T oriented bounding box
	ret.includePoint(ori);
	ret.includePoint(ori+ux);
	ret.includePoint(ori+uy);
	ret.includePoint(ori+uz);
	ret.includePoint(ori+ux+uy);
	ret.includePoint(ori+ux+uz);
	ret.includePoint(ori+uy+uz);
	ret.includePoint(ori+ux+uy+uz);
	return ret;
}
bool BoundingBox::checkMinMax(const Segment3D &s)
{
//alias
Vector3D &m=minVertex,&M=maxVertex;
//si ambas coordenadas son mayores o menores que el min o el max respect, no hay corte
if((s.p1.x<m.x)&&(s.p2.x<m.x))return false;
if((s.p1.x>M.x)&&(s.p2.x>M.x))return false;
if((s.p1.y<m.y)&&(s.p2.y<m.y))return false;
if((s.p1.y>M.y)&&(s.p2.y>M.y))return false;
if((s.p1.z<m.z)&&(s.p2.z<m.z))return false;
if((s.p1.z>M.z)&&(s.p2.z>M.z))return false;
//aqui se podría afinar mas si hubiera un test rápido de descarte... de momento no
return true;
}
bool BoundingBox::checkMinMax(Vector3D &o, Vector3D &u)
{
	//alias
	Vector3D &m=minVertex,&M=maxVertex;
	Vector3D vmax=M-o, vmin=m-o;
	for(int i=0;i<3;i++)
	{
		if(fabs(u[i])>EPS){
			BoundingBox box;
			double t1=vmax[i]/u[i];
			double t2=vmin[i]/u[i];
			box.includePoint(o+u*t1);
			box.includePoint(o+u*t2);
			if(box.minVertex[(i+1)%3]>maxVertex[(i+1)%3])return false;
			if(box.maxVertex[(i+1)%3]<minVertex[(i+1)%3])return false;
			if(box.minVertex[(i+2)%3]>maxVertex[(i+2)%3])return false;
			if(box.maxVertex[(i+2)%3]<minVertex[(i+2)%3])return false;
		}
	}
	return true;
}
bool BoundingBox::checkMinMax(Transformation3D t, double range) 
{
BoundingBox box=(*this)/t;
if(box.maxVertex.z<0)return false;
if(box.minVertex.z>0)return false;
if(range<0)return true;
if(box.maxVertex.x<-range)return false;
if(box.minVertex.x>range)return false;
if(box.maxVertex.y<-range)return false;
if(box.minVertex.y>range)return false;
return true;
}
bool BoundingBox::checkMinMax(const Face &f) 
{
//transformo el box a las coordenadas de face
BoundingBox box=(*this)/f.base;
if(box.maxVertex.z<0)return false;
if(box.minVertex.z>0)return false;
if(box.maxVertex.x<f.minV.x)return false;
if(box.minVertex.x>f.maxV.x)return false;
if(box.maxVertex.y<f.minV.y)return false;
if(box.minVertex.y>f.maxV.y)return false;
return true;
}
bool BoundingBox::checkMinMax(const Vector3D &p)
{
//simplemente ver si es interior
//alias
Vector3D &m=minVertex,&M=maxVertex;
if((p.x<m.x)||(p.x>M.x))return false;
if((p.y<m.y)||(p.y>M.y))return false;
if((p.z<m.z)||(p.z>M.z))return false;
return true;
}
bool BoundingBox::checkMinMax(const BoundingBox &b) const
{
//alias
const Vector3D &m=minVertex,&M=maxVertex;
const Vector3D &bm=b.minVertex, &bM=b.maxVertex;
//si ambas coordenadas son mayores o menores que el min o el max respect, no hay corte
if(m.x>bM.x)return false;
if(M.x<bm.x)return false;

if(m.y>bM.y)return false;
if(M.y<bm.y)return false;

if(m.z>bM.z)return false;
if(M.z<bm.z)return false;

return true;

}
void BoundingBox::drawGL()
{
	if(!validData)return;
Vector3D &m=minVertex,&M=maxVertex;
	GLTools::Color(ORANGE,0.55f);
	glDisable(GL_LIGHTING);


	glBegin(GL_LINE_STRIP);
		glVertex3f(m.x,m.y,m.z);
		glVertex3f(M.x,m.y,m.z);
		glVertex3f(M.x,m.y,M.z);
		glVertex3f(m.x,m.y,M.z);
	glEnd();
	glBegin(GL_LINE_STRIP);

		glVertex3f(M.x,M.y,M.z);
		glVertex3f(m.x,M.y,M.z);
		glVertex3f(m.x,M.y,m.z);
		glVertex3f(M.x,M.y,m.z);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(M.x,m.y,M.z);
		glVertex3f(M.x,M.y,M.z);
		glVertex3f(M.x,M.y,m.z);
		glVertex3f(M.x,m.y,m.z);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex3f(m.x,M.y,m.z);
		glVertex3f(m.x,m.y,m.z);
		glVertex3f(m.x,m.y,M.z);
		glVertex3f(m.x,M.y,M.z);
	glEnd();
	glEnable(GL_LIGHTING);
	glEnd();
}
void BoundingBox::includeBox(BoundingBox b){
	includePoint(b.maxVertex);
	includePoint(b.minVertex);
}
} //mr
