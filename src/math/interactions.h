/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
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

#ifndef __MRCORE__INTERACTIONS__H
#define __MRCORE__INTERACTIONS__H






namespace mr{
class Face;
class Segment3D;
class Vector3D;

class Interactions 
{

public:
// 
// name: faceSegment
// @param const Face &face: reference to the face object
// @param const Segment3D &s: reference to the Segment3D object
// @param Point3D &p: the function returns the point where the intersection takes place if any.
// @return true if intersects.
static bool faceSegment(Face &face, const Segment3D &s, Vector3D &p);
// name: faceRay
// @param const Face &face: reference to the face object
// @param Vector3D origin: source point
// @param Vector3D direction: the direction and metric vector (usually is a unitary vector) .
// @return true if intersects, distance would include the distance to the face plane if it is
//posible to compute it (including negative values or the case of no intersection into the face).
static bool faceRay(Face &face, Vector3D origin, Vector3D direction, double &distance);

};


}



#endif
