#include "lasersensorserver.h"
#include "base/streamstring.h"
#include <iostream>


namespace mr
{
string LaserSensorServer::handleRequest(const string& msg)
{
	if(msg.size()==0)return string("Error protocolo msg size=0");

	StreamString stream(msg);
	char command=-1;
	stream>>command;
	if(command==1)//getData
	{
		LaserData las;
		laser->getData(las);
		StreamString str;
		str.write(&las);
		return str.getString();
	}
	return string("Error protocolo comand not recognized");
}
	
}; //Namespace mr