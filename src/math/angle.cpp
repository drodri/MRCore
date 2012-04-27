/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Diego Rodriguez-Losada, Alberto Valero
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
 
#include "angle.h"

namespace mr{

Angle::Angle(double ang)
{
    trig = false;
    value = ang;
    Normalize();
}

Angle::~Angle()
{

}

void Angle::setValue(double v)
{
	 trig = false;
    value = v;
    Normalize();
}

double Angle::Normalize()
{
	/*while (value > M_PI)
	{
		value-=2*M_PI;
	}
	while (value < -M_PI)
	{
		value+=2*M_PI;
	}*/

    int n;
    if (value >= 0)
        n = (int)floor(value/M_PI);
    else
        n = (int)ceil(value/M_PI);

	value -= n*2*M_PI;

	return value;
}
double Angle::makePositive(double angle)
{
	while (angle > 2*PI){
		angle-=2*PI;
	}	
	while (angle <=0){
		angle+=2*PI;
	}
	return angle;
}
double Angle::difference(double a1,double a2)
{
	return (Angle(a1)-Angle(a2)).getValue();
}
int Angle::getQuadrant(double a){
	double angle=Angle::makePositive(a);
	if (angle < PI/2) return 1;
	else if (angle < PI) return 2;
	else if (angle < 2*PI/3) return 3;
	else return 4;
}
Angle Angle::operator-(const Angle& ang) const
{
   double ang_value = ang.value;
	double err=value-ang_value;
	if(fabs(err)>M_PI)
	{
		if(err>0)err-=(2*M_PI);
		else err+=(2*M_PI);
	}

	Angle ret(err);
   return ret;
}


Angle Angle::operator+(const Angle& ang) const
{
   double ang_value = ang.value;
	double angle = value + ang_value;
	Angle ret(angle);  //includes normalization
	return ret;


}

bool Angle::operator<(const Angle& ang) const
{
	return (value < ang.value);

}

bool Angle::operator>(const Angle& ang) const
{
    return (value > ang.value);

}

bool Angle::operator<=(const Angle& ang) const
{
	return (value <= ang.value);

}

bool Angle::operator>=(const Angle& ang) const
{
    return (value >= ang.value);

}
double Angle::average(vector<double> angles)
{
		double x=0.0,y=0.0;//ax=-PI,min=PI;
		for(unsigned int i=0;i<angles.size();i++)
		{
			x+=cos(angles[i]);
			y+=sin(angles[i]);
		}
		
		double ret = atan2(y, x);
		
		if (ret != ret)
			cout << " x " << x << " y " << y << endl;
		
		return atan2(y, x);
}

}
