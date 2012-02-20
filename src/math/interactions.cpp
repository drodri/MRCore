

#include "face.h"
#include "segment3d.h"
#include "interactions.h"
#include "mrmath.h"
#include "trianglemesh.h"


namespace mr{

bool Interactions::faceSegment(Face &face, const Segment3D &s, Vector3D &p)
{
	
	Vector3D u=s.p2-s.p1;
	Vector3D w=s.p1-face.getOrigin();
	Vector3D n=face.getNormal();

    double     D = n*u;
    double    N = -(n*w);

    if (fabs(D) < EPS) {          // segment is parallel to plane
        if (N == 0)                     // segment lies in plane
			return true;
        else
            return false;                   // no intersection
    }
    // they are not parallel
    // compute intersect param
    double sI = N / D;
    if (sI < 0 || sI > 1)
        return false;                       // no intersection
    p = s.p1 + u*sI;                 // compute segment intersect point
	//transform to face coordinates
	Vector3D p2d=face.base.obtainRelativeCoordinates(p);
	return face.isInteriorPoint(Vector2D(p2d.x,p2d.y));

	
}
bool Interactions::faceRay(Face &face, Vector3D origin, Vector3D direction, double &distance)
{
	
	Vector3D &u=direction;
	Vector3D w=origin-face.getOrigin();
	Vector3D n=face.getNormal();

    double     D = n*u;
    double    N = -(n*w);

    if (fabs(D) < EPS) {          // ray is parallel to plane
		if (N == 0)	// ray lies in plane
			return false;
        else
            return false;                   // no intersection
    }
    // they are not parallel
    // compute intersect param
    distance = N / D;
    if (distance < 0 )
        return false;                       // no intersection
    // compute ray intersect point and
	//transform to face coordinates
	Vector3D p2d=face.base.obtainRelativeCoordinates(origin + u*distance);
	return face.isInteriorPoint(Vector2D(p2d.x,p2d.y));

	
}







}
