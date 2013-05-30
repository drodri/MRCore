/**********************************************************************
 *
 * This code is part of the MRcore project
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
#include "face.h"
#include <math.h>
#define MR_TESS (void (APIENTRY *)(void))
GLdouble vaux[3];
namespace mr{
IMPLEMENT_MR_OBJECT(Face)

bool Face::enableAlfa=true;
float Face::defaultR=1.0f;
float Face::defaultG=1.0f;
float Face::defaultB=1.0f;
float Face::defaultAlfa=1.0f;

ostream& operator<<(ostream& os, const Face& face)
{
	/*os<<"Origin: "<<face.origin<<endl;
	os<<"Normal: "<<face.normal<<endl;*/
	return os;
}
Face::Face()
{
convex=true;
r=defaultR;
g=defaultG;
b=defaultB;
alfa=defaultAlfa;
}

Face::Face(const Transformation3D &inbase, const vector<Vector2D> &list_of_vertex )
{
setBase(inbase);
for(unsigned int i=0;i<list_of_vertex.size();i++){
	vertex.push_back(Vector3D(list_of_vertex[i].x,list_of_vertex[i].y));
	}
absVertex=vertex;
updateData();
baseUpdated();
r=defaultR;
g=defaultG;
b=defaultB;
alfa=defaultAlfa;
}

Face::Face(const Transformation3D &inbase, double x_min,double y_min, double x_max, double y_max)
{
setBase(inbase);
vertex.push_back(Vector3D(x_min,y_min));
vertex.push_back(Vector3D(x_max,y_min));
vertex.push_back(Vector3D(x_max,y_max));
vertex.push_back(Vector3D(x_min,y_max));
absVertex=vertex;
updateData();
baseUpdated();
r=defaultR;
g=defaultG;
b=defaultB;
alfa=defaultAlfa;
}

void Face::setBase(const Transformation3D &inbase)
{
base=inbase;
baseUpdated();
}
Face::~Face()
{
}
void Face::baseUpdated()
{
int num=(int)(vertex.size());
for(int i=0;i<num;i++)absVertex[i]=base*vertex[i];
}

void Face::writeToStream(Stream& stream)
{
	int num=(int)(vertex.size());
    base.writeToStream(stream);
	stream<<num;
	for(int i=0;i<num;i++)stream<<vertex[i].x<<vertex[i].y;
	/*stream<<normal.x<<normal.y<<normal.z;
	stream<<origin.x<<origin.y<<origin.z;*/

	stream<<r<<g<<b<<alfa;
}
void Face::readFromStream(Stream& stream)
{
	int num;
	double x,y;

	base.readFromStream(stream);
	stream>>num;
	for(int i=0;i<num;i++){
		stream>>x>>y;
		addVertex(x,y);
	}
	stream>>r>>g>>b>>alfa;
//add vertex updates the remainer internal data
}
void Face::writeToXML(XMLElement* parent)//,int numface)
{
		int num=(int)(vertex.size());
		//XMLAux aux;

		XMLElement* fac=new XMLElement(parent,"face");
		XMLElement* vert=new XMLElement(parent,"vertex");
		XMLContent* c;

		for(int i=0;i<num;i++)
		{
			c=new XMLContent(vert,-1,Vector3D :: vector3DToString(vertex[i]).c_str());
			vert->AddContent(c,0);

		}

		parent->AddElement(fac);
		fac->AddElement(vert);
		base.writeToXML(fac);


		//XMLElement* atrib=new XMLElement(fac,"Attributes");
		//XMLVariable* re= new XMLVariable("r",aux.string_Convert<float>(r).c_str());
		//XMLVariable* gr= new XMLVariable("g",aux.string_Convert<float>(g).c_str());
		//XMLVariable* bl= new XMLVariable("b",aux.string_Convert<float>(b).c_str());
		//XMLVariable* al= new XMLVariable("alfa",aux.string_Convert<float>(alfa).c_str());
		//fac->AddElement(atrib);
		//atrib->AddVariable(re);
		//atrib->AddVariable(gr);
		//atrib->AddVariable(bl);
		//atrib->AddVariable(al);


}

void Face::readFromXML(XMLElement* parent)
{

		XMLElement* fac;
		char check_parent[50]={0};
		parent->GetParent()->GetElementName(check_parent);

		if(parent->FindElementZ("face"))
			fac=parent->FindElementZ("face");

		else if(string(check_parent)=="FaceSetPart")
			fac=parent;

			if (fac->FindElementZ("vertex"))
			{


				int num=fac->FindElementZ("vertex")->GetContentsNum();

				if(num)
				{

					XMLContent** c=fac->FindElementZ("vertex")->GetContents();
					string cad;//,type="vertex";
					for (int i=0;i<num;i++)
					{
						cad=string();
						cad.resize(c[i]->GetSize());
						c[i]->GetValue((char*)cad.c_str());
						vector<Vector3D> aux_vec=Vector3D::stringToVectorVector3D(cad);
						for (int j=0;j<aux_vec.size();j++)
						{
							addVertex(aux_vec[j].x,aux_vec[j].y);
						}

					}
				 }



		}

			//if(fac->FindElementZ("Attributes"))
			//{
			//	XMLElement* atrib=fac->FindElementZ("Attributes");
			//	if(atrib->FindVariableZ("r"))
			//		r=atrib->FindVariableZ("r")->GetValueFloat();
			//	if(atrib->FindVariableZ("g"))
			//		g=atrib->FindVariableZ("g")->GetValueFloat();
			//	if(atrib->FindVariableZ("b"))
			//		b=atrib->FindVariableZ("b")->GetValueFloat();
			//	if(atrib->FindVariableZ("alfa"))
			//		alfa=atrib->FindVariableZ("alfa")->GetValueFloat();
			//}

		base.readFromXML(fac);
}

