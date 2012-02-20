#include "lms200sim.h"
#include <iostream>
#include "gl/gltools.h"
#include "../../world/cylindricalpart.h"

namespace mr
{
IMPLEMENT_MR_OBJECT(LMS200Sim)

LMS200Sim::LMS200Sim()
{
//setlaserProperties

//constructor: create the physical representation of the laserscanner

PrismaticPart *bod=new PrismaticPart;

//ojo el laserSensor tiene como referencia el centro del haz, luego al montarlo
//mecánicamente hay que tener en cuenta la transformación

double body[9][2]=
{{0.063,0.122},{-0.093,0.122},{-0.093,-0.063},{0.063,-0.063},
{0.063,-0.053},{0.0,-0.053},{-0.040,-0.030},{-0.040,0.053},{0.063,0.053}};
vector<Vector2D> list;
int i;
for(i=0;i<9;i++)list.push_back(Vector2D(body[i][0],body[i][1]));
bod->setPolygonalBase(list);
bod->setHeight(0.155); //155mm
bod->setRelativePosition(Vector3D(0,0.155/2,0));  
bod->setRelativeOrientation(X_AXIS,PI/2);
bod->setColor(0.3,0.3,1.0);
//bod->setIntersectable(false);
(*this)+=bod;

//las ruedas no se pueden añadir hasta no tener un mecanismo de exclusión de detección
CylindricalPart *glass=new CylindricalPart(0.110,0.053,10);
glass->setColor(0.1,0.1,0.1);
glass->setRelativePosition(Vector3D(0,0,-0.055));
//glass non detectable by raytracing
glass->setIntersectable(false);
(*this)+=glass;

setLaserProperties(-PI/2, DEG2RAD*1.0,181,8.0,0);
setDrawReferenceSystem(false); //due to the solid allows the orientation identification
								//the painting of the ref system is desactivated
}
void LMS200Sim::drawGL()
{
	//it is possible to make the drawing independent of the geometric model
	//so we overwrite the composedentity drawing function with this one
	ComposedEntity::drawGL();
	LaserSensorSim::drawGL();
	return;
}
void LMS200Sim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
}
void LMS200Sim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
}

}; //Namespace mr
