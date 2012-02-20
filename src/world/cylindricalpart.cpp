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

#include "cylindricalpart.h"
#include <fstream>
using namespace std;
namespace mr{

IMPLEMENT_MR_OBJECT(CylindricalPart)

void CylindricalPart::writeToStream(Stream& stream)
{
	PrismaticPart::writeToStream(stream);
}
void CylindricalPart::readFromStream(Stream& stream)
{
	PrismaticPart::readFromStream(stream);
}
ostream& operator<<(ostream& os, const CylindricalPart& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}



CylindricalPart::~CylindricalPart(void)
{

}

void CylindricalPart::drawGL()
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

		
		glBegin(GL_POLYGON);
		Vector3D up1=polygonalBase.getAbsoluteVertex(i)+Vector3D(0,0,height);
		Vector3D up2=polygonalBase.getAbsoluteVertex((i+1)%num)+Vector3D(0,0,height);
		Vector3D n1=(lateralNormals[i]+lateralNormals[(i-1+num)%num]).getUnitaryVector();
		Vector3D n2=(lateralNormals[i]+lateralNormals[(i+1)%num]).getUnitaryVector();
		glNormal3dv(n1.values);
		glVertex3dv(polygonalBase.getAbsoluteVertex(i).values);
		glVertex3dv(up1.values);
		glNormal3dv(n2.values);
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
}