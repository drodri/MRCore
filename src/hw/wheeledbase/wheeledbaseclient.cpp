#include "wheeledbaseclient.h"
#include <iostream>
#include "base/streamstring.h"
#include "base/datalog.h"

namespace mr
{

//Requests: 1 = GetPose, 2 = Move
//Response. Pose2D
bool WheeledBaseClient::getOdometry(Odometry& pose)
{
	string request(1,1);
	string response;//protocol character=1 means Ping
	int ret=sendRequest(request,response,50);
	if(ret!=COMM_OK)
	{
		return false;
	}
	else 
	{
		StreamString str(response);
		Object* ob=str.read();//FIXME stream class
		Odometry* p=dynamic_cast<Odometry*> (ob);
		if(p)
			pose=*p;	
		else
		{
			LOG_ERROR("Error message getPose received not odometry");	
			return false;
		}
		delete ob;
	}

	return true;
}
bool WheeledBaseClient::move(double speed, double rot)
{
	StreamString stream;
	stream<<(char)2<<speed<<rot;
	string request=stream.getString();
	string response;//protocol character=1 means Ping
	int ret=sendRequest(request,response,50);
	if(ret!=COMM_OK)
	{
		return false;
	}
	else 
	{
		if(response.size()!=0)
		{
			LOG_ERROR("Error mesnaje Move response not empty "<<response);	
			return false;
		}
	}

	return true;
}


}; //Namespace mr
