/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio &Miguel Hernando Gutierrez
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

#include "meshpart.h"
#include <math.h>
#include <fstream>
using namespace std;
namespace mr{

IMPLEMENT_MR_OBJECT(MeshPart)

void MeshPart::writeToStream(Stream& stream)
{
	PrimitiveSolidEntity::writeToStream(stream);
	mesh.writeToStream(stream);
}
void MeshPart::readFromStream(Stream& stream)
{
	PrimitiveSolidEntity::readFromStream(stream);
	mesh.readFromStream(stream);
	//actualizaciones
	absoluteMesh=mesh;
	setMeshNeedToBeUpdated();
	createWiredModel();

}
void MeshPart::writeToXML(XMLElement* parent)
{
	PrimitiveSolidEntity::writeToXML(parent);
	mesh.writeToXML(parent);

}

void MeshPart::readFromXML(XMLElement* parent)
{
	PrimitiveSolidEntity::readFromXML(parent);
	mesh.readFromXML(parent);
	//actualizaciones
	absoluteMesh=mesh;
	setMeshNeedToBeUpdated();
	createWiredModel();
}

void MeshPart::createWiredModel()
{
	int i,num;
	num=(int)mesh.edges.size();
	wiredModel.clear();
	for(i=0;i<num;i++){
		wiredModel.push_back(Segment3D(mesh.vertex[mesh.edges[i].a],mesh.vertex[mesh.edges[i].b]));
	}
	setWiredModelNeedToBeUpdated();
}
ostream& operator<<(ostream& os, const MeshPart& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
MeshPart::MeshPart(void)
{

}



MeshPart::~MeshPart(void)
{

}
//the object position have been modified... therefore the absolute mesh
//have to be recomputed when needed
void  MeshPart::locationUpdated(){
//realizo las operaciones genericas necesarias por mover un solido
PrimitiveSolidEntity::locationUpdated();
//operaciones especificas:
//la posicion absoluta de los vertices no esta actualizada
setMeshNeedToBeUpdated();
}

//methods
void MeshPart::addTriangle(	Vector3D a,Vector3D b, Vector3D c)
{
	//agrego el triangulo al Triangle mesh
	mesh.addTriangle(a,b,c);
	//actualizo el box relativo
	box=mesh.getBoundingBox();
	//el box absoluto no esta calculado
	setBoxNeedToBeUpdated();
	//la posicion absoluta de los vertices no esta actualizada
	setMeshNeedToBeUpdated();
	//en el caso del mesh, por como funciona, actualizo el wiredmodel directamente añadiendo
	wiredModel.push_back(Segment3D(a,b));
	wiredModel.push_back(Segment3D(b,c));
	wiredModel.push_back(Segment3D(c,a));
	setWiredModelNeedToBeUpdated();

}
void MeshPart::updateMesh()
{
if(meshNeedToBeUpdated==false)return;
absoluteMesh=getAbsoluteT3D()*mesh;
meshNeedToBeUpdated=false;
}
void MeshPart::drawGL()
{
	PrimitiveSolidEntity::drawGL();
	material.loadMaterial();
	//en este caso hay que aplicar la transformacion y dibujar el mesh sin 
	//transformar... el mesh transformado es solo ncesario para calculos de
	//colision etc... en el dibujo no se recalcula
	glPushMatrix();
	location.transformGL();
	mesh.drawGL();
	glPopMatrix();
	
}
bool MeshPart::rayIntersection(Vector3D &ori, Vector3D &dir, double &dist)
{
	//chequeo min max
	if(!intersectable)return false;
	if(getAbsoluteBoundingBox().checkMinMax(ori,dir)==false)return false;
	//actualizo si es necesario la posición de la malla
	updateMesh();
	return absoluteMesh.rayIntersection(ori,dir,dist);
}

bool  MeshPart::segmentIntersection(const Segment3D &s,vector<Vector3D> *ipoints)
{
	//chequeo min max
	if(!intersectable)return false;
	if(getAbsoluteBoundingBox().checkMinMax(s)==false)return false;
	updateMesh();  
	Segment3D aux=s;
	return absoluteMesh.segmentIntersection(aux,ipoints);   
}
//Two structs only for reading the STL file
struct CVector3f 
{
	float x,y,z;
	CVector3f() : x(0),y(0),z(0) {}
	CVector3f(const float& X,const float& Y,const float& Z) : x(X),y(Y),z(Z) {}
};

struct CSTLRecord
{
	CVector3f n,a,b,c;
	char res[2];
};
bool MeshPart::loadAsciiSTL(string filename)
{
		// load the STL file
	try {
		ifstream file(filename.c_str(),ios::in);
		if( !file.is_open() )
			{
			//log error
			return false;
			}
		char buff[100];
		string data;
		file.getline(buff,99); //solid & name
		int end=0;

		while (!end && !file.eof())
		{
			//structura
			// facet normal ni nj nk
			// outer loop
			//	vertex v1x v1y v1z
			//vertex v2x v2y v2z
			//vertex v3x v3y v3z
			//endloop
			//endfacet
			Vector3D a,b,c,n;
			file>>data;
			if(data=="endsolid"){end=1;break;}
			if(data!="facet"){end=2;break;}
			file>>data;
			if(data!="normal"){end=2;break;}
			file>>n.x>>n.y>>n.z;
			file>>data>>data; //outer loop
			
			file>>data>>a.x>>a.y>>a.z; //vertex x,y,z
			file>>data>>b.x>>b.y>>b.z;
			file>>data>>c.x>>c.y>>c.z;
			file>>data;//endloop
			file>>data;//endfacet

			//verify that the triangle is defined as expected in mr:
			Vector3D db=b-a;
			Vector3D dc=c-a;
			Vector3D n2=db.cross(dc);
			if(n2*n>0)addTriangle(a,b,c);
			else addTriangle(a,c,b);

		}
		if(end==1){	file>>data; //name
			return true;
		}
		return false;
		
	}
	catch (...)
	{ 
		//std::cout << "Error while Loading STL file: " << filename.c_str() << endl;
		return false;
	}
	return true;
}
bool MeshPart::loadBinarySTL(string filename)
{
	// load the STL file
	try {
		ifstream file(filename.c_str(),ios::binary);
		if( !file.is_open() )
			{
			//log error
			return false;
			}
		char buf[80];
		file.read(buf,80);
		int num=0;
		file.read((char*)&num,4);
		CSTLRecord triangle;
		while (num-- && !file.eof())
		{
			file.read((char*)&triangle,50);
			// cast the vertex and normal data from float (STL-patch) to double (GL-patches)
			Vector3D n, a, b, c;
			n=Vector3D(triangle.n.x,triangle.n.y,triangle.n.z);
			a=Vector3D(triangle.a.x,triangle.a.y,triangle.a.z);
			b=Vector3D(triangle.b.x,triangle.b.y,triangle.b.z);
			c=Vector3D(triangle.c.x,triangle.c.y,triangle.c.z);
			//verify that the triangle is defined as expected in mr:
			Vector3D db=b-a;
			Vector3D dc=c-a;
			Vector3D n2=db.cross(dc);
			if(n2*n>0)addTriangle(a,b,c);
			else addTriangle(a,c,b);

		}
		
	}
	catch (...)
	{ 
		//std::cout << "Error while Loading STL file: " << filename.c_str() << endl;
		return false;
	}
	return true;
}
bool  MeshPart::saveAsMRcoreCppCode(string filename)
{
	//TODO: A funtion that writes in a file the C code that creates the current mesh.
	try 
	{
		ofstream file(filename.c_str());
		if( !file.is_open() )
			{
			//log error
			return false;
			}
		file<<"//Automatic code created by MeshPart, in order to code in mrcode"<<endl;
		file<<"//a mesh so it is posible to create it easily by code"<<endl;
		file<<"//It is recomended to change the name of the created generic function"<<endl;
		file<<"//just call: MeshPart *aux=thisfunction(); in your code"<<endl<<endl;
		file<<"MeshPart *meshFunction(void)"<<endl<<"{"<<endl;
		file<<"const int num="<<mesh.triangles.size()<<";"<<endl;
		file<<"const double v[][3]={"<<endl;
		for(int i=0;i<(int)mesh.triangles.size();i++){
			TriangleMesh::TMTriangle &t=mesh.triangles[i];
			Vector3D &a=mesh.vertex[t.a];
			Vector3D &b=mesh.vertex[t.b];
			Vector3D &c=mesh.vertex[t.c];
			file<<"{"<<a.x<<","<<a.y<<","<<a.z<<"},";
			file<<"{"<<b.x<<","<<b.y<<","<<b.z<<"},";
			file<<"{"<<c.x<<","<<c.y<<","<<c.z<<"}";
			if((i+1)==(int)mesh.triangles.size())file<<"};"<<endl;
			else file<<","<<endl;
		}
		file<<"MeshPart *aux=new MeshPart;"<<endl;
		file<<"for(int i=0;i<num;i++){"<<endl;
		file<<"\taux->addTriangle(Vector3D(v[i*3][0],v[i*3][1],v[i*3][2]),Vector3D(v[i*3+1][0],v[i*3+1][1],v[i*3+1][2]),"<<endl;
		file<<"\t                 Vector3D(v[i*3+2][0],v[i*3+2][1],v[i*3+2][2]));"<<endl;
		file<<"}"<<endl;
		file<<"return aux;"<<endl<<"}";

	}
	catch (...)
	{ 
		//std::cout << "Error while Loading STL file: " << filename.c_str() << endl;
		return false;
	}
	return true;
}
}//mr
