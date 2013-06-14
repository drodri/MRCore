/**********************************************************************
 *
 * This code is part of the MRcore project
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
#include "data.h"


namespace mr{
void Data::writeToStream(Stream& stream)
{
	stream<<timestamp.seconds<<timestamp.microseconds;
}
void Data::readFromStream(Stream& stream)
{
	stream>>timestamp.seconds>>timestamp.microseconds;
}

void Data::writeToXML(XMLElement* parent)
{
	//XMLElement* dat=new XMLElement(parent,"data");
	XMLVariable* sec= new XMLVariable ("seconds",XMLAux::string_Convert<int>(timestamp.seconds).c_str());
	XMLVariable* microsec= new XMLVariable ("microseconds",XMLAux::string_Convert<int>(timestamp.microseconds).c_str());

	parent->AddVariable(sec);
	parent->AddVariable(microsec);
	//parent->AddElement(dat);
}

void Data::readFromXML(XMLElement* parent)
{
	if (parent->FindVariableZ("seconds"))
	{
		XMLVariable* sec=parent->FindVariableZ("seconds");
		timestamp.seconds=sec->GetValueInt();
	}
	if (parent->FindVariableZ("microseconds"))
	{
		XMLVariable* microsec=parent->FindVariableZ("microseconds");
		timestamp.microseconds=microsec->GetValueInt();
	}
}


} //mr