#pragma once

#include <iostream>
#include "transformation3d.h"
#include "../base/globject.h"

using namespace std;

namespace mr{

class Segment3D : public GLObject
{
	DECLARE_MR_OBJECT(Segment3D)
	friend class Interactions;
public:
//constructors
	Segment3D(){;}
	Segment3D(const Vector3D& v1,const Vector3D& v2):p1(v1),p2(v2){;}
	virtual ~Segment3D(void){;}
	
	virtual void writeToStream(Stream& stream){}
	virtual void readFromStream(Stream& stream){}

	virtual void drawGL();


	Vector3D p1;
	Vector3D p2;///<orientation p1->p2
//inline operators	
	friend Segment3D operator*(Transformation3D &t, Segment3D& s){
		return Segment3D(t*s.p1,t*s.p2);
	}
};

}




