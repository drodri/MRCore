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
#ifndef __MRCORE__XMLFILE__H
#define __MRCORE__XMLFILE__H

#include "xml.h"


using namespace std;



namespace mr
{
class Object;
/*!
    \class XMLfile
    \brief open, read, and writes a XMLfile for one object. 
	is useful for worlds and world objects. 
*/
class XMLfile 
{
public:
	static Object *load(const char *file); //loads and parses the file
	static bool save(Object* pObj,const char *file); //saves the XML file created with the object
}; //End of class Stream





};
#endif  //__MRCORE__XMLfile_H
