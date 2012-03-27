/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Miguel Hernando, Diego Rodríguez-Losada, Paloma de la Puente,
 *   Alberto Valero, Pablo Sansegundo, Luis Pedraza.
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
#ifndef __MRCORE__VECTOR3D_H
#define __MRCORE__VECTOR3D_H

#include <iostream>
#include <cmath>
#include <math.h>
#include "base/stream.h"

using namespace std;

namespace mr
{
/*!
    \class Vector3D
    \brief Vector3D - a basic mathematical class for three dimensional vectors.
*/
class Vector3D{

	/**Text output**/
	friend ostream& operator<<(ostream& os, const Vector3D& p);

public:
//attributes. Due to the nature of this class, all the atributes are public
	union{
		double values[3];
		struct{double x,y,z;};
	};
 

//constructors
	Vector3D(const double xi=0.0f, const double yi=0.0f, const double zi=0.0f){
			x=xi;y=yi;z=zi;
	}
	Vector3D(const double *valuesi){
			x=valuesi[0];y=valuesi[1];z=valuesi[2];
	}


//operators
	inline Vector3D operator - (const Vector3D& p2) const 
	{
		return Vector3D(x-p2.x,y-p2.y,z-p2.z);
	}

	inline Vector3D operator + (const Vector3D& p2) const
	{
		return Vector3D(x+p2.x,y+p2.y,z+p2.z);
	}
	inline void operator += (const Vector3D& p2) 
	{
		x=x+p2.x;y=y+p2.y;z=z+p2.z;
	}
	///Escalar product
	inline double operator *(const Vector3D& p2) const
	{
		return (x*p2.x+y*p2.y+z*p2.z);
	}	
	///Cross product
	inline Vector3D cross(const Vector3D& w) const
	{
		return Vector3D(y*w.z - z*w.y,z*w.x - x*w.z,x*w.y - y*w.x);
	}	
	///product by scalar
	inline Vector3D operator *(double f) const
	{
		return Vector3D(x*f,y*f,z*f);;
	}
	///division by scalar
	inline Vector3D operator /(double f) const
	{
		return Vector3D(x/f,y/f,z/f);
	}
	inline Vector3D &operator= (const Vector3D &vin)
	{
		x=vin.x;y=vin.y;z=vin.z;
		return *this;
	}
	inline double & operator[](int i) //not safe but friendly
	{
	return values[i];
	}
	inline bool operator==(const Vector3D &vin){
		if((x==vin.x)&&(y==vin.y)&&(z==vin.z))return true;
		return false;
	}
//methods
	double module() const {return sqrt(x*x+y*y+z*z);}
	Vector3D &normalize();
	Vector3D getUnitaryVector() const;

	

	//Punto3D is a typedef of Vector3D
	inline double distanceTo(const Vector3D& p) const { return ((*this) - p).module(); }
	
	//Serializers
	void writeToStream(Stream& stream){stream<<x<<y<<z;}
	void readFromStream(Stream& stream){stream>>x>>y>>z;}
};

}
#endif  //__MRCORE__VECTOR3D_H
