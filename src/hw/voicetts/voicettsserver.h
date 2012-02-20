/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  the MRCore group
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

#ifndef __VOICE_TTS_SERVER_H_
#define __VOICE_TTS_SERVER_H_

#include "net/server.h"
#include "hw/voicetts.h"
#include "base/streamstring.h"

namespace mr
{
class VoiceTTSServer : public Server
{
public:
	//Pointer to the robot we want to serve
	VoiceTTSServer(VoiceTTS* rob,string name):Server(name){voice=rob;}

protected:
	virtual string handleRequest(const string& msg)
	{
		if(msg.size()==0)return string("Error protocolo msg size=0");
		StreamString stream(msg);
		char command=-1;
		stream>>command;
		if(command==1)
		{
			string text;
			stream>>text;
			voice->say(text);
			return string();
		}
		return string("Error protocolo comand not recognized");
	}
	//do not handling data
	void handleData(const std::string &){}
	
	VoiceTTS* voice;

};

}; //end namespace mr

#endif
