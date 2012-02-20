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

#include "sonardata.h"
#include "gl/gltools.h"

#include <math.h>
#include <fstream>
#include <string>

namespace mr{

IMPLEMENT_MR_OBJECT(SonarData);

SonarData::SonarData()
{
	drawGLMode=0;
	readings.resize(0);
}

void SonarData::writeToStream(Stream& stream)
{
	stream<<maxRange;
	stream<<sigma;
	stream<<fov;
	
	int n=(int)readings.size();
	stream<<n;
	for(int i=0;i<n;i++)
	{
		stream<<readings[i].range;
		readings[i].pose.writeToStream(stream);
	}	
}
void SonarData::readFromStream(Stream& stream)
{
	stream>>maxRange;
	stream>>sigma;
	int n;
	stream>>n;
	readings.resize(n);
	for(int i=0;i<n;i++)
	{
		stream>>readings[i].range;
		readings[i].pose.readFromStream(stream);
	}
}


//TODO
void SonarData::drawGL()
{
	cout << "Sonar Data Drawing not Implemented" << endl;
}

ostream& operator << (ostream& os,const  SonarData& scan)
{
	int size= (int)scan.readings.size();
	os<<"SONAR "<<size<<" ";
	for(int i=0;i<size;i++)
	{
		os <<scan.readings[i].range<<" "<<scan.readings[i].pose.x<<" "<<scan.readings[i].pose.y<<" "<<scan.readings[i].pose.theta<<" ";
	}
	os<<endl;
	return os;
}

} //end namespace
