/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  mrcore developers
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
#ifndef __MRCORE__VECTOR2D_H
#define __MRCORE__VECTOR2D_H

#include <iostream>
#include <math.h>
#include <cmath>
#include "angle.h"


using namespace std;
//using std::cos;
//using std::sin;
//using std::min;
//using std::max;

namespace mr
{
/*!
    \class Vector2D
    \brief Vector2D - a basic mathematical class for 2 dimensional vectors.
*/
class Vector2D{

	/**Text output**/
	friend ostream& operator<<(ostream& os, const Vector2D& p);

public:
	union{
		double values[2];
		struct{double x,y;};
	};

	//constructors
	Vector2D(double xi=0.0f, double yi=0.0f){
			x=xi;y=yi;
	}
	Vector2D(double *valuesi){
			x=valuesi[0];y=valuesi[1];
	}

	inline void setPolarCoords(double p, double t){
		x= p*cos(t); y=p*sin(t);
	}

	inline void setPolarCoords(double p, Angle t){
		setPolarCoords(p,t.getValue());
	}

	//operators
	inline Vector2D operator - (const Vector2D& p2) const
	{
		return Vector2D(x-p2.x,y-p2.y);
	}

	//operators
	

	inline Vector2D operator + (const Vector2D& p2) const
	{
		return Vector2D(x+p2.x,y+p2.y);
	}

	///product by scalar
	inline Vector2D operator * (const int& v) const
	{
		return Vector2D(v*x,v*y);
	}

	inline Vector2D operator *(double f) const
	{
		return Vector2D(x*f,y*f);
	}
	///division by scalar
	inline Vector2D operator /(double f) const
	{
		return Vector2D(x/f,y/f);
	}
	inline Vector2D &operator= (const Vector2D &vin)
	{
		x=vin.x;y=vin.y;
		return *this;
	}
	inline double & operator[](int i) //not safe but friendly
	{
	return values[i];
	}


	/** Subtracts two vectors, unary version . */
	Vector2D&  operator-= (const Vector2D&  p) { x -= p.x; y -= p.y; return *this; }

	/** Adds two vectors, unary version. */
	Vector2D&  operator+= (const Vector2D&  p) { x += p.x; y += p.y; return *this; }

	/** Scalar product. */
	double operator* (const Vector2D&  p) const { return    x * p.x + y * p.y;     }


/// Methods

	/** Return the argument (angle with X axis) of the vector */
	double argument() const { return atan2(y,x); }

	/** Returns L2-norm of vector.*/
	double module()  const { return sqrt(module2());  }

	/** Returns L2-norm of vector, squared. */
	double module2() const { return x * x + y * y; }

	/** Returns the squared distance between this point and p. */
	double distance2(const Vector2D& p) const { return ((*this) - p).module2(); }

	/** Returns the distance between this point and p. */
	double distance(const Vector2D& p) const { return ((*this) - p).module(); }
	static inline double distance(const Vector2D& point1, const Vector2D& Vector2D)
	{
		return point1.distance(Vector2D);
	}
	/** Returns a vector perpendicular to this, with the same module. */
	Vector2D perpendicularVector() const { return Vector2D(y,-x); }

	/** Returns a versor (= norm 1) parallel to this. */
//	Vector2D vers() const { return vers(theta()); }



	/** Rotate this vector of  @param thetar radians around center @param center. */
	Vector2D rot(double thetar, const Vector2D& center = Vector2D(0, 0)) const
	{
		double newx = (double) (this->x - center.x) * cos(thetar) - (this->y - center.y) * sin(thetar) + center.x;
		double newy = (double) (this->x - center.x) * sin(thetar) + (this->y - center.y) * cos(thetar) + center.y;
		return Vector2D((double) newx, (double) newy);
	}


	/// Static methods

//	/** Returns a versor of the specified angle*/
//	static Vector2D vers(double theta)  {
//		return Vector2D(cos(theta), sin(theta));
//	}
//
//	/** Returns a versor with the minima of the components of @a and @b.*/
//	static Vector2D min(const Vector2D&a, const Vector2D&b)  {
//		return Vector2D( std::min(a.x,b.x), std::min(a.y,b.y));
//	}
//
//	/** Returns a versor with the maxima of the components of @a and @b.*/
//	static Vector2D max(const Vector2D&a, const Vector2D&b)  {
//		return Vector2D( std::max(a.x,b.x), std::max(a.y,b.y));
//	}

	Vector2D &normalize();
	Vector2D getUnitaryVector() const;
}; 



}
#endif  //__MRCORE__VECTOR2D_H
