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
#include "plane.h"
#include "mrmath.h"
#include "../math/angle.h"
#include <math.h>
#include "gl/gltools.h"

namespace mr{
IMPLEMENT_MR_OBJECT(Plane)


ostream& operator<<(ostream& os, const Plane& p)
{
	os<<"Origin: "<<p.origin<<endl;
	os<<"Normal: "<<p.normal<<endl;
	return os;
}
Plane::Plane():normal(1,0,0)
{
}

Plane::Plane(Vector3D cg, Vector3D nor):origin(cg),normal(nor)
{
	normal.normalize();
}
Plane::~Plane()
{

}

Plane::Plane(Vector3D p1, Vector3D p2, Vector3D p3)
{
	Vector3D v1=p1-p2;
	Vector3D v2=p3-p2;
	
	Vector3D norm=v1.cross(v2);//v1 x v2
	normal=norm.normalize();

	origin=(p1+p2+p3)/3.0;
}
void Plane::writeToStream(Stream& stream)
{
	stream<<normal.x<<normal.y<<normal.z;
	stream<<origin.x<<origin.y<<origin.z;
}
void Plane::readFromStream(Stream& stream)
{
	stream>>normal.x>>normal.y>>normal.z;
	stream>>origin.x>>origin.y>>origin.z;
}
double Plane::distance(const Vector3D& point) const
{
	double a=normal.x;
	double b=normal.y;
	double c=normal.z;
	double d=-a*origin.x-b*origin.y-c*origin.z;
	
	double dist=a*point.x+b*point.y+c*point.z+d;
	
	//dist/=normal.Module();//The normal has to be Unitary
	
	return fabs(dist);
}
void Plane::LSQFit(const std::vector<Vector3D>& points, double* res)
{
/*	float sx=0;float sy=0;float sz=0;	
	
	float sx2=0;float sy2=0;float sz2=0;
	float sxy=0;float sxz=0;float syz=0;

	int num = points.size();

	int i;
	for (i=0;i<num;i++)
	{
		sx+=points[i].x;
		sy+=points[i].y;
		sz+=points[i].z;
	}

	origin.x=sx/num;
	origin.y=sy/num;
	origin.z=sz/num;
	
	for (i=0; i<num;i++)
	{
		sx2+=(points[i].x-origin.x)*(points[i].x-origin.x);
		sy2+=(points[i].y-origin.y)*(points[i].y-origin.y);
		sz2+=(points[i].z-origin.z)*(points[i].z-origin.z);
		sxy+=(points[i].x-origin.x)*(points[i].y-origin.y);
		sxz+=(points[i].x-origin.x)*(points[i].z-origin.z);
		syz+=(points[i].y-origin.y)*(points[i].z-origin.z);
	}

	Matrix aux(3,3);
	SymmetricMatrix A;
	Real c1[3]={sx2,sxy,sxz};
	Real c2[3]={sxy,sy2,syz};
	Real c3[3]={sxz,syz,sz2};

	aux.Column(1) << c1;
	aux.Column(2) << c2;
	aux.Column(3) << c3;

	A << aux;
	
	//find the eigenvalue closest to zero and its associated eigenvector etc

	DiagonalMatrix D;
	Matrix V;
	EigenValues(A,D,V);
	
	float min_eig=D(1,1);
	ColumnVector n(3);
	n<<V.Column(1);
	for (i=2;i<=3;i++)
	{
		if (D(i,i)<min_eig)
		{
			min_eig=D(i,i);
			n<<V.Column(i);
		}
	}
	
	normal=Vector3D(n(1),n(2),n(3));	
	float sense=origin*normal;
	if(sense>0)
		normal=Vector3D(-n(1),-n(2),-n(3));	
	
	float rs=0;
	float m=0;
	for (i=0;i<num;i++)
	{
		ColumnVector v1(3);
		Vector3D p1=points[i]-origin;
		Real v[3]={p1.x,p1.y,p1.z};
		v1 << v;
		float dist=DotProduct(v1,n);
		if (dist > m)
			m=dist;
		rs+=dist*dist;
	}


	//max_res=m;
	*res=rs;*/
}

void Plane::drawGL()
{
	//compute Transformation
//	HTrans3D t(*this);
		
	glPushMatrix();
	//	t.Draw();
		glTranslatef(origin.x,origin.y,origin.z);
		double roll = atan2(normal.y, normal.x);
		double pitch= acos(normal.z);
		glRotatef(roll*RAD2DEG,0,0,1);
		glRotatef(pitch*RAD2DEG,0,1,0);
		GLTools::DrawFrame();

		glEnable(GL_BLEND);			// Turn Blending On
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);	// Set The Blending Function For Translucency
			GLTools::Color(CYAN,0.55f);
			glEnable(GL_LIGHTING);
			glNormal3f(0,0,1);
			glBegin(GL_POLYGON);
				glVertex3f(-4,-4,0);
				glVertex3f(-4,4,0);
				glVertex3f(4,4,0);
				glVertex3f(4,-4,0);
			glEnd();

		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

	glPopMatrix();
}
} //mr
