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
#ifndef __MRCORE__STREAMFILE__H
#define __MRCORE__STREAMFILE__H

#include "stream.h"
#include <fstream>

namespace mr
{

class StreamFile :public Stream
{
public:
	//if filename is not provided, we will use a in memory buffer (stringstream)
	StreamFile(string filename,bool read)
	{
		if(read)
			stream=new fstream(filename.c_str(),ios::in|ios::binary);
		else
			stream=new fstream(filename.c_str(),ios::out|ios::binary);
	}
	inline bool good(){return stream->good();}
	~StreamFile()
	{
		((fstream*)stream)->close();
		//delete stream; el destructor base ya lo hace
	}

protected:
//non-copyable, non movable
	StreamFile(const StreamFile& str);
	void operator =(const StreamFile& str);
};



};
#endif  //__MRCORE__OBJECT_H
