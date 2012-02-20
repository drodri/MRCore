/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Diego
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


#include "kinectsim.h"
#include "../gl/gltools.h"
#include "../world/cylindricalpart.h"
#include <math.h>
#include <stdlib.h> 

namespace mr{

void KinectSim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
	//data.writeToStream(stream);


}
void KinectSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
	//data.readFromStream(stream);
}

ostream& operator<<(ostream& os, const KinectSim& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
KinectSim::KinectSim(void)
{
setDrawReferenceSystem(); //by default the refence system is drawn
setColor(1,1,0);

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
bod->setColor(1.0,0.1,1.0);
//bod->setIntersectable(false);
(*this)+=bod;
}



KinectSim::~KinectSim(void)
{

}
void KinectSim::locationUpdated()
{
PositionableEntity::locationUpdated();
//the laser beam is only updated under request
}

//ESTA VERSION ES AL MENOS UN 200% MAS RAPIDA QUE LA RAW
void KinectSim::updateSensorData(World *w,float dt)
{
	if(w==0)w=getWorld();
	if(w==0)return;

	data.width=600;
	data.height=400;
	data.points.resize(data.width*data.height);
	cout<<"Updating kienct data"<<endl;
	for(int i=0;i<data.width;i++)
		for(int j=0;j<data.height;j++)
		{
			float range=1+0.1*(float)rand()/(float)RAND_MAX;
		//	cout<<"Range "<<range<<endl;
			data.points[i+j*data.width]=Vector3D(i/100.0,j/100.0,range);
		}

}
//VERSION BASICA
/*
void KinectSim::updateSensorData(World *w,float dt)
{

	if(w==0)w=getWorld();
	if(w==0)return;
	if(beamNeedToBeUpdated)updateBeam();
	Vector3D pos=getAbsoluteT3D().position;
	double daux;
	for(int i=0;i<numSteps;i++){
		if(w->rayIntersection(pos,absoluteVectorBeam[i],daux)==false)daux=maxRange;
		data.setRange(i,daux);
		}
}
*/

void KinectSim::drawGL()
{
	//Draw axis
	m.Lock();
	ComposedEntity::drawGL();
	PositionableEntity::drawGL();
	//DrawData
	glPushMatrix();
	location.getAbsoluteT3D().transformGL();
	material.loadMaterial();
	data.drawGL();
	glPopMatrix();
	m.Unlock();
}

}//mr
