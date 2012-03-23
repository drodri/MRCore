/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  -----------anyone
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

#ifndef __MRCORE__SONARDATA__H
#define __MRCORE__SONARDATA__H

#include "data.h"
#include "math/transformation2d.h"
#include <iostream>
#include <vector>

using namespace std;

namespace mr{

/** This class corresponds to a single sonar reading **/

class SonarReading{

public:
		SonarReading(){};
		SonarReading(double r, Pose2D p){ range=r; pose.x=p.x; pose.y=p.y; pose.theta=p.theta;}
		virtual ~SonarReading(){}
		
		double range;
		Pose2D pose;
};
		
/** The sonar data is referenced to the following coordinates:
The origin is in the visual center of the sensor, righ handed system
The X axis is pointing front
The Y axis is in the scan plane
The Z axis points upwards
The bearings are rotations in the Z axis. **/
		

class SonarData :public Data
{
	///Text stream serializers for files and cout, cerr (std)
	friend ostream& operator << (ostream& os,const SonarData& scan);
	
	DECLARE_MR_OBJECT(SonarData)

public:
	SonarData();
	virtual ~SonarData(){}

	//Serializers
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);

	/**
	 * Push a sonar reading into the Sonar Data, making a copy of the passed param
	**/
	void pushReading(const SonarReading& r){
		readings.push_back(r);
	}
	/**
	 * Gets a copy of a particular reading
	**/
	SonarReading getReading(int i){return readings[i];}

	/**
	 * Clear all Readings
	**/

	void clear(){readings.clear();}
	
	/** Drawing mode. 0=points, 1=solid, 2=contour **/
	int drawGLMode;
	virtual void drawGL();//<OpenGL draw
		
protected:
	//the data itself
	vector<SonarReading> readings ;

public:
	//Configuration parameters, could be factorized to a LaserConf class
	double maxRange;//<the maximum range of the
	double sigma; //< standard deviation of the range measurement
	double fov; //< field of view of the sonar Readings
};


}



#endif
