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

#ifndef __MRCORE__LASERSENSOR_3D_SIM__H
#define __MRCORE__LASERSENSOR_3D_SIM__H

#include "hw/lasersensor3d.h"
#include "../world/composedentity.h"
#include <vector>
#include "../data/laserdata3d.h"
//#include "../math/segment3d.h"
#include "../world/world.h"
#include "../system/mutex.h"
#include <stdlib.h>

using namespace std;

namespace mr
{
/*!
    \class LaserSensorSim
    LaserSensorSim	->	A generic class for a positionable laser scanner sensor.
	Its reference is as follows: X axis is the 0 rad laser beam, while PI/2 is the Y axis. Z is
	oriented perpendicular tu the laser beam plane.
*/

class LaserSensor3DSim : public LaserSensor3D, public ComposedEntity
{
	DECLARE_MR_OBJECT(LaserSensor3DSim )
protected:
	//this transformation should be modified by the joint parameters
	LaserData3D data; 
	Mutex m;

public:
//attributes
	
//constructors
	//Basic constructor
	LaserSensor3DSim(void);

	//Destructor
	virtual ~LaserSensor3DSim(void);

//methods
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	//set laser properties

	void drawGL();
	void setDrawGLMode(int m){data.drawGLMode=m;}

	//laserSensor Methods
	virtual void updateSensorData()
	{
		vector<LaserData> auxL;
		vector<Transformation3D> auxT;
		for(double alfa=-M_PI/4; alfa<M_PI/4;alfa+=0.1)
		{
			LaserData las;
			las.setProperties(-90*DEG2RAD,1*DEG2RAD,181);
			vector<double> v(181,4.0);
			for(unsigned int i=1;i<v.size();i++)
				v[i]=2+0.5*rand()/(float)RAND_MAX;
			las.setRanges(v);
			Transformation3D t(0,0,0,0,alfa,0);
			
			auxL.push_back(las);
			auxT.push_back(t);	
		}
		m.Lock();
		data.set(auxL,auxT);
		m.Unlock();
	}
	virtual bool getData(LaserData3D& d){
		m.Lock();
		d=data;
		m.Unlock();
		return true;
	}
	//using laserSensorSim for displaying other laserSensor data
	virtual void setData(const LaserData3D& d){
		data=d;
	}
};

};
#endif  //__MRCORE__LASERSCANNERVIRTUAL__H
