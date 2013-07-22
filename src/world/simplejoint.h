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

#ifndef __MRCORE__SIMPLEJOINT__H
#define __MRCORE__SIMPLEJOINT__H


#include "joint.h"
#include "math/mrmath.h"


using namespace std;
namespace mr
{
/**
    \class SimpleJoint
    Joint	->	Implementation of 1D angular/prismatic joints.
*/



class SimpleJoint: public Joint
{
	DECLARE_MR_OBJECT(SimpleJoint)
	friend class World;
	/**Text output**/
	friend ostream& operator<<(ostream& os, const SimpleJoint& p);
protected:
//attributes
   double vmax;
   double vmin;
   double offset;
   double factor;
   double value;
   Axis axis;
   bool prismatic; //true if prismatic. By default is rotational




public:
//constructors

	//Basic Constructor
	SimpleJoint(double _max=PI, double _min=-PI, bool _CW=true, double _offset=0, Axis ax=Z_AXIS,bool _prismatic=false){
		setProperties(_max,_min,_CW,_offset,ax,_prismatic);
	}
	//Destructor
	virtual ~SimpleJoint(void);
	
//methods
	void setValue(double val);
	double getValue(){return value;}
	bool checkValue(const double val) const 
		{if(val<vmin)return false; if(val>vmax)return false; return true;}
	//serialization
	virtual void writeToStream(Stream& stream);
	virtual void readFromStream(Stream& stream);
	virtual void writeToXML(XMLElement* parent);
	virtual void readFromXML(XMLElement* parent);

	void setProperties(double _max, double _min, bool CW, double _offset, Axis ax, bool _prismatic);
	void getProperties(double *_max, double *_min, bool *CW=0, double *_offset=0, Axis *ax=0, bool *prism=0)
	{
		if(_max)*_max=vmax;if(_min)*_min=vmin;
		if(CW)*CW=factor>0?true:false;
		if(_offset)*_offset=offset;
		if(ax)*ax=axis;
		if(prism)*prism=prismatic;

	}
	bool isPrismatic(){return prismatic;}
	void getMaxMin(double &_max, double &_min){_max=vmax; _min=vmin;}



};

};
#endif  //__MRCORE__SIMPLEJOINT__H
