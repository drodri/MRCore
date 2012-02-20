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

#include "segment.h"

namespace mr {

// Sedgewick's algorithm (segments intersection)
int sedgewick_ccw(const Point2& p0, const Point2& p1, const Point2& p2)
{
	double dx1 = p1.x - p0.x;
	double dy1 = p1.y - p0.y;
	double dx2 = p2.x - p0.x; 
	double dy2 = p2.y - p0.y;
	if (dx1*dy2 > dy1*dx2) return +1;
	if (dx1*dy2 < dy1*dx2) return -1;
	if ((dx1*dx2 < 0) || (dy1*dy2 < 0)) return -1;
	if ((dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2)) return +1;
	return 0;
}

// Sedgewick's algorithm (segments intersection)
bool sedgewickIntersect(const Point2& s0p0, const Point2& s0p1, const Point2& s1p0, const Point2& s1p1)
{
	return ((sedgewick_ccw(s0p0, s0p1, s1p0) * sedgewick_ccw(s0p0, s0p1, s1p1)) <= 0)
	    && ((sedgewick_ccw(s1p0, s1p1, s0p0) * sedgewick_ccw(s1p0, s1p1, s0p1)) <= 0);
}

} // namespaces
