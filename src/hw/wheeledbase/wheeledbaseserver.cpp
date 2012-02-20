#include "wheeledbaseserver.h"
#include "base/streamstring.h"
#include <iostream>


namespace mr
{
string WheeledBaseServer::handleRequest(const string& msg)
{

	if(msg.size()==0)return string("Error protocolo msg size=0");

	StreamString stream(msg);
	char command=-1;
	stream>>command;
	if(command==1)//getPose
	{
		Odometry p;
		robot->getOdometry(p);
		StreamString str;
		str.write(&p);
		return str.getString();//FIXME: Complete Stream class
	}
	else if(command==2)//move
	{

		double va=0,vg=0;
		stream>>va>>vg;
		robot->move(va,vg);
		return string();
	}
	return string("Error protocolo comand not recognized");
}
	
}; //Namespace mr
