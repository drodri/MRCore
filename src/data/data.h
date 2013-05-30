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
#ifndef __MRCORE__DATA__H
#define __MRCORE__DATA__H

#include "base/globject.h"
#include "system/time.h"
#include "../base/xmlaux.h"

namespace mr
{
/*!
    \class Data
    \brief Data, the base class that holds raw data from sensors
*/
class Data: public GLObject
{
public:
	void writeToStream(Stream& stream);
	void readFromStream(Stream& stream);
	void writeToXML(XMLElement* parent);
	void readFromXML(XMLElement* parent);
	//annotates this data item with the current time
	void timeStamp()
	{
		timestamp.tic();
	}
	MRTime getTimeStamp()
	{
		return timestamp;
	}
protected:
	MRTime timestamp;
};

};
#endif  //__MRCORE__SENSOR_DATA__H
