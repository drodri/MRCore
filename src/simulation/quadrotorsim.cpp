#include "quadrotorsim.h"
#include <iostream>
#include "gl/gltools.h"

#include "base/logger.h"
#include "../world/world.h"
#include "../world/cylindricalpart.h"

namespace mr
{
IMPLEMENT_MR_OBJECT(QuadrotorSim)

QuadrotorSim::QuadrotorSim(double rad){
	radius=rad;
	upSpeed=forwardSpeed=lateralSpeed=rotSpeed=0;
	motorsOn=false;
	quadrotorData.battery=100;
}
//serializers
void QuadrotorSim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
	
}
void QuadrotorSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
}
void QuadrotorSim::simulate(double t)
{
	Transformation3D position=getAbsoluteT3D();
	
	quadrotorData.altitude=(float)position.position.z;
	quadrotorData.battery-=0.001f;

	if(quadrotorData.battery<1)//less than 1%
		motorsOn=false;

	static float velFalling=0;
	if(!motorsOn)
	{
		//simulate falling to groudn	
		position.position.z+=velFalling*t-4.9*t*t;
		velFalling-=9.8f*t;
		if(position.position.z<=0)
			position.position.z=0;
		setAbsoluteT3D(position);
		return;
	}
	else 
		velFalling=0;

	double roll, pitch, yaw;
	position.orientation.getRPY(roll,pitch,yaw);
	double deltaX=forwardSpeed*t;
	double deltaY=lateralSpeed*t;
	double deltaZ=upSpeed*t;
	double deltaYaw=rotSpeed*t;
	Transformation3D inc(deltaX,deltaY,deltaZ,Z_AXIS,deltaYaw);
	Transformation3D newposition=position*inc;	
	setAbsoluteT3D(newposition);
}

void QuadrotorSim::drawGL()
{
	//las ruedas no se pueden añadir hasta no tener un mecanismo de exclusión de detección
	CylindricalPart ring(0.2,radius);
	ring.setColor(0.5,0.5,1.0);

glPushMatrix();	
	getAbsoluteT3D().transformGL();
	GLTools::Print("Hola que tal");

	glRotatef(forwardSpeed*30,0,1,0);
	glRotatef(-lateralSpeed*30,1,0,0);
	
	//DrawDirection
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glLineWidth(2);
		if(motorsOn)
			glColor3ub(0,255,0);
		else
			glColor3ub(255,0,0);
		glVertex3f(2,0,0);
		glVertex3f(0,0,0);
	glEnd();

	//each ring
	ring.setRelativeOrientation(Z_AXIS,PI/2);
	ring.setRelativePosition(Vector3D(radius,radius,0));
	ring.drawGL();

	ring.setRelativeOrientation(Z_AXIS,PI/2);
	ring.setRelativePosition(Vector3D(-radius,radius,0));
	ring.drawGL();

	ring.setRelativeOrientation(Z_AXIS,PI/2);
	ring.setRelativePosition(Vector3D(-radius,-radius,0));
	ring.drawGL();

	ring.setRelativeOrientation(Z_AXIS,PI/2);
	ring.setRelativePosition(Vector3D(radius,-radius,0));
	ring.drawGL();
glPopMatrix();	
	
	return;
}
bool QuadrotorSim::move(double up, double forward,double lateral,double rot)
{
	if(!motorsOn)
	{
		upSpeed=forwardSpeed=lateralSpeed=rotSpeed=0;
		return true;
	}
	upSpeed=up;
	forwardSpeed=forward;
	lateralSpeed=lateral;
	rotSpeed=rot;

	if(upSpeed>1)upSpeed=1;if(upSpeed<-1)upSpeed=-1;
	if(forwardSpeed>1)forwardSpeed=1;if(forwardSpeed<-1)forwardSpeed=-1;
	if(lateralSpeed>1)lateralSpeed=1;if(lateralSpeed<-1)lateralSpeed=-1;
	if(rotSpeed>1)rotSpeed=1;if(rotSpeed<-1)rotSpeed=-1;
	return true;
}

}; //Namespace mr