void Face::updateData()
{
int sign,signini,i;
double sum=0;
Vector3D v1,v2;
int num=(int)(vertex.size());

if(num<3)return;

convex=true;
v1=(vertex[0]-vertex[num-1]).normalize();
v2=(vertex[num-1]-vertex[num-2]).normalize();

if((v1[0]*v2[1]-v1[1]*v2[0])>0.0F)sign=1;
		else sign=-1;
signini=sign;
minV=maxV=vertex[0];
/*a sign change imply non-convexity*/
sum=sign*acos(v2*v1);
for(i=1;i<num;i++)
	{
	v2=v1;
	v1=(vertex[i]-vertex[i-1]).normalize();
	if((v1[0]*v2[1]-v1[1]*v2[0])>0.0F)sign=1;
		else sign=-1;
	if(signini!=sign)convex=false;
	sum+=sign*acos(v2*v1);
//min max computing
	if(vertex[i].x<minV.x)minV.x=vertex[i].x;
	else if(vertex[i].x>maxV.x)maxV.x=vertex[i].x;
	if(vertex[i].y<minV.y)minV.y=vertex[i].y;
	else if(vertex[i].y>maxV.y)maxV.y=vertex[i].y;
	}
if(sum>0.0F)clockwise=true; 
	   else clockwise=false;

return ;
}
//only draws the GL polygon face without normals and colors
//dir implyes if it is in inverse or direct order (false==inverse)
void Face::drawPrimitive(bool dir)
{
	int num=(int)(vertex.size());
	int i;

	if(convex){
		glBegin(GL_POLYGON);
		if(clockwise==dir){
			for(i=0;i<num;i++)
				glVertex3dv(vertex[i].values);
		}
		else{
			for(i=num-1;i>=0;i--)
				glVertex3dv(vertex[i].values);
		}
		glEnd();
	}
	else{
		GLUtesselator *TESS=gluNewTess();
		gluTessCallback(TESS,GLU_BEGIN,MR_TESS glBegin);
		gluTessCallback(TESS,GLU_VERTEX,MR_TESS glVertex3dv);
		gluTessCallback(TESS,GLU_END, glEnd);
		
		gluTessBeginPolygon(TESS, NULL);
		gluTessBeginContour(TESS);

		if(clockwise==dir){
			for(i=0;i<num;i++)gluTessVertex(TESS,vertex[i].values,vertex[i].values);		
		}
		else{ 		
			for(i=num-1;i>=0;i--)gluTessVertex(TESS,vertex[i].values,vertex[i].values);
		}
		gluTessEndContour(TESS);
		gluTessEndPolygon(TESS);
		gluDeleteTess(TESS);
	}
}
void Face::drawGL()
{
	//compute Transformation
	int i,num;
	
	glPushMatrix();
	base.transformGL();
	//GLTools::DrawFrame();

	num=(int)(vertex.size());
	if(num<3){
		glPopMatrix();
		return;
		}
	if(enableAlfa)
	{
		glEnable(GL_BLEND);			// Turn Blending On
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);	// Set The Blending Function For Translucency
	}
	glColor4f(r,g,b,alfa);
//	GLTools::Color(RED,0.55f);
	glEnable(GL_LIGHTING);
	glNormal3d(0,0,1.0);
		
	drawPrimitive();		

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	//GLTools::Color(CYAN,1.0f);
	glColor4f(r,g,b,1);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<num;i++)glVertex3f(vertex[i].x,vertex[i].y,0);
	glVertex3f(vertex[0].x,vertex[0].y,0);
	glEnd();
	
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
//initially no special cases are considered, but it is important to
//detect the cases in wich the auxiliar line intersect vertexes
//it is more eficient if is onsidered the convexity or not-convexity: to be done
bool Face::isInteriorPoint(Vector2D p)
{
	//min max check: avoids computing the algorithm
	if(p.x<minV.x)return false;
	if(p.y<minV.y)return false;
	if(p.x>maxV.x)return false;
	if(p.y>maxV.y)return false;

	int num=(int)(vertex.size());     
	if(num<3)return false;
	int i=0, j=num-1;
	bool ret = false;

    for (i = 0; i < num; j = i++) {
        if ((((vertex[i].y <= p.y) && (p.y < vertex[j].y)) ||
             ((vertex[j].y <= p.y) && (p.y < vertex[i].y))) &&
            (p.x < (vertex[j].x - vertex[i].x) * (p.y - vertex[i].y) / (vertex[j].y - vertex[i].y) + vertex[i].x))
          ret = !ret;
      }
      return ret;

}



} //mr
