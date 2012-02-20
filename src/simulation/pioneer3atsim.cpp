#include "pioneer3atsim.h"
#include <iostream>
#include "gl/gltools.h"
//#include <GL/glut.h>
#include "../world/cylindricalpart.h"

namespace mr
{
IMPLEMENT_MR_OBJECT(Pioneer3ATSim)
Pioneer3ATSim::Pioneer3ATSim():WheeledBaseSim(0.34,0.25,0.1,0.1)
{
speed=0;
rotSpeed=0;
//constructor: create the physical representation of the robot
PrismaticPart *top=new PrismaticPart;
PrismaticPart *bod=new PrismaticPart;

double topVertex[20][2]=
	{{0.176776695,-0.176776695},{0.216506351,-0.125},{0.241481457,-0.064704761},
	 {0.25,0},{0.241481457,0.064704761},{0.216506351,0.125},
	 {0.176776695,0.176776695},{0.132,0.193},{-0.037,0.193},
	 {-0.037,0.168},{-0.184,0.168},{-0.216506351,0.125},
	 {-0.241481457,0.064704761},{-0.25,0},{-0.241481457,-0.064704761},
	 {-0.216506351,-0.125},{-0.184,-0.168},{-0.037,-0.168},
	 {-0.037,-0.193},{0.132,-0.193}};
double body[8][2]=
{{0.2,0.0825},{0.16,0.1325},{-0.16,0.1325},{-0.2,0.0825},
{-0.2,-0.0825},{-0.16,-0.1325},{0.16,-0.1325},{0.2,-0.0825}};
vector<Vector2D> list;
int i;
for(i=0;i<20;i++)list.push_back(Vector2D(topVertex[i][0],topVertex[i][1]));
top->setPolygonalBase(list);
top->setHeight(0.005); //5mm
top->setRelativePosition(Vector3D(0,0,0.275));
top->setColor(0.1,0.1,0.1);
(*this)+=top;
list.clear();
for(int i=0;i<8;i++)list.push_back(Vector2D(body[i][0],body[i][1]));
bod->setPolygonalBase(list);
bod->setHeight(0.19); //190mm
bod->setRelativePosition(Vector3D(0,0,0.085));
bod->setColor(1.0,0.1,0.1);
(*this)+=bod;

//las ruedas no se pueden añadir hasta no tener un mecanismo de exclusión de detección
CylindricalPart *wheel1=new CylindricalPart(wheel_width,wheel_radius);
wheel1->setColor(0.1,0.1,0.1);
wheel1->setRelativeOrientation(X_AXIS,-PI/2);

CylindricalPart *wheel2=new CylindricalPart(wheel_width,wheel_radius);
wheel2->setColor(0.1,0.1,0.1);
wheel2->setRelativeOrientation(X_AXIS,-PI/2);

CylindricalPart *wheel3=new CylindricalPart(wheel_width,wheel_radius);
wheel3->setColor(0.1,0.1,0.1);
wheel3->setRelativeOrientation(X_AXIS,PI/2);

CylindricalPart *wheel4=new CylindricalPart(wheel_width,wheel_radius);
wheel4->setColor(0.1,0.1,0.1);
wheel4->setRelativeOrientation(X_AXIS,PI/2);

wheel1->setRelativePosition(Vector3D(large/2,width/2,wheel_radius+0.001));
wheel2->setRelativePosition(Vector3D(-large/2,width/2,wheel_radius+0.001));
wheel3->setRelativePosition(Vector3D(large/2,-width/2,wheel_radius+0.001));
wheel4->setRelativePosition(Vector3D(-large/2,-width/2,wheel_radius+0.001));

(*this)+=wheel1;
(*this)+=wheel2;
(*this)+=wheel3;
(*this)+=wheel4;
}
void Pioneer3ATSim::writeToStream(Stream& stream)
{WheeledBaseSim::writeToStream(stream);}
void Pioneer3ATSim::readFromStream(Stream& stream)
{WheeledBaseSim::readFromStream(stream);}
void Pioneer3ATSim::drawGL()
{
	//it si possible to make the drawing independent of the geometric model
	//so we overwrite the composedentity drawing function with this one
	ComposedEntity::drawGL();
	return;
}
void Pioneer3ATSim::simulate(double delta_t)
{
//a trick for avoiding the collision of wheels
objects[2]->setRelativePosition(Vector3D(large/2,width/2,wheel_radius*1.3));
objects[3]->setRelativePosition(Vector3D(-large/2,width/2,wheel_radius*1.3));
objects[4]->setRelativePosition(Vector3D(large/2,-width/2,wheel_radius*1.3));
objects[5]->setRelativePosition(Vector3D(-large/2,-width/2,wheel_radius*1.3));
WheeledBaseSim::simulate(delta_t);
objects[2]->setRelativePosition(Vector3D(large/2,width/2,wheel_radius+0.001));
objects[3]->setRelativePosition(Vector3D(-large/2,width/2,wheel_radius+0.001));
objects[4]->setRelativePosition(Vector3D(large/2,-width/2,wheel_radius+0.001));
objects[5]->setRelativePosition(Vector3D(-large/2,-width/2,wheel_radius+0.001));

}

}; //Namespace mr