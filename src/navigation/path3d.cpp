/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Diego Rodriguez-Losada
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
 
#include "path3d.h"
#include "gl/gltools.h"

namespace mr{
	
IMPLEMENT_MR_OBJECT(Path3D)

ostream& operator << (ostream& os,const Path3D& path){
	os<<path.points.size()<<" ";
	for (unsigned int i=0;i<path.points.size();i++){
		os<<" "<<path.points[i].x<<" "<<path.points[i].y<<" "<<path.points[i].z;
	}
	os << endl;
	return os;
}

void Path3D::writeToStream(Stream& stream){
	GLObject::writeToStream(stream);
	int size=points.size();
	stream<<size;
	for (unsigned int i=0;i<points.size();i++){
		points[i].writeToStream(stream);//<<points[i].x<<points[i].y<<points[i].z;		
	}
}

void Path3D::readFromStream(Stream& is){
	GLObject::readFromStream(is);
	int size; is >> size;
	points.resize(size);
	for (int i=0;i<size;i++){
		points[i].readFromStream(is);
	}
}

void Path3D::drawGL(){
	glDisable(GL_LIGHTING);
	glColor3ub(r,g,b);
	
	glBegin(GL_LINE_STRIP);
	for (unsigned int i=0; i<points.size(); i++){
		glVertex3f(points[i].x,points[i].y,points[i].z);
	}
	glEnd();
}


}//end namespace
