#include "quadrotorserver.h"
#include "base/streamstring.h"
#include <iostream>


namespace mr
{
string QuadrotorServer::handleRequest(const string& msg)
{

	if(msg.size()==0)return string("Error protocolo msg size=0");

	StreamString stream(msg);
	char command=-1;
	stream>>command;
	if(command==1)//takeoff
	{
		robot->takeOff();
		return string();
	} 
	else if(command==2)//land
	{
		robot->land();
		return string();
	}else if(command==3)//move
	{
		double up=0,forward=0,lateral=0,rot=0;
		stream>>up>>forward>>lateral>>rot;
		robot->move(up,forward,lateral,rot);
		return string();
	} 
	else if(command==4)//getData
	{
		return string();
	}
	return string("Error protocolo comand not recognized");
}
	
}; //Namespace mr
