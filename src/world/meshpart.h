/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
 * 
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

#ifndef __MRCORE__MESHPART__H
#define __MRCORE__MESHPART__H


#include "primitivesolidentity.h"
#include "../math/trianglemesh.h"


using namespace std;

namespace mr
{
/*!
    \class MeshPart
    MeshPart	->	A generic but not optimized class for implementing a part. Geometry is
	described through a set of triangles (triangle mesh). 
	It is useful because, any non articulated part can be modeled at the end as a triangle mesh 

	DESIGN INFO:
	It is a interesting class in order to see haw a new solid primitive have to be implemented
	*/

class MeshPart: public PrimitiveSolidEntity
{
	DECLARE_MR_OBJECT(MeshPart)
	
	/**Text output**/
	friend ostream& operator<<(ostream& os, const MeshPart& p);
protected:
	//invariant triangle mesh 
	TriangleMesh mesh;
	//triangle mesh computed in absolute coordinates
	TriangleMesh absoluteMesh;

	bool meshNeedToBeUpdated;
public:
//attributes
	
//constructors
	//Basic constructor
	MeshPart(void);

	//Destructor
	virtual ~MeshPart(void);

	//operator =
	const MeshPart &operator=(const MeshPart &v){
		(*((PrimitiveSolidEntity *)this))=v;
		mesh=v.mesh;
		absoluteMesh=v.absoluteMesh;
		return (*this);
	}
	MeshPart(const MeshPart &v){
		(*this)=v;
	}

//overwrittn virtual methods
	virtual void locationUpdated();

//methods
	void setMeshNeedToBeUpdated(){meshNeedToBeUpdated=true;}
	void updateMesh();
	void addTriangle(Vector3D a,Vector3D b, Vector3D c);
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	void drawGL();
//specific operations that have to be implemented due to its solid entity condition
	bool segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints);
	bool rayIntersection(Vector3D &ori, Vector3D &dir, double &dist); 
//specific operations due to its primitive condition
	void createWiredModel();
//read a STL file and converts it to a meshpart
	bool loadBinarySTL(string filename);
	bool loadAsciiSTL(string filename);
	bool saveAsMRcoreCppCode(string filename);

};

};
#endif  //__MRCORE__MESHPART__H
