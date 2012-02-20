/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Miguel Hernando & Diego Rodr�guez-Losada
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
#include "trianglemesh.h"
#include "mrmath.h"
#include <math.h>


namespace mr{
IMPLEMENT_MR_OBJECT(TriangleMesh)

ostream& operator<<(ostream& os, const TriangleMesh& tm)
{
	/*os<<"Origin: "<<face.origin<<endl;
	os<<"Normal: "<<face.normal<<endl;*/
	return os;
}
TriangleMesh::TriangleMesh()
{
listGL=0; generateGLlist=true;
}


TriangleMesh::~TriangleMesh()
{

}


void TriangleMesh::writeToStream(Stream& stream)
{
	int i,num=(int)vertex.size();
	stream<<num;
	for(i=0;i<num;i++)stream<<vertex[i].x<<vertex[i].y<<vertex[i].z;

	num=(int)edges.size();
	stream<<num;
	for(i=0;i<num;i++)stream<<edges[i].a<<edges[i].b;

	num=(int)triangles.size();
	stream<<num;
	for(i=0;i<num;i++)
	{
		stream<<triangles[i].a<<triangles[i].b<<triangles[i].c;
		stream<<triangles[i].ea<<triangles[i].eb<<triangles[i].ec;
		stream<<triangles[i].normal.x<<triangles[i].normal.y<<triangles[i].normal.z;
	}
	box.writeToStream(stream);
}
void TriangleMesh::readFromStream(Stream& stream)
{
	int i,num;
	
	Vector3D vaux;
	stream>>num;
	for(i=0;i<num;i++){
		stream>>vaux.x>>vaux.y>>vaux.z;
		vertex.push_back(vaux);
	}
	TMEdge eaux;
	stream>>num;
	for(i=0;i<num;i++){
		stream>>eaux.a>>eaux.b;
		edges.push_back(eaux);
	}

	TMTriangle taux;
	stream>>num;
	for(i=0;i<num;i++)
	{
		stream>>taux.a>>taux.b>>taux.c;
		stream>>taux.ea>>taux.eb>>taux.ec;
		stream>>taux.normal.x>>taux.normal.y>>taux.normal.z;
		triangles.push_back(taux);
	}
	box.readFromStream(stream);
}
int TriangleMesh::addVertex(const Vector3D &v) 
{
int i;
for(i=0;i<(int)(vertex.size());i++){
	if(vertex[i]==v)return i;
	}
vertex.push_back(v);
return i;
}
int TriangleMesh::addEdge(int a, int b)
{
int i;
for(i=0;i<(int)(edges.size());i++){
	if((edges[i].a==a)&&(edges[i].b==b))return i;
	if((edges[i].b==a)&&(edges[i].a==b))return i;
	}
TMEdge e;
e.a=a;
e.b=b;
edges.push_back(e);
return i;
}
bool TriangleMesh::addTriangle(Vector3D a,Vector3D b, Vector3D c){
//compute normal and cheq if it a valid triangle:
	Vector3D db=b-a;
	Vector3D dc=c-a;
	Vector3D n=db.cross(dc);
	if(n.module()<EPS)return false;
	n.normalize();
	TMTriangle t;
	t.a=addVertex(a);
	t.b=addVertex(b);
	t.c=addVertex(c);
	t.ea=addEdge(t.a,t.b);
	t.eb=addEdge(t.b,t.c);
	t.ec=addEdge(t.c,t.a);
	t.normal=n;
	triangles.push_back(t);
	//update boundingbox
	box.includePoint(a);
	box.includePoint(b);
	box.includePoint(c);
	generateGLlist=true;
	return true;

}
TriangleMesh operator*(Transformation3D T, TriangleMesh& b)
{
	TriangleMesh ret=b; //make a copy
	int i;
	for(i=0;i<(int)(b.vertex.size());i++){
		ret.vertex[i]=T*(b.vertex[i]);
	}
	for(i=0;i<(int)(b.triangles.size());i++){
		ret.triangles[i].normal=(T.orientation)*((b.triangles[i]).normal);
	}
	ret.box=b.getTransformedBoundingBox(T);
	return ret;
}

void TriangleMesh::drawGL()
{
	int i;
	//test only
/*	GLTools::Color(YELLOW);
	glEnable(GL_LIGHTING);*/

	if((generateGLlist)||(glIsList(listGL)==GL_FALSE)){
		//if listGL !=0 clear that list

		if((listGL!=0)&&(glIsList(listGL)==GL_TRUE))glDeleteLists(listGL,1);
		listGL=0;
		//look for a new list
		listGL=glGenLists(1);
		glNewList(listGL,GL_COMPILE);

		glBegin(GL_TRIANGLES);
		for(i=0;i<(int)(triangles.size());i++){
			TMTriangle &t=triangles[i];
			glNormal3dv(t.normal.values);
			glVertex3dv(vertex[t.a].values);
			glVertex3dv(vertex[t.b].values);
			glVertex3dv(vertex[t.c].values);
		}
		glEnd();
		glEndList();

		generateGLlist=false;
	}
	glCallList(listGL);

	//test
	//box.drawGL();
}

//basic intersections
bool TriangleMesh::segmentTriangleIntersection(const TMTriangle &t, Segment3D &s,vector<Vector3D> *ipoints)
{
	
	double dist;
	Vector3D aux=s.p2-s.p1;
	if(rayTriangleIntersection(t, s.p1, aux, dist)){
		if(dist>1.0)return false;
		if(ipoints)ipoints->push_back(s.p1+(s.p2-s.p1)*dist);
		return true;
		}
	return false;
}
bool TriangleMesh::rayTriangleIntersection(const TMTriangle &tr, Vector3D &ori, Vector3D &dir, double &dist)
{
	Vector3D &ta=vertex[tr.a], &tb=vertex[tr.b],&tc=vertex[tr.c],n=tr.normal;
	Vector3D u=(tb-ta);
	Vector3D v=(tc-ta);
               
    Vector3D w0 = ori - ta; //trianglo or to ray or
	double a = -(n*w0);
    double b = n*dir;
    if (fabs(b) < EPS) {     // ray is parallel to triangle plane
        if (a == 0)                // ray lies in triangle plane
            return false;
        else return false;             // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    dist = a / b;
    if (dist < 0.0) return false ;                 // ray goes away from triangle
    // for a segment, also test if (r > 1.0) => no intersect
	
	Vector3D I=ori+dir*dist; // intersect point of ray and plane
    // is I inside T?
    double    uu, uv, vv,wu, wv, D;

    uu = u*u;
    uv = u*v;
    vv = v*v;
    Vector3D w = I - ta;
    wu = w*u;
    wv = w*v;
    D = uv * uv - uu * vv;

    // get and test parametric coords
    double s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)  return false;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  return false;
        
	return true;
}
bool TriangleMesh::segmentIntersection(Segment3D &s,vector<Vector3D> *ipoints)
{
	
int i;
bool flag=false;
if(box.checkMinMax(s)==false)return false;
	for(i=0;i<(int)(triangles.size());i++){
		if(segmentTriangleIntersection(triangles[i],s,ipoints))flag=true;
		if((ipoints==0)&&(flag))return true;
	}
return flag;
}
bool TriangleMesh::rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)
{
double dist2;
int i;
bool flag=false;
if(box.checkMinMax(ori,dir)==false)return false;
	for(i=0;i<(int)(triangles.size());i++){
		if(rayTriangleIntersection(triangles[i],ori,dir,dist2)){
			if(flag)dist=dist2<dist?dist2:dist;
			else {flag=true; dist=dist2;}
		}
	}
return flag;
}



} //mr
