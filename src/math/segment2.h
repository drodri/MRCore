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

#ifndef MRCORE_SEGMENT2_H
#define MRCORE_SEGMENT2_H

#include <string>
#include <sstream>
#include <assert.h>
#include <cmath>

#include "point.h"

namespace mr {

using std::sqrt;
using std::cos;
using std::sin;

/**
 * A generic vector with two components. 
 *
 * Note: by design, none of the methods changes the values of the members.
 *
 * @author  Andrea Censi  <andrea.censi@dis.uniroma1.it>
 * @author Daniele Calisi <calisi@dis.uniroma1.it>
 * @author Alberto Ingenito (serialize/deserialize) <alberto.ing@gmail.com>
*/

template <typename Numeric> 
struct Segment2 {
	Point2 p0;
	Point2 p1;

	Segment2() : p0(0, 0), p1(0, 0) { }
	Segment2(Point2 p0, Point2 p1) : p0(p0), p1(p1) { }
	Segment2(Numeric p0x, Numeric p0y, Numeric p1x, Numeric p1y) : p0(p0x, p0y), p1(p1x, p1y) { }

}; // template Point2

} // ns

#endif
