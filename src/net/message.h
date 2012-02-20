// Logger.h: interface for the Utils class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include "base/serialize.h"
#include "base/logger.h"
#include <assert.h>

#define HEADER_SIZE			6
#define MESSAGE_CTRL_CHAR	123

namespace mr
{

enum {TYPE_NONE, TYPE_REQUEST, TYPE_RESPONSE, TYPE_DATA};

class Message 
{
public:
	//Constructor with message header
	Message()
	{
		type=TYPE_NONE;
	}
	///contents is the actual payload of the message
	Message(const string& contents,int typ)
	{
		type=typ;
		contentSize=(unsigned int)contents.size();
		char header[10];
		int cont=0;
		writeChar(header,cont,MESSAGE_CTRL_CHAR);// (1 byte)
		writeChar(header,cont,type);
		writeLong(header,cont,contentSize);
		assert(cont==HEADER_SIZE);
		buffer.append(header,cont);
		buffer.append(contents);
	}
	virtual ~Message(){;}
	inline bool isResponse()const {return type==TYPE_RESPONSE;}
	inline bool isRequest()const  {return type==TYPE_REQUEST;}
	inline bool isData()const  {return type==TYPE_DATA;}

	///call these functions to add incoming bytes and conform message
	///the header will not be recomputed, just added the first time
	//returns message OK,incomplete or error 
	bool addHeader(const char* header, unsigned int headerSize)
	{
		if(headerSize!=HEADER_SIZE)
			return false;

		int cont=0;
		unsigned char control;
		readChar(header,cont,control);
		if(control!=123)
			return false;

		readChar(header,cont,type);
		if(type<TYPE_REQUEST || type >TYPE_DATA)
			return false;

		readLong(header,cont,contentSize);
		return true;
	}
	bool addContents(const char* cont, unsigned int contSize)
	{
		if(contSize!=contentSize)
			return false;
		content=string(cont,contSize);
		return true;
	}
	///returns the whole buffer, including header, length and payload
	///useful for sending through the socket
	const char* getBuffer(){return buffer.data();}
	unsigned int bufferSize(){return (unsigned int)buffer.size();}
	///returns the content: only the payload
	///useful for extracting when received
	string getContent()const {return content;}
	inline unsigned int getContentSize()const{return contentSize;}

protected:
	string buffer;//outgoing
	string content;//incoming
	
	//header
	int type;
	unsigned int contentSize;
};

};