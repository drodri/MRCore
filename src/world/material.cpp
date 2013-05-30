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


#include "material.h"
#include "../gl/gltools.h"

namespace mr{

void Material::loadMaterial()
{
	glColor4f(color.r,color.g,color.b,1);
	glEnable(GL_LIGHTING);
}

void Material::writeToStream(Stream& stream)
{
	stream<<color.r<<color.g<<color.b;

}
void Material::readFromStream(Stream& stream)
{
	stream>>color.r>>color.g>>color.b;
}

void Material::writeToXML(XMLElement* parent)
{
	//XMLAux aux;
	bool defaultR=true, defaultG=true, defaultB=true;

	XMLElement* colour=new XMLElement(parent,"colour");

	if (color.r!=0.5)
	{
		XMLVariable* r= new XMLVariable("r",XMLAux::string_Convert<double>(color.r).c_str());
		colour->AddVariable(r);
		defaultR=false;
	}
	if (color.g!=0.5)
	{
		XMLVariable* g= new XMLVariable("g",XMLAux::string_Convert<double>(color.g).c_str());
		colour->AddVariable(g);
		defaultG=false;
	}
	if (color.b!=0.5)
	{
		XMLVariable* b= new XMLVariable("b",XMLAux::string_Convert<double>(color.b).c_str());
		colour->AddVariable(b);
		defaultB=false;
	}

	if (defaultR && defaultG && defaultB)
		return;
	else
		parent->AddElement(colour);

}

void Material::readFromXML(XMLElement* parent)
{
	XMLElement* colour;
	double r=color.r,g=color.g,b=color.b;

	if(parent->FindElementZ("colour"))
	{
		colour=parent->FindElementZ("colour");

		if(colour->FindVariableZ("r"))
			r=XMLAux::GetValueDouble(colour->FindVariableZ("r"));
		if(colour->FindVariableZ("g"))
			g=XMLAux::GetValueDouble(colour->FindVariableZ("g"));
		if(colour->FindVariableZ("b"))
			b=XMLAux::GetValueDouble(colour->FindVariableZ("b"));
	}

	setColor(r,g,b);

}
}//mr
