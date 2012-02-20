/**********************************************************************
 *
 * This code is part of the MRcore project
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

#ifndef MRCORE_TRANSFORMATION2D_H
#define MRCORE_TRANSFORMATION2D_H

#include "vector2d.h"
#include "angle.h"
#include "../base/object.h"

namespace mr {

/**
	Beware that the operators + - * uses also the "theta" component.

*/

class Transformation2D : public Object {

	///Text stream serializers (files)
	friend ostream& operator << (ostream& os,const Transformation2D& scan);

	DECLARE_MR_OBJECT(Transformation2D)

	
public:
	Angle theta;
	double x;
	double y;

	//Basic useful accessors
	/// Generates a point2 from the x,y components. 
	Vector2D position() const { return Vector2D(x,y); }
	double angle() const {return theta.getValue();}
	
/// @name Constructors
//@{
	Transformation2D(double xv=0, double yv=0, double th=0) : x(xv), y(yv), theta(th) { }
	Transformation2D(double xv, double yv, const Angle& th) :  x(xv), y(yv), theta(th) { }
	explicit Transformation2D(const Vector2D& p, double th=0) : x(p.x), y(p.y), theta(th) { }
	Transformation2D(const Vector2D& p, const Angle& th) : x(p.x), y(p.y), theta(th) { }
//@}
	
/// @name Operators
//@{
	/// Subtracts two vectors
	Transformation2D  operator- (const Transformation2D&  p) const { 
		return Transformation2D(x-p.x,y-p.y,theta-p.theta); 
	}
	/// Adds two vectors 
	Transformation2D operator+ (const Transformation2D&  p) const { 
		return Transformation2D(x+p.x,y+p.y,theta+p.theta); 
	}

	//Composition operator
	Transformation2D operator* (const Transformation2D& p) const
	{	
		Transformation2D pos;
		pos.x=		x	+	p.x*cos(theta)-p.y*sin(theta);
		pos.y=		y	+	p.x*sin(theta)+p.y*cos(theta);
		pos.theta=	theta+	p.theta; 	
		return pos;
	}
	///Unary Inversion operator
	Transformation2D operator-() const
	{
		Transformation2D pos;
		pos.x= -x*cos(theta)-y*sin(theta);
		pos.y= x*sin(theta)-y*cos(theta);
		pos.theta= Angle(0)-theta;
		return pos;
	}
	///****************ALBERTO PARA TI*****************************************
	/// Note: USE algebra of transformations for change of bases
	///For example, changing a point from local reference to global is just
	///Vector2D local;
	///Transformation2D reference;
	///Vector2D global=reference*local;
	///
	///For example, changing a point from global reference to local is 
	///Vector2D global;
	///Transformation2D reference;
	///Vector2D local= (-reference)*global;

	Vector2D operator* (const Vector2D& p) const
	{	
		Vector2D pos;
		pos.x=		x	+	p.x*cos(theta)-p.y*sin(theta);
		pos.y=		y	+	p.x*sin(theta)+p.y*cos(theta);	
		return pos;
	}



	/// Rotate this vector of @param thetar radians around point @param p
/*	Point2o rotate(double thetar, const Vector2D& p = Vector2D(0, 0)) const 
	{
		double x = this->x, y = this->y;
		x -= p.x;
		y -= p.y;
		double newx = (double) x  * cos(thetar) - y * sin(thetar);
		double newy = (double) x  * sin(thetar) + y * cos(thetar);
		double newt = theta + thetar;
		newx += p.x;
		newy += p.y;
		return Point2o((double) newx, (double) newy, newt);
	}

	/// Change the reference system of the point, @param localReferenceSystem is the origin
	/// of the local reference system in global coordinates (theta points towards x axis)
	Point2o toLocalReferenceSystem(const Point2o& localReferenceSystem) const
	{
		Vector2D tempp(this->x - localReferenceSystem.x, this->y - localReferenceSystem.y);
		tempp = tempp.rot(-localReferenceSystem.theta);
		return Point2o(tempp.x, tempp.y, this->theta - localReferenceSystem.theta);
	}

	/// Change back to global reference system, @param localReferenceSystem is the origin
	/// of the local reference system in global coordinates (theta points towards x axis)
	Point2o toGlobalReferenceSystem(const Point2o& localReferenceSystem) const
	{
		Vector2D tempp(this->x, this->y);
		tempp = tempp.rot(localReferenceSystem.theta);
		tempp.x = tempp.x + localReferenceSystem.x;
		tempp.y = tempp.y + localReferenceSystem.y;
		return Point2o(tempp.x, tempp.y, this->theta + localReferenceSystem.theta);
	}
	
	

	/*inline Point2o weightedMean(const Point2o& p1, const Point2o& p2, double alpha)
	{
		double beta=1-alpha;
		double x = (double)(beta*p1.x+alpha*p2.x);
		double y = (double)(beta*p1.y+alpha*p2.y);
		double theta = angleWeightedMean(p1.theta,p2.theta,alpha);
		return Point2o(x,y,theta);
	}*/

//@}

		
	
	void transformGL();//<OpenGL draw
	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);	
};

typedef Transformation2D Pose;
typedef Transformation2D Point2o;
inline double distance(const Point2o& point1, const Point2o& point2)
{
	Point2o aux = point2 - point1;
	Vector2D vector(aux.x, aux.y);
	return vector.module();
}
} // ns

#endif
/*inline Point2 toLocalReferenceSystem(const Point2& point, const Point2o& localReferenceSystem)
{
	Point2 tempp(point.x - localReferenceSystem.x, point.y - localReferenceSystem.y);
	tempp = tempp.rot(-localReferenceSystem.theta);
	return tempp;
}

inline Point2o toLocalReferenceSystem(const Point2o& point, const Point2o& localReferenceSystem)
{
	Point2 tempp(point.x - localReferenceSystem.x, point.y - localReferenceSystem.y);
	tempp = tempp.rot(-localReferenceSystem.theta);
	return Point2o(tempp.x, tempp.y, point.theta - localReferenceSystem.theta);
}

inline Point2 toGlobalReferenceSystem(const Point2& point, const Point2o& localReferenceSystem)
{
	Point2 tempp(point.x, point.y);
	tempp = tempp.rot(localReferenceSystem.theta);
	tempp.x = tempp.x + localReferenceSystem.x;
	tempp.y = tempp.y + localReferenceSystem.y;
	return tempp;
}

inline Point2o toGlobalReferenceSystem(const Point2o& point, const Point2o& localReferenceSystem)
{
	Point2 tempp(point.x, point.y);
	tempp = tempp.rot(localReferenceSystem.theta);
	tempp.x = tempp.x + localReferenceSystem.x;
	tempp.y = tempp.y + localReferenceSystem.y;
	return Point2o(tempp.x, tempp.y, point.theta + localReferenceSystem.theta);
}*/
