/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:   Diego Rodriguez-Losada
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
#ifndef __MRCORE__PATH3D_H
#define __MRCORE__PATH3D_H

#include <iostream>
#include "base/globject.h"
#include "math/transformation3d.h"

using namespace std;

namespace mr{
		
class Path3D : public GLObject {
	
	///Text stream serializers (files)
	friend ostream& operator << (ostream& os,const Path3D& path);
	DECLARE_MR_OBJECT(Path3D)
	
public:
	
	Path3D(){;}
	Path3D(vector<Vector3D> path){points = path;}
	virtual ~Path3D(){}
	
	void drawGL();
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);

	inline void clear(){points.clear();}
	inline void push_back(Vector3D p){points.push_back(p);}
	inline unsigned int size(){return points.size();}
	inline bool empty(){return points.empty();}
	Vector3D& operator[](int i){return points[i];}

protected:
	vector<Vector3D> points;


};


}//end namespace

#endif
