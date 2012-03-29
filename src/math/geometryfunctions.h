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

#ifndef GEOMETRYFUNCTIONS_H
#define GEOMETRYFUNCTIONS_H

#include <cmath>
#include <vector>

#include "transformation2d.h"
#include "segment3d.h"
#include "vector2d.h"
#include "segment2d.h"

namespace mr {

using namespace std;

/** Line fitting: doesn't use error (=0) yet  */
/*void regression(const std::vector<Vector2D>& points, double&theta, double&rho, double&error);	

Vector2D projectPointOntoLine(const Vector2D& p, const Segment2D& segm);
Vector2D projectPointOntoSegment(const Vector2D& p, const Segment2D& segm);

double pointDistanceToLine(const Vector2D& p, const Segment2D& segm);

bool segmentIntersection(const Segment2D& segm1, const Segment2D& segm2, Vector2D* intersection);
bool lineIntersection(const Segment2D& line1, const Segment2D& line2, Vector2D* intersection);

vector<Segment2D> recursiveLineFitting(const vector<Vector2D>& points, double maxErrorAllowed);

*/


/*
inline Vector2D vectorFromTo(const Transformation2D& point1, const Transformation2D& Vector2D){
	Vector2D aux = Vector2D - point1;
	Vector2D vector(aux.x, aux.y);
	return vector;
}*/


}// ns

#endif
