/**********************************************************************
 *
 * This code is part of the MRcore project
 * Author:  Rodrigo Azofra Barrio & Miguel Hernando Gutierrez
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

#ifndef __MRCORE__LASERSENSORSIM__H
#define __MRCORE__LASERSENSORSIM__H

#include "hw/lasersensor.h"
#include "../world/composedentity.h"
#include <vector>
#include "../data/laserdata.h"
//#include "../math/segment3d.h"
#include "../world/world.h"
#include "../system/mutex.h"
#include "../base/xmlaux.h"



using namespace std;

namespace mr
{
/*!
    \class LaserSensorSim
    LaserSensorSim	->	A generic class for a positionable laser scanner sensor.
	Its reference is as follows: X axis is the 0 rad laser beam, while PI/2 is the Y axis. Z is
	oriented perpendicular tu the laser beam plane.
*/

class LaserSensorSim : public LaserSensor, public ComposedEntity
{
	

	/**Text output**/
	friend ostream& operator<<(ostream& os, const LaserSensorSim& p);
protected:
	//this transformation should be modified by the joint parameters
	LaserData data; 
	//beam of laser segments relative to the sensor base
	vector<Vector3D> vectorBeam;
	//beam of laser segments in absolute coordinates
	vector<Vector3D> absoluteVectorBeam;
	//bool flag that indicates that the absolute segments have to be updated if needed
	bool beamNeedToBeUpdated;
	//virtual methos executed each time the laser is moved
	void locationUpdated();
	//laser scanner properties
	double startAngle; //minimum -PI/2
	double stepAngle; //maximun PI/2
	int numSteps; //number of laser measures 360
	double maxRange; //max range in m : 10
	double sigma;  //min range =0
	void updateBeam();
	Mutex m;
	bool sensorActivated; //true if when simulated... is updated
public:
//attributes
	
//constructors
	//Basic constructor
	LaserSensorSim(void);

	//Destructor
	virtual ~LaserSensorSim(void);

//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);
	//set laser properties
	void setLaserProperties(double _startangle, double _step, int _numsteps, 
								double _maxrange, double _sigma);
	void updateSensorData(World *w=0,float dt=0);
	void drawGL();
	void setDrawGLMode(int m){data.drawGLMode=m;}
	void setActive(bool val=true){sensorActivated=val;}
	void simulate(double t)
	{
		m.Lock();
		updateSensorData();
		m.Unlock();
	}
	//laserSensor Methods
	virtual bool getData(LaserData& d){
		m.Lock();
		d=data;
		m.Unlock();
		return true;
	}
	//using laserSensorSim for displaying other laserSensor data
	virtual void setData(const LaserData& d){
		m.Lock();
		data=d;
		m.Unlock();
	}
};

};
#endif  //__MRCORE__LASERSCANNERVIRTUAL__H
