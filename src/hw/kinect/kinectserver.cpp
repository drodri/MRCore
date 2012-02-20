#include "kinectserver.h"
#include "base/streamstring.h"
#include <iostream>


namespace mr
{
string KinectServer::handleRequest(const string& msg)
{
	if(msg.size()==0)return string("Error protocolo msg size=0");

	StreamString stream(msg);
	char command=-1;
	stream>>command;
	if(command==1)//getData
	{
		PointCloud las;
		laser->getData(las);
		StreamString str;
		str.write(&las);
		return str.getString();
	}
	return string("Error protocolo comand not recognized");
}
	
}; //Namespace mr
