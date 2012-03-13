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
	inline double distance(const Vector3D &p)
	{
		Vector3D aux1=p2-p1;
		Vector3D aux2=p-p1;
		double norma=aux1*aux1;
		double esc=aux1*aux2;
		if(esc<=0)return sqrt(aux2*aux2);
		if(esc>norma)return (p-p2).module();
		double module=sqrt(norma);
		esc/=module;
		return sqrt(aux2*aux2-esc*esc);
	}
};

}




