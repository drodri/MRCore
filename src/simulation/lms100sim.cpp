#include "lms100sim.h"
#include <iostream>
#include "gl/gltools.h"
#include "../world/cylindricalpart.h"

namespace mr
{
IMPLEMENT_MR_OBJECT(LMS100Sim)

LMS100Sim::LMS100Sim()
{

//constructor: create the physical representation of the laserscanner
//ojo el laserSensor tiene como referencia el centro del haz, luego al montarlo
//mecánicamente hay que tener en cuenta la transformación (la altura de la base al haz es 0.01157 m 

//la resolución tomada es 0.5º  270º
CylindricalPart *body1=new CylindricalPart(0.095,0.05025);
body1->setColor(0.3,0.3,1.0);
body1->setRelativePosition(Vector3D(0,0,-0.1157));
(*this)+=body1;


vector<Vector2D> list;
PrismaticPart *body2=new PrismaticPart;
list.push_back(Vector2D(0.0,-0.05));
list.push_back(Vector2D(-0.0525,-0.05));
list.push_back(Vector2D(-0.0525,0.05));
list.push_back(Vector2D(0.0,0.05));
body2->setPolygonalBase(list);
body2->setHeight(0.09); //90mm
body2->setColor(0.3,0.3,1.0);
body2->setRelativePosition(Vector3D(0,0,-0.1157));  
(*this)+=body2;

//el cristal se pinta pero no se usa en la detección
CylindricalPart *glass=new CylindricalPart(0.100,0.045,10);
glass->setColor(0.1,0.1,0.1);
glass->setRelativePosition(Vector3D(0,0,-0.0635));
//glass non detectable by raytracing
glass->setIntersectable(false);
(*this)+=glass;

setLaserProperties(DEG2RAD*(-135), DEG2RAD*0.5,541,20.0,0);
setDrawReferenceSystem(false); //due to the solid allows the orientation identification
								//the painting of the ref system is desactivated
}
void LMS100Sim::drawGL()
{
	//it is possible to make the drawing independent of the geometric model
	//so we overwrite the composedentity drawing function with this one
	ComposedEntity::drawGL();
	LaserSensorSim::drawGL();
	return;
}
void LMS100Sim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
}
void LMS100Sim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
}
void LMS100Sim::writeToXML(XMLElement* parent)
{
	SolidEntity::writeToXML(parent);
}

void LMS100Sim::readFromXML(XMLElement* parent)
{
	SolidEntity::readFromXML(parent);
}
}; //Namespace mr
