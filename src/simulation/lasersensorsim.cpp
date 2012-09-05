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


#include "lasersensorsim.h"
#include "../gl/gltools.h"

#include <math.h>

namespace mr{

void LaserSensorSim::writeToStream(Stream& stream)
{
	SolidEntity::writeToStream(stream);
	//data.writeToStream(stream);
	stream<<startAngle<<stepAngle<<numSteps<<maxRange<<sigma;

}
void LaserSensorSim::readFromStream(Stream& stream)
{
	SolidEntity::readFromStream(stream);
	//data.readFromStream(stream);
	stream>>startAngle>>stepAngle>>numSteps>>maxRange>>sigma;
	setLaserProperties(startAngle,stepAngle,numSteps,maxRange,sigma);
}

ostream& operator<<(ostream& os, const LaserSensorSim& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

//constructors
LaserSensorSim::LaserSensorSim(void)
{
setDrawReferenceSystem(); //by default the refence system is drawn
setColor(1,1,0);
sensorActivated=true; //by default, when simulated... the values are computed
}



LaserSensorSim::~LaserSensorSim(void)
{

}
void LaserSensorSim::locationUpdated()
{
PositionableEntity::locationUpdated();
//the laser beam is only updated under request
beamNeedToBeUpdated=true;
}
	//set laser properties
void LaserSensorSim::setLaserProperties(double _startangle, double _step, int _numsteps, 
								double _maxrange, double _sigma)
{
startAngle=_startangle;
stepAngle=_step;
numSteps=_numsteps;
maxRange=_maxrange;
sigma=_sigma;
data.setProperties(startAngle,stepAngle, numSteps,maxRange,sigma);

//segments have to be recomputed.
vector<Angle> angles(data.getAngles());
//preparo los vectores directores de los rayos en coord relativas:
vectorBeam.resize(numSteps);
absoluteVectorBeam.resize(numSteps);
for(int i=0;i<numSteps;i++){
	vectorBeam[i].x=cos(angles[i]);//.getCosine();
	vectorBeam[i].y=sin(angles[i]);//.getSine();
	vectorBeam[i].z=0;
	}
beamNeedToBeUpdated=true;
}
void LaserSensorSim::updateBeam(){
	if(!beamNeedToBeUpdated)return;
	Transformation3D T=getAbsoluteT3D();
	for(int i=0;i<numSteps;i++)absoluteVectorBeam[i]=(T.orientation)*(vectorBeam[i]);
	beamNeedToBeUpdated=false;
} 
//ESTA VERSION ES AL MENOS UN 200% MAS RAPIDA QUE LA RAW
void LaserSensorSim::updateSensorData(World *w,float dt)
{

	if(w==0)w=getWorld();
	if(w==0)return;
	if(sensorActivated==false)return;
	if(beamNeedToBeUpdated)updateBeam();
	Transformation3D T=getAbsoluteT3D();
	//plane filter
	vector<SolidEntity *> list;
	w->getPrimitivesBoundingBoxesCutByPlane(T,list,maxRange);
	//Ahora queda recorrer la lista y verificar
	Vector3D pos=T.position;
	double daux;
	int num=(int)list.size();
	for(int i=0;i<numSteps;i++){
		daux=maxRange;
		double dist;
		for(int j=0;j<num;j++)
		{
			if(list[j]->rayIntersection(pos,absoluteVectorBeam[i],dist))
				if(dist<daux)daux=dist;
		}
		data.setRange(i,daux);
	}
}
//VERSION BASICA
/*
void LaserSensorSim::updateSensorData(World *w,float dt)
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

void LaserSensorSim::drawGL()
{
	//Draw axis
	m.Lock();
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
