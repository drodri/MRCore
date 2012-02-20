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
#include "orientationmatrix.h"
#include "mrmath.h"

namespace mr{
OrientationMatrix::OrientationMatrix(Axis axis,double ang):Matrix3x3(1)
{
	double ca=cos(ang);
	double sa=sin(ang);
	if(axis==X_AXIS)//ang es alfa
	{
		mat[1][1]=ca;
		mat[1][2]=-sa;
		mat[2][1]=sa;
		mat[2][2]=ca;
	}
	if(axis==Y_AXIS)//ang es fi
	{
		mat[0][0]=ca;
		mat[0][2]=sa;
		mat[2][0]=-sa;
		mat[2][2]=ca;
	}
	if(axis==Z_AXIS)//ang es theta
	{
		mat[0][0]=ca;
		mat[0][1]=-sa;
		mat[1][0]=sa;
		mat[1][1]=ca;
	}
}
OrientationMatrix &OrientationMatrix::setRPY(double rx, double ry, double rz)
{
	double sx,cx,sy,cy,sz,cz;
	sx=sin(rx);cx=cos(rx);
	sy=sin(ry);cy=cos(ry);
	sz=sin(rz);cz=cos(rz);
	mat[0][0]=cz*cy;	mat[0][1]=cz*sy*sx-sz*cx;	mat[0][2]=cz*sy*cx+sz*sx;
	mat[1][0]=sz*cy;	mat[1][1]=sz*sy*sx+cz*cx;	mat[1][2]=sz*sy*cx-cz*sx;
	mat[2][0]=-sy;		mat[2][1]=cy*sx;			mat[2][2]=cy*cx;
	return (*this);
}
void OrientationMatrix::getRPY(double& rx, double& ry, double& rz)
{
	rz = atan2(mat[1][0],mat[0][0]);
	ry = atan2(-mat[2][0], cos(rz)*mat[0][0] + sin(rz)*mat[1][0]);
	rx = atan2(sin(rz)*mat[0][2]-cos(rz)*mat[1][2], -sin(rz)*mat[0][1]+cos(rz)*mat[1][1]);
}



OrientationMatrix::OrientationMatrix(double roll, double pitch, double yaw)
{
	setRPY(roll, pitch, yaw);
}
//obtains the orientation matrix based on the unitary vector x, 
//the vector vv corrspondent to y. z is cw defined, and vv corrected
OrientationMatrix::OrientationMatrix(Vector3D vu, Vector3D vv)
{
	vu.normalize();
	Vector3D vw=vu.cross(vv);
	vw.normalize();
	vv=vw.cross(vu);
	mat[0][0]=vu[0];mat[1][0]=vu[1];mat[2][0]=vu[2];
	mat[0][1]=vv[0];mat[1][1]=vv[1];mat[2][1]=vv[2];
	mat[0][2]=vw[0];mat[1][2]=vw[1];mat[2][2]=vw[2];
}
} //mr