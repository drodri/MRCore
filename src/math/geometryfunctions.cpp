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
#include "geometryfunctions.h"

#include <string>
#include <set>
using namespace std;

namespace mr {
void regression(const std::vector<Point2>& points, double&theta, double&rho, double&error)
{
	int n = points.size(); assert(n);
		double mu_x = 0;
		double mu_y = 0;
		for(int a=0;a<n;a++) {
			mu_x += points[a].x / n;
			mu_y += points[a].y / n;
		}
		double s_x2 = 0;
		double s_y2 = 0;
		double s_xy = 0;
		for(int a=0;a<n;a++) {
			s_x2 += square(points[a].x-mu_x);
			s_y2 += square(points[a].y-mu_y);
			s_xy += (points[a].y-mu_y)*(points[a].x-mu_x);
		}
		
		theta = 0.5 * atan2(-2*s_xy, s_y2-s_x2);
		rho = mu_x * cos(theta) + mu_y * sin(theta);
		if(rho>0) {
			rho = - rho;
			theta = theta + M_PI;
		}
		error = 0;
}
Point2 projectPointOntoLineOrSegment(const Point2& p, const Segment2d& segm, bool itIsALine)
{
	double vx = segm.p1.x - segm.p0.x, vy = segm.p1.y - segm.p0.y;
	double wx = p.x - segm.p0.x, wy = p.y - segm.p0.y;
	double c1 = (wx*vx + wy*vy);
	if (!itIsALine && c1 <= 0) return segm.p0;
	else {
		double c2 = (vx*vx + vy*vy);
		if (!itIsALine && c2 <= c1) return segm.p1;
		double b = c1 / c2;
		double bx = segm.p0.x + b*vx;
		double by = segm.p0.y + b*vy;
		return Point2(bx, by);
	}
}

Point2 projectPointOntoLine(const Point2& p, const Segment2d& segm)
{
	return projectPointOntoLineOrSegment(p, segm, true);
}

Point2 projectPointOntoSegment(const Point2& p, const Segment2d& segm)
{
	return projectPointOntoLineOrSegment(p, segm, false);
}

double pointDistanceToLine(const Point2& p, const Segment2d& segm)
{
	Point2 projectedPoint = projectPointOntoLine(p, segm);
	return p.distanceTo(projectedPoint);
}

double pointDistanceToSegment(const Point2& p, const Segment2d& segm)
{
	Point2 projectedPoint = projectPointOntoSegment(p, segm);
	return p.distanceTo(projectedPoint);
}

vector<Segment2d> recursiveLineFitting(const vector<Point2>& points, double maxErrorAllowed)
{
	set<int> endPoints;
	endPoints.insert(0);
	endPoints.insert(points.size() - 1);
	double maxDist = 0;
	do {
		int maxDistIndex = -1;
		maxDist = 0;
		set<int>::iterator it = endPoints.begin();
		set<int>::iterator it2 = ++endPoints.begin();
		for (; it2 != endPoints.end(); ++it, ++it2) {
			int i0 = *it;
			int i1 = *it2;
			Segment2d s;
			s.p0 = points[i0];
			s.p1 = points[i1];
			for (int j = i0 + 1; j < i1; j++) {
				double dist = pointDistanceToLine(points[j], s);
				if (dist > maxDist) {
					maxDist = dist;
					maxDistIndex = j;
				}
			}
		}
		if (maxDist > maxErrorAllowed) {
			endPoints.insert(maxDistIndex);
		}
	} while (maxDist > maxErrorAllowed);

	vector<Segment2d> retval;
	set<int>::iterator it = endPoints.begin();
	set<int>::iterator it2 = ++endPoints.begin();
	for (; it2 != endPoints.end(); ++it, ++it2) {
		Segment2d s;
		s.p0 = points[*it];
		s.p1 = points[*it2];
		retval.push_back(s);
	}
	return retval;
}

//  public domain function by Darel Rex Finley, 2006
//  Determines the intersection point of the line segment defined by points A and B
//  with the line segment defined by points C and D.
//
//  Returns YES if the intersection point was found, and stores that point in X,Y.
//  Returns NO if there is no determinable intersection point, in which case X,Y will
//  be unmodified.
bool segmentIntersection(const Segment2d& segm1, const Segment2d& segm2, Point2* intersection)
{
	double Ax = segm1.p0.x;	double Ay = segm1.p0.y;
	double Bx = segm1.p1.x;	double By = segm1.p1.y;
	double Cx = segm2.p0.x;	double Cy = segm2.p0.y;
	double Dx = segm2.p1.x;	double Dy = segm2.p1.y;

  double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line segment is zero-length.
  if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return false;

  //  Fail if the segments share an end-point.
  if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy)
  ||  (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy)) {
    return false; }

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if segment C-D doesn't cross line A-B.
  if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.)) return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  Fail if segment C-D crosses line A-B outside of segment A-B.
  if (ABpos<0. || ABpos>distAB) return false;

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  if (intersection) {
	  intersection->x=Ax+ABpos*theCos;
	  intersection->y=Ay+ABpos*theSin;
  }

  //  Success.
  return true;
}

//  public domain function by Darel Rex Finley, 2006



//  Determines the intersection point of the line defined by points A and B with the
//  line defined by points C and D.
//
//  Returns true if the intersection point was found, and stores that point in X,Y.
//  Returns false if there is no determinable intersection point, in which case X,Y will
//  be unmodified.

bool lineIntersection(const Segment2d& line1, const Segment2d& line2, Point2* intersection)
{
	double Ax = line1.p0.x;	double Ay = line1.p0.y;
	double Bx = line1.p1.x;	double By = line1.p1.y;
	double Cx = line2.p0.x;	double Cy = line2.p0.y;
	double Dx = line2.p1.x;	double Dy = line2.p1.y;

  double  distAB, theCos, theSin, newX, ABpos ;

  //  Fail if either line is undefined.
  if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return false;

  //  (1) Translate the system so that point A is on the origin.
  Bx-=Ax; By-=Ay;
  Cx-=Ax; Cy-=Ay;
  Dx-=Ax; Dy-=Ay;

  //  Discover the length of segment A-B.
  distAB=sqrt(Bx*Bx+By*By);

  //  (2) Rotate the system so that point B is on the positive X axis.
  theCos=Bx/distAB;
  theSin=By/distAB;
  newX=Cx*theCos+Cy*theSin;
  Cy  =Cy*theCos-Cx*theSin; Cx=newX;
  newX=Dx*theCos+Dy*theSin;
  Dy  =Dy*theCos-Dx*theSin; Dx=newX;

  //  Fail if the lines are parallel.
  if (Cy==Dy) return false;

  //  (3) Discover the position of the intersection point along line A-B.
  ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);

  //  (4) Apply the discovered position to line A-B in the original coordinate system.
  if (intersection) {
	  intersection->x=Ax+ABpos*theCos;
	  intersection->y=Ay+ABpos*theSin;
  }

  //  Success.
  return true; 
}

} // ns
