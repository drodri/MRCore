#pragma once

#include <iostream>
#include "transformation2d.h"
#include "../base/globject.h"
#include "gl/gltools.h"

using namespace std;

namespace mr{

class Segment2D 
{
	friend class Interactions;
public:
//constructors
	Segment2D(){;}
	Segment2D(const Vector2D& v1,const Vector2D& v2):p1(v1),p2(v2){;}
	virtual ~Segment2D(void){;}
	
	double distance(const Vector2D& p) const
	{
		Vector2D v1=p-p1;//(s.source,p);
		Vector2D v2=p2-p1;//(s.source,s.target);
		double lambda=(v1*v2)/(v2*v2);
		if(lambda>=1)
			return p.distance(p2);//squaredDistance(p,target);
		if(lambda<=0)
			return p.distance(p1);

		Vector2D proj=p1+v2*lambda;//projection point

		return p.distance(proj);
	}

	Vector2D p1;
	Vector2D p2;///<orientation p1->p2

};

}




