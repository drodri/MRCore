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
#ifndef __MRCORE__STREAM__H
#define __MRCORE__STREAM__H

#include <map>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;


#define DECLARE_STREAM_RW_SIMPLE_TYPE( T ) \
		Stream& operator<<(const T &a); \
		Stream& operator>>(T &a);


namespace mr
{
class Object;
/*!
    \class Stream
    \brief
*/
class Stream 
{
public:
	bool good(){return stream!=0 && stream->good();}
	void write(Object* pObj);
	Object* read();

	template<class T> bool read(T* object)
	{
		string className;
		(*this)>>className;
		if(object->getClassName()!=className)
			return false;

		object->readFromStream(*this);
		if(!good())
			return false;
		return true;
	}

	Stream& operator << (Object* pObj);///<alias to write
	Stream& operator >> (Object*& pObj);///<alias to read

	// Definitions:
	DECLARE_STREAM_RW_SIMPLE_TYPE( bool )
	DECLARE_STREAM_RW_SIMPLE_TYPE( char )
	DECLARE_STREAM_RW_SIMPLE_TYPE( unsigned char )
	DECLARE_STREAM_RW_SIMPLE_TYPE( int )	//FIXME, int type not defined the same in every OS
	DECLARE_STREAM_RW_SIMPLE_TYPE( float )
	DECLARE_STREAM_RW_SIMPLE_TYPE( double )
	Stream& operator << (const std::string &str);
	Stream& operator >> (std::string &str);

protected:
	//this class cannot be directly instantiated
	Stream(){stream=0;}
	~Stream();
	//non copyable
	Stream(const Stream& str);
	void operator =(const Stream& str);

	void read(char *Buffer, size_t Count);
	void write(const char *Buffer, size_t Count);
	iostream* stream;
}; //End of class Stream





};
#endif  //__MRCORE__OBJECT_H
