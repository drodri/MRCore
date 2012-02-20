/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Miguel Hernando & Diego Rodríguez-Losada & Alberto Valero
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



#ifndef __MRCORE__MRMATH_H
#define __MRCORE__MRMATH_H

#pragma once
/*!
	General definitions, includes, basic types    
*/
//standar includes
#define _USE_MATH_DEFINES
#include <math.h>
#include <utility>

//#include <cmath>

using namespace std;

namespace mr{

//Constants
#define PI (M_PI)
#define EPS (0.000001)

template <class X> inline int sgn(X d) { return d<0?-1:1; }
template<class X> inline X square(const X& x) { return x*x; }
template<class X> inline X Abs(const X& x){ return (sgn(x)*x); }
template<class X> inline X Max(const X& x, const X&y){ return x>y?x:y; }
template<class X> inline X Min(const X& x, const X&y){ return x<y?x:y; }

inline pair<double,double> solve_quadratic(double a, double b, double c){
	pair<double,double> result;
	double squareroot_result = sqrt(b*b -4*a*c);
	double aa = 2*a;
	result.first = (-b + squareroot_result) / (aa);
	result.second = (-b - squareroot_result) / (aa);
	return result;
}

}//mr

#endif
