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
#include "stream.h"
#include "object.h"

namespace mr
{

Stream::~Stream()
{
	delete stream;
}
void Stream::read(char *Buffer, size_t Count)
{
	stream->read(Buffer,(streamsize)Count);
}
void Stream::write(const char *Buffer, size_t Count)
{
	stream->write(Buffer,(streamsize)Count);
}

#define IMPLEMENT_STREAM_RW_SIMPLE_TYPE( T ) \
	Stream& Stream::operator<<( const T &a) \
	{ \
		write( (char*)&a, sizeof(a) ); \
		return *this; \
	} \
	Stream& Stream::operator>>(T &a) \
	{ \
		read( (char*)&a, sizeof(a) ); \
		return *this; \
	}

IMPLEMENT_STREAM_RW_SIMPLE_TYPE( bool )
IMPLEMENT_STREAM_RW_SIMPLE_TYPE( char)
IMPLEMENT_STREAM_RW_SIMPLE_TYPE( unsigned char)

IMPLEMENT_STREAM_RW_SIMPLE_TYPE( int )
IMPLEMENT_STREAM_RW_SIMPLE_TYPE( float )
IMPLEMENT_STREAM_RW_SIMPLE_TYPE( double )


Stream& Stream::operator << ( const std::string &str)
{
	int n = (int) str.size();
	*this << n;
	if (n)
		write( str.c_str(), n );
	return *this;
}
Stream& Stream::operator >> (std::string &str)
{
	int n=0;
	*this>>n;
	char* buffer=new char[n];
	if(n)
		read(buffer,n);

	str=string(buffer,n);
	delete buffer;

	return *this;
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
	return pObj;
}
Stream& Stream::operator << ( Object* pObj)
{
	write(pObj);
	return *this;
}
Stream& Stream::operator >> (Object*& pObj)
{
	pObj=read();
	return *this;
}
} ;//mr
