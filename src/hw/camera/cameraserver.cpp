#include "cameraserver.h"
#include "base/streamstring.h"
#include <iostream>


namespace mr
{
string CameraServer::handleRequest(const string& msg)
{
	if(msg.size()==0)return string("Error protocolo msg size=0");

	StreamString stream(msg);
	char command=-1;
	stream>>command;
	if(command==1)//getData
	{
		Image image(0,0);
		camera->getImage(image);
		StreamString str;
	//	str.write(&las);
		image.writeToStream(str);
		return str.getString();
	}
	return string("Error protocolo comand not recognized");
}
	
}; //Namespace mr