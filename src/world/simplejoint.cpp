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


#include "simplejoint.h"

#include <math.h>

namespace mr{
IMPLEMENT_MR_OBJECT(SimpleJoint)
void SimpleJoint::writeToStream(Stream& stream)
{
	Joint::writeToStream(stream);
	stream<<((char)axis);
	stream<<factor<<offset<<prismatic<<value<<vmax<<vmin;
//	stream<<speed<<maxSpeed;

}
void SimpleJoint::readFromStream(Stream& stream)
{
	char aux;
	Joint::readFromStream(stream);
	stream>>aux; axis=(Axis)aux;
	stream>>factor>>offset>>prismatic>>value>>vmax>>vmin;
//	stream>>speed>>maxSpeed;
}

void SimpleJoint::writeToXML(XMLElement* parent)
{
	//XMLAux aux;
	//XMLElement* sJoint=new XMLElement(parent,"simplejoint");
	XMLVariable* ax= new XMLVariable ("axis",(const char*)axis);
	XMLVariable* fac= new XMLVariable ("factor",XMLAux::string_Convert<double>(factor).c_str());
	XMLVariable* offs= new XMLVariable ("offset",XMLAux::string_Convert<double>(offset).c_str());
	XMLVariable* prismat= new XMLVariable ("prisamtic",XMLAux::string_ConvertBool(prismatic).c_str());
	XMLVariable* val= new XMLVariable ("value",XMLAux::string_Convert<double>(value).c_str());
	XMLVariable* _vmax= new XMLVariable ("vmax",XMLAux::string_Convert<double>(vmax).c_str());
	XMLVariable* _vmin= new XMLVariable ("vmin",XMLAux::string_Convert<double>(vmin).c_str());
//	XMLVariable* veloc= new XMLVariable ("speed",XMLAux::string_Convert<double>(speed).c_str());
//	XMLVariable* maxveloc= new XMLVariable ("maxSpeed",XMLAux::string_Convert<double>(maxSpeed).c_str());

	parent->AddVariable(ax);
	parent->AddVariable(fac);
	parent->AddVariable(offs);
	parent->AddVariable(prismat);
	parent->AddVariable(val);
	parent->AddVariable(_vmax);
	parent->AddVariable(_vmin);
//	parent->AddVariable(veloc);
//	parent->AddVariable(maxveloc);
	//parent->AddElement(sJoint);
	Joint::writeToXML(parent);
}

void SimpleJoint::readFromXML(XMLElement* parent)
{

	if (parent->FindVariableZ("axis"))
	{
		char cad;
		parent->FindVariableZ("axis")->GetValue(&cad);
		axis=(Axis)cad;
	}
	if (parent->FindVariableZ("factor"))
	{
		factor=XMLAux::GetValueDouble(parent->FindVariableZ("factor"));
	}
	if (parent->FindVariableZ("offset"))
	{
		offset=XMLAux::GetValueDouble(parent->FindVariableZ("offset"));
	}

	if (parent->FindVariableZ("prismatic"))
	{
		string cad;
		cad=XMLAux::GetValueCadena(parent->FindVariableZ("prismatic"));

		if (cad=="true" || cad!="0")
			prismatic=true;
		else
			prismatic=false;
	}

	if (parent->FindVariableZ("value"))
	{
		value=XMLAux::GetValueDouble(parent->FindVariableZ("value"));
	}

	if (parent->FindVariableZ("vmax"))
	{
		vmax=XMLAux::GetValueDouble(parent->FindVariableZ("vmax"));
	}

	if (parent->FindVariableZ("vmin"))
	{
		vmin=XMLAux::GetValueDouble(parent->FindVariableZ("vmin"));
	}

	//if (parent->FindVariableZ("speed"))
	//{
	//	speed=XMLAux::GetValueDouble(parent->FindVariableZ("speed"));
	//}

	//if (parent->FindVariableZ("maxSpeed"))
	//{
	//	maxSpeed=XMLAux::GetValueDouble(parent->FindVariableZ("maxSpeed"));
	//}

	Joint::readFromXML(parent);

}

ostream& operator<<(ostream& os, const SimpleJoint& p)
{
	//os<<p.x<<" "<<p.y<<" "<<p.z;
	return os;
}

void SimpleJoint::setValue(double val){
	if(val>vmax)val=vmax;
	if(val<vmin)val=vmin;
	value=val;
	if(prismatic){
		Vector3D pos;
		pos[(int)axis]=offset+value*factor;
		outputLocation.setRelativePosition(pos);
	}
	else
	outputLocation.setRelativeOrientation(axis,offset+value*factor);
}
void SimpleJoint::setProperties(double _max, double _min, bool CW, double _offset, Axis ax, bool _prismatic)
{
	if(_max>_min){
		vmax=_max;
		vmin=_min;
	}
   value=0;
   if(value<vmin)value=vmin;
   if(value>vmax)value=vmax;
   offset=_offset;
   if(CW)factor=1.0F;
   else factor=-1.0F;
   axis=ax;
   prismatic=_prismatic;
}


SimpleJoint::~SimpleJoint(void)
{

}

}//mr
