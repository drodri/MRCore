/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Alberto Valero
 * Parts taken from: OpenRDK: http://openrdk.sf.net
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
 *     by the author ori licensor (but not in any way that suggests that
 *     they endorse you ori your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, ori build upon this work,
 *     you may distribute the resulting work only under the same ori
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs ori
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY ori FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/


#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <iostream>
#include "mrmath.h"

#define M_2PI   (2*M_PI)
#define DEG2RAD (M_PI/180.0)
#define RAD2DEG (180.0/M_PI)

using namespace std;

namespace mr{

inline double deg2rad(double d ) { return d * DEG2RAD; }
inline double rad2deg(double d ) { return d * RAD2DEG; }


class Angle
{


public:
	friend ostream& operator<<(ostream& os, const Angle& a){os<<a.value;return os;} 
	friend inline double sin(const Angle& a);
	friend inline double cos(const Angle& a);

	Angle(double ang = 0);
	virtual ~Angle(void);

	Angle operator -(const Angle& ang) const;
	Angle operator +(const Angle& ang) const;
	bool operator < (const Angle& ang) const;
	bool operator > (const Angle& ang) const;
	bool operator <= (const Angle& ang) const;
	bool operator >= (const Angle& ang) const;

	void setValue(double v);
	inline double getValue() const {return value;}

	//wraps an angle in 0,2PI
	static double makePositive(double angle);
	static double makePositive(Angle a){return makePositive(a.getValue());}
	static int getQuadrant(double a);
	static int getQuadrant(Angle a){return getQuadrant(a.getValue());}
protected:
//angles normlized in -PI, PI
   double value;
   double Normalize();

   inline void updTrig() const{if(!trig){cos_theta=::cos(value);sin_theta=::sin(value);trig=true;}}
	mutable double cos_theta;//for efficiency
	mutable double sin_theta;//for efficiency
	mutable bool trig;
};


   /** Returns an angle that is 0 <= angle < 2 * PI */
	inline double angNorm2PiUnsig(double ang)
	{
		
		double r = std::fmod(ang, 2 * M_PI);
		if (r < 0) r = 2 * M_PI + r;
		return r;
	}
	
	/** returns an angle that is -PI <= angle <= PI */
	inline double angNormPiSig(double ang)
	{
		
		double r = std::fmod(ang, 2 * M_PI);
		if (r > M_PI) r = -(2 * M_PI - r);
		else if (r < -M_PI) r = 2 * M_PI + r;
		return r;
	}

	inline double angDiff(double ang1, double ang2) { 
		return angNormPiSig(angNorm2PiUnsig(ang1) - angNorm2PiUnsig(ang2)); 
	}
	double sin(const Angle& a){a.updTrig();return a.sin_theta;}
	double cos(const Angle& a){a.updTrig();return a.cos_theta;}

	inline double angleWeightedMean(double Angle1, double Angle2, double alpha)
	{
		return atan2((1-alpha)*sin(Angle1)+alpha*sin(Angle2),(1-alpha)*cos(Angle1)+alpha*cos(Angle2));
	}
	
}	



