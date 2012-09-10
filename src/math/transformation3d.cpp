/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  --------------
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
#include "transformation3d.h"

#include <math.h>
#include "../gl/gltools.h"
namespace mr{

ostream& operator<<(ostream& os, const Transformation3D& p)
{
	os<<p.position.x<<" "<<p.position.y<<" "<<p.position.z<<endl;
	return os;
}

void Transformation3D::writeToStream(Stream& stream)
{
	stream<<orientation[0][0]<<orientation[0][1]<<orientation[0][2];
	stream<<orientation[1][0]<<orientation[1][1]<<orientation[1][2];
	stream<<orientation[2][0]<<orientation[2][1]<<orientation[2][2];
	stream<<position[0]<<position[1]<<position[2];
}
void Transformation3D::readFromStream(Stream& stream)
{
	Matrix3x3 &m=orientation;
	stream>>m[0][0]>>m[0][1]>>m[0][2];
	stream>>m[1][0]>>m[1][1]>>m[1][2];
	stream>>m[2][0]>>m[2][1]>>m[2][2];
	stream>>position[0]>>position[1]>>position[2];
}

void Transformation3D::transformGL()
{
	//OpenGL can directly manage Tranformation matrices
	double matrix[16];

	//Transpose required for OpenGL
	for(int i=0;i<3;i++)
	{
		matrix[i*4]=orientation[0][i];
		matrix[i*4+1]=orientation[1][i];
		matrix[i*4+2]=orientation[2][i];
		matrix[i*4+3]=0;
	}
	matrix[12]=position.x;
	matrix[13]=position.y;
	matrix[14]=position.z;
	matrix[15]=1;

	glMultMatrixd(matrix);
}
void Transformation3D::drawGL()
{
	glPushMatrix();
	transformGL();
	GLTools::DrawFrame();
	glPopMatrix();
}
Transformation3D Transformation3D::inverted() const
{
	Transformation3D ret;
	ret.orientation=orientation.inverted();
	Vector3D pos=(ret.orientation*position)*(-1.0);
	ret.position=pos;
	return ret;
}
} //mr
