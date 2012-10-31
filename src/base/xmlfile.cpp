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
#include "xmlfile.h"
#include "object.h"

namespace mr
{


Object *XMLfile::load(const char *file) //loads, parses the file and creates the object
{

}
bool XMLfile::save(Object* pObj,const char *file); //saves the XML file created with the object
{

}
void Stream::write(Object* pObj)
{
	//id
	//size
	// First, the "classname".
 	string className = pObj->getClassName();
	(*this)<<className;
//	cout<<"Writing object: "<<className<<endl;
	
	// Next, the object data.
	pObj->writeToStream(*this);
}

Object* Stream::read()
{
	//id
	//size
	string className;
	(*this)>>className;
	Object*	pObj=Object::create(className);
	if(pObj)
		pObj->readFromStream(*this);
	if(!good())
	{
		delete pObj;
		return 0;
	}
	return pObj;
}




} ;//mr
