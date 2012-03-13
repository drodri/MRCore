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

#include "pointcloud.h"
#include "../gl/gltools.h"

#include <math.h>
#include <fstream>
#include <string>



namespace mr{

IMPLEMENT_MR_OBJECT(PointCloud);

PointCloud::PointCloud()
{

}
void PointCloud::writeToStream(Stream& stream)
{
	cout<<"Sending: "<<width<<" "<<height<<endl;
	stream<<width;
	stream<<height;
	for(unsigned int i=0;i<points.size();i++)
		stream<<points[i].x<<points[i].y<<points[i].z;

//	stream<<(int)color.size();
//	for(int i=0;i<color.size();i++)
//		stream<<color[i];
}
void PointCloud::readFromStream(Stream& stream)
{
	stream>>width;
	stream>>height;
	cout<<"W: "<<width<<" H: "<<height<<endl;
	points.resize(width*height);
	for(int i=0;i<width*height;i++)
		stream>>points[i].x>>points[i].y>>points[i].z;

//	int s=0;
//	stream>>s;
//	cout<<"COlor: "<<s<<endl;
//	color.resize(s);
//	for(int i=0;i<color.size();i++)
//		stream>>color[i];
}

void PointCloud::drawGL()
{
	glDisable(GL_LIGHTING);
	glPointSize(1);

	glBegin(GL_POINTS);
		for(unsigned int i=0;i<points.size();i++)
		{
			glVertex3f(points[i].x,points[i].y,points[i].z);
		}
	glEnd();
}



} //namepsace mr
