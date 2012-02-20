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
#ifndef __MRCORE__STREAMSTRING__H
#define __MRCORE__STREAMSTRING__H

#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include "stream.h"

using namespace std;


namespace mr
{
/*!
    \class Stream
    \brief
*/
class StreamString: public Stream
{
public:
	StreamString()
	{
		stream=new stringstream;
	}
	StreamString(const string& contents)
	{
		stream=new stringstream;
		((stringstream*)stream)->str(contents);
	}
	void setString(const string& contents)
	{
		((stringstream*)stream)->str(contents);
	}
	string getString()
	{
		return ((stringstream*)stream)->str();
	}

protected:
	//non-copyable, non movable
	StreamString(const StreamString& str);
	void operator =(const StreamString& str);

}; 


};
#endif  //__MRCORE__OBJECT_H
