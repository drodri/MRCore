/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Alberto Valero
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
 
#include "path2d.h"
#include "config.h"
#include "gl/gltools.h"

namespace mr{
	
IMPLEMENT_MR_OBJECT(Path2D)

ostream& operator << (ostream& os,const Path2D& path){
	os<<path.points.size()<<" ";
	for (int i=0;i<path.points.size();i++){
		os<<" "<<path.points[i].x<<" "<<path.points[i].y;
	}
	os << endl;
	return os;
}

void Path2D::writeToStream(Stream& stream){
	int size=points.size();
	stream<<size;
	for (int i=0;i<points.size();i++){
		stream<<points[i].x<<points[i].y;
	}
}

void Path2D::readFromStream(Stream& is){
	points.clear();
	int size; is >> size;
	points.resize(size);
	for (int i=0;i<size;i++){
		int x; is >> x;
		int y; is >> y;
		points[i]=Point2(x,y);
	}
}

void Path2D::drawGL(){
	
	glColor3i(rgb[0],rgb[1],rgb[2]);
	
	glBegin(GL_LINES);
	for (int i=0; i<points.size(); i++){
		glVertex2f(points[i].x,points[i].y);
	}
	glEnd();
	
}


}//end namespace
