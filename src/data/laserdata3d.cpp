/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
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

#include "laserdata3d.h"
#include "gl/gltools.h"
#include "math/mrmath.h"

#include <math.h>
#include <fstream>
#include <string>



namespace mr{

IMPLEMENT_MR_OBJECT(LaserData3D);

LaserData3D::LaserData3D()
{
	drawGLMode=0;
}
void LaserData3D::writeToStream(Stream& stream)
{
	Data::writeToStream(stream);
	if(laserdata.size()!=transf.size())return;

	stream<<(int)laserdata.size();
	for(int i=0;i<(int)laserdata.size();i++)
		laserdata[i].writeToStream(stream);
	stream<<(int)transf.size();
	for(int i=0;i<(int)transf.size();i++)
		transf[i].writeToStream(stream);
}
void LaserData3D::readFromStream(Stream& stream)
{
	Data::readFromStream(stream);

	int n;
	stream>>n;
	laserdata.resize(n);
	for(int i=0;i<(int)laserdata.size();i++)
		laserdata[i].readFromStream(stream);

	stream>>n;
	transf.resize(n);
	for(int i=0;i<(int)transf.size();i++)
		transf[i].readFromStream(stream);

	if(laserdata.size()!=transf.size())
	{
		laserdata.clear();
		transf.clear();
	}
}

void LaserData3D::drawGL()
{
	if(laserdata.size()!=transf.size())return;
	for(int i=0;i<(int)transf.size();i++)
	{
		glPushMatrix();
		transf[i].transformGL();
		laserdata[i].drawGLMode=drawGLMode;
		laserdata[i].drawGL();
		glPopMatrix();
	}
}


}
