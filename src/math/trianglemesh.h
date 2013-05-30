/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  Miguel Hernando & Diego Rodríguez-Losada
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
#ifndef __MRCORE__TRIANGLEMESH_H
#define __MRCORE__TRIANGLEMESH_H

#include "vector2d.h"
#include "vector3d.h"
#include "boundingbox.h"
#include "transformation3d.h"
#include "../gl/gltools.h"
#include "../base/globject.h"
#include <vector>

namespace mr{

/*!
    \class TriangleMesh
	-A geometric TriangleMesh. is a set of triangles. It is supossed that a lot of edges and
	vertexes are shared among triangles. It is used mainly as a general geometric
	modeling utility. Almost anything could be aproximated by this models. References are done using index 
	so it is posible to easily copy and transmit the object without further transformations and
	verifications.
*/
class TriangleMesh: public GLObject
{
	DECLARE_MR_OBJECT(TriangleMesh)
	friend class Interactions;
	friend class MeshPart;
	struct TMEdge{
		int a; //vertex index of both
		int b;
	};
	struct TMTriangle{
		Vector3D normal;
		int a; //vertex index for the 3
		int b; //vertrxes of the face
		int c;
		int ea; //index of the edges
		int eb;
		int ec;
	};
//Attributes
protected:
	
	vector<Vector3D> vertex;//vertex coordinates
	vector<TMEdge> edges; //two vertex index
	vector<TMTriangle> triangles; //three vertex index and three edges indexes
	BoundingBox box; //minimun and maximum coordinates computed. fastest tests

	//glspecific for making the painting faster
	bool generateGLlist;
	GLint listGL;

	int addVertex(const Vector3D &v); //a vertex is added only if it is different. the function returns the vector index
	int addEdge(int a, int b);
	bool segmentTriangleIntersection(const TMTriangle &t,  Segment3D &s,vector<Vector3D> *ipoints);
	bool rayTriangleIntersection(const TMTriangle &t, Vector3D &ori, Vector3D &dir, double &dist);
public:
	friend ostream& operator<<(ostream& os, const TriangleMesh& p);
//constructors
	TriangleMesh();

	virtual ~TriangleMesh(void);
//assignation
	TriangleMesh& operator = (const TriangleMesh& f){vertex=f.vertex; edges=f.edges;
			triangles=f.triangles;box=f.box;return *this;listGL=0; generateGLlist=true;}
//copy constructor
	TriangleMesh(const TriangleMesh &f){(*this)=f;}
//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	virtual void drawGL();
	bool addTriangle(Vector3D a,Vector3D b, Vector3D c);//normal vector is computed throug the cross product cw
	//special operator that obtains the TriangleMesh transformed by the T reference system
	friend TriangleMesh operator*(Transformation3D T, TriangleMesh& b);
	BoundingBox getTransformedBoundingBox(Transformation3D T){return BoundingBox(T*box);}
	//intersection operators. are placed here beacause always a mesh object have to be previously created 
	//(no inline construction is possible)TMTriangle
	BoundingBox getBoundingBox(){return box;}
	bool segmentIntersection(Segment3D &s,vector<Vector3D> *ipoints);
	bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist);
};

}

#endif  //__MRCORE__FACE_H
