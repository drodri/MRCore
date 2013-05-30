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
#ifndef __MRCORE__FACE_H
#define __MRCORE__FACE_H

#include "vector2d.h"
#include "vector3d.h"
#include "transformation3d.h"
#include "../base/globject.h"
#include "../gl/gltools.h"
#include <vector>

namespace mr{

/*!
    \class Face
	-A geometric face. It could be described by 2d coordinates, an a Trans3d
	location, assuming that Z axis is the normal vector pointing out of the face
	- It is also possible to define the face through a set of 3d points. Internally,
	it would be converted to the previous representation (a plane fitting thorugh LSQM 
	it is being carried out.
*/
class Face: public GLObject
{
	DECLARE_MR_OBJECT(Face)
	friend class Interactions;
	friend class BoundingBox;
//Attributes
protected:
	Transformation3D base;
	vector<Vector3D> vertex;//x,y plane coordinates,z=0
	vector<Vector3D> absVertex; //3d coordinates vertex aplying the base
	bool convex; //true if convex face
	bool clockwise; //true if clockwise
	Vector3D minV,maxV; //minimun and maximum coordinates computed. fastest tests
private:
	float r,g,b,alfa;
	static float defaultR,defaultG,defaultB,defaultAlfa;

public:
	friend ostream& operator<<(ostream& os, const Face& p);
//constructors
	Face();
	Face(const Transformation3D &inbase, const vector<Vector2D> &list_of_vertex);
//Rectangular Face
	Face(const Transformation3D &inbase, double x_min,double y_min, double x_max, double y_max);
	virtual ~Face(void);
//assignation
	Face& operator = (const Face& f){base=f.base; vertex=f.vertex; absVertex=f.absVertex;
					convex=f.convex;clockwise=f.clockwise; minV=f.minV; maxV=f.maxV;r=f.r;b=f.b;g=f.g;alfa=f.alfa;return *this;}
//copy constructor
	Face(const Face &f){(*this)=f;}
	void setColor(float _r, float _g,float _b,float _alfa){r=_r;g=_g;b=_b;alfa=_alfa;}
//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);//,int numface=0);
	virtual void readFromXML(XMLElement* parent);

	virtual void drawGL();
	void drawPrimitive(bool dir=true);
	void setBase(const Transformation3D &inbase);
	void updateData();
	void addVertex(double x, double y){vertex.push_back(Vector3D(x,y));
									absVertex.push_back(base*Vector3D(x,y));
									updateData();
	}
	

	void changeVertex (int ind, double x, double y)
	{								if (ind<0)return;
									if (ind>=(int)vertex.size())return;
									vertex[ind]=Vector3D(x,y);
									absVertex[ind]=base*Vector3D(x,y);
									updateData();
	}

	
	void deleteVertex (int ind)
	{								if (ind<0)return;
									if (ind>=(int)vertex.size())return;
									if (vertex.empty() || absVertex.empty())return;
									vertex.erase(vertex.begin()+ind);
									absVertex.erase(absVertex.begin()+ind);
									updateData();
																		
	}




	bool isInteriorPoint(Vector2D p);
	bool isClockwise(){return clockwise;}
	void setOrigin(Vector3D& ori){base.position=ori;baseUpdated();}
	void baseUpdated(); //compute the absolutevertexes
	inline Vector3D getNormal(){return base.getVectorW();}
	inline Vector3D getOrigin(){return base.position;}
	inline Transformation3D getBase(){return base;}
	inline Vector3D getMaxVertex(){return maxV;}
	inline Vector3D getMinVertex(){return minV;}

	int getNumVertex(){return (int)(vertex.size());}
	Vector3D getAbsoluteVertex(int i){return (absVertex[i]);}
	
	static bool enableAlfa;
	static void setDefaultColor(float _r, float _g,float _b,float _alfa){defaultR=_r;defaultG=_g;defaultB=_b;defaultAlfa=_alfa;}
};

}

#endif  //__MRCORE__FACE_H
