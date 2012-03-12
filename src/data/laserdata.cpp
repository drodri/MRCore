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

#include "laserdata.h"
#include "gl/gltools.h"
#include "math/mrmath.h"

#include <math.h>
#include <fstream>
#include <string>



namespace mr{

IMPLEMENT_MR_OBJECT(LaserData);

LaserData::LaserData()
{
	drawGLMode=0;
	//drawGLColor=YELLOW;
	numSteps=0;
}
void LaserData::writeToStream(Stream& stream)
{
	Data::writeToStream(stream);
	stream<<startAngle;
	stream<<stepAngle;
	stream<<numSteps;
	stream<<maxRange;
	for(int i=0;i<numSteps;i++)
		stream<<range[i];
}
void LaserData::readFromStream(Stream& stream)
{
	Data::readFromStream(stream);
	stream>>startAngle;
	stream>>stepAngle;
	stream>>numSteps;
	stream>>maxRange;
	range.resize(numSteps);
	for(int i=0;i<numSteps;i++)
		stream>>range[i];

	points.clear();
}

void LaserData::setRanges(const vector<double>& rang)
{
	if(numSteps!=(int)rang.size())return;
	range=rang;
	points.clear();
}


void  LaserData::setProperties(double _startangle, double _step, int _numSteps, double _maxrange, double _sigma)
{
if(_numSteps<=0)_numSteps=1;
startAngle=_startangle;
stepAngle=_step; 
numSteps=_numSteps; 
maxRange=_maxrange; 
sigma=_sigma;
range.resize(numSteps);
}
bool LaserData::setRange(int index, double _range)
{
if(index<0)return false;
if(index>=numSteps)return false;
if(_range>maxRange)_range=maxRange;
range[index]=_range;
points.clear();
return true;
}
double LaserData::getRange(int index)
{
if(index<0)return maxRange;
if(index>=numSteps)return maxRange;
return range[index];
}

vector<Angle>	LaserData::getAngles() const
{
	vector<Angle> angles;
	angles.resize(range.size());
	for(int i=0;i<numSteps;i++)
		angles[i]=Angle(startAngle+i*stepAngle);
	return angles;
}

vector<Vector2D> LaserData::getPoints() const
{
	if(points.size()!=numSteps)
	{
		points.resize(numSteps);
		for(int i=0;i<numSteps;i++)
		{
			double ang=startAngle+i*stepAngle;
			points[i].x=range[i]*cos(ang);
			points[i].y=range[i]*sin(ang);
		}
	}	
	return points;
}


void LaserData::drawGL()
{
	vector<Vector2D> points=getPoints();
	glDisable(GL_LIGHTING);
	glPointSize(2);
	GLTools::Color(YELLOW);//Diego: fixme. Pero es que necesito ver los puntos, pq sino estan grises y no los veo casi
	if(drawGLMode==0)
	{
		glBegin(GL_POINTS);
		for(unsigned int i=0;i<points.size();i++)
		{
			glVertex3f(points[i].x,points[i].y,0);
		}
		glEnd();
	}
	else if(drawGLMode==1)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLfloat vauxColor[4];
		glGetFloatv(GL_CURRENT_COLOR,vauxColor);
		vauxColor[3]=0.5F;
		glColor4fv(vauxColor);
		glBegin(GL_LINES);
		for(unsigned int i=0;i<points.size();i++)
		{
			glVertex3f(0,0,0);
			glVertex3f(points[i].x,points[i].y,0);
		}
		glEnd();
		glDisable(GL_BLEND);
	}
	else if(drawGLMode==2)
	{
		glBegin(GL_LINE_STRIP);
		glVertex3f(0,0,0);
		for(unsigned int i=0;i<points.size();i++)
			glVertex3f(points[i].x,points[i].y,0);
		glVertex3f(0,0,0);
		glEnd();
	}
	else if(drawGLMode==3)
	{

		glBegin(GL_POINTS);
		for(unsigned int i=0;i<points.size();i++)
		{
			if(range[i]<maxRange)glVertex3f(points[i].x,points[i].y,0);
		}
		glEnd();
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLfloat vauxColor[4];
		glGetFloatv(GL_CURRENT_COLOR,vauxColor);
		vauxColor[3]=0.5F;
		glColor4fv(vauxColor);
		glBegin( GL_TRIANGLES );

		if(points.size()>1)
		for (unsigned int i=0;i<points.size()-1;i++)
		{
			glVertex3f( 0,0,0 );
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
		}
		glEnd();
			glDisable(GL_BLEND);
	}
}

ostream& operator << (ostream& os,const  LaserData& scan)
{
	int size=(int)scan.numSteps;
	os<<"LASER "<<size<<" ";
	for(int i=0;i<size;i++)
	{
		os<<scan.range[i]<<" ";
	}
	os<<endl;
	return os;
}


}
