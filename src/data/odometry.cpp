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

#include "odometry.h"
#include "gl/gltools.h"
#include "math/mrmath.h"
#include "math/gaussian.h"

#include <math.h>
#include <fstream>
#include <string>



namespace mr{

IMPLEMENT_MR_OBJECT(Odometry);

Odometry::Odometry()
{
	
}
void Odometry::writeToStream(Stream& stream)
{
	Data::writeToStream(stream);
	pose.writeToStream(stream);

}
void Odometry::readFromStream(Stream& stream)
{
	Data::readFromStream(stream);
	pose.readFromStream(stream);
}

void Odometry::writeToXML(XMLElement* parent)
{
	Data::writeToXML(parent);
	pose.writeToXML(parent);
}

void Odometry::readFromXML(XMLElement* parent)
{
	Data::readFromXML(parent);
	pose.readFromXML(parent);
}

void Odometry::drawGL()
{
	
	
}
Transformation3D Odometry::getIncrement(const Odometry& lastOdometry,float noise)
{
	Pose3D inc=lastOdometry.pose.inverted()*pose;

	if(noise>0){
		double r,p,y;
		inc.orientation.getRPY(r,p,y);
		double m=inc.module();
		Pose3D noisePose(m*sampleGaussian(0,noise),m*sampleGaussian(0,noise),0,
						 0,0,m*sampleGaussian(0,noise));
		inc*=noisePose;
	}	
	return inc;
}
ostream& operator << (ostream& os,const  Odometry& odom)
{
	os<<"ODOM "<<odom.pose.position.x<<" "<<odom.pose.position.y<<" "<<odom.pose.position.z<<endl;
	return os;
}


}
