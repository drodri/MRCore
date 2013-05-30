#include "powercube70sim.h"
#include <iostream>
#include "gl/gltools.h"

#include "base/logger.h"
#include "../world/world.h"
#include "../world/cylindricalpart.h"
#include "../world/tcp.h"

namespace mr
{
IMPLEMENT_MR_OBJECT(PowerCube70Sim)
PowerCube70Sim::PowerCube70Sim(){
/*speed=0;
rotSpeed=0;*/

//constructor: create the physical representation of the robot


CylindricalPart *cilbase=new CylindricalPart(0.038,0.032);
cilbase->setColor(0.1,0.5,0.0);

panJoint = new SimpleJoint(PI,-PI);
panJoint->setRelativePosition(Vector3D(0,0,0.0377));
panJoint->LinkTo(cilbase);

vector<Vector2D> list;
list.push_back(Vector2D(-0.035,0.035));
list.push_back(Vector2D(-0.035,-0.035));
list.push_back(Vector2D(0.035,-0.035));
list.push_back(Vector2D(0.035,0.035));
PrismaticPart *body1=new PrismaticPart;
body1->setPolygonalBase(list);
body1->setColor(0.1,0.5,0.0);
body1->setHeight(0.1);
body1->LinkTo(panJoint);

tiltJoint = new SimpleJoint(2*PI/3,-2*PI/3,true,0,Y_AXIS);
tiltJoint->setRelativePosition(Vector3D(0,0,0.0574));
tiltJoint->LinkTo(body1);

CylindricalPart *ciltop=new CylindricalPart(0.150,0.035);
ciltop->setColor(0.1,0.5,0.0);
ciltop->setRelativePosition(Vector3D(0,-0.0880,0));
ciltop->setRelativeOrientation(X_AXIS,-PI/2);
ciltop->LinkTo(tiltJoint);

list.clear();
list.push_back(Vector2D(-0.070,-0.044));
list.push_back(Vector2D(-0.070,-0.054));
list.push_back(Vector2D(0.0,-0.054));
list.push_back(Vector2D(0.0,0.054));
list.push_back(Vector2D(-0.070,0.054));
list.push_back(Vector2D(-0.070,0.044));
list.push_back(Vector2D(-0.01,0.044));
list.push_back(Vector2D(-0.01,-0.044));
PrismaticPart *top=new PrismaticPart;
top->setPolygonalBase(list);
top->setColor(0.1,0.5,0.0);
top->setHeight(0.07);
top->setRelativePosition(Vector3D(0.035,0.0,0.07));
top->setRelativeOrientation(Y_AXIS,-PI/2);
top->LinkTo(tiltJoint);


//hay que hacer el TCP para que se puedan enganchar al powercube
tcp=new Tcp;
tcp->setRelativePosition(Vector3D(0,0,0.07));
tcp->LinkTo(tiltJoint);
//tcp->setDrawReferenceSystem();


(*this)+=cilbase; //all the dependent components are added after the base


}

void PowerCube70Sim::writeToStream(Stream& stream){
	SolidEntity::writeToStream(stream);
	stream<<panJoint->getValue()<<tiltJoint->getValue();
}
void PowerCube70Sim::readFromStream(Stream& stream){
		SolidEntity::readFromStream(stream);
		double p,t; 
		stream>>p>>t;
		setPos(p,t);
}
void PowerCube70Sim::writeToXML(XMLElement* parent)
{
	SolidEntity::writeToXML(parent);
}
void PowerCube70Sim::readFromXML(XMLElement* parent)
{
	SolidEntity::readFromXML(parent);
}


void PowerCube70Sim::simulate(double delta_t)
{
	//aqui es necesario recoger el mundo al que pertenece el world

}
void PowerCube70Sim::drawGL()
{
//constructor: create a physical representation of the robot only for drawing
	
	ComposedEntity::drawGL();
	return;
}
int PowerCube70Sim::Move(float pos_pan,float pos_tilt)
{
	//de momento es instantaneo
	setPos(pos_pan,pos_tilt);
	return 1;
}
void PowerCube70Sim::setPos(double pan, double tilt)
{
	panJoint->setValue(pan);
	tiltJoint->setValue(tilt);
}
int PowerCube70Sim::GetPos(float& pan_angle,float& tilt_angle)
{
	pan_angle=(float)panJoint->getValue();
	tilt_angle=(float)tiltJoint->getValue();
	return 1;
}
}; //Namespace mr