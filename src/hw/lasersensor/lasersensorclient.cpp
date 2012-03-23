#include "lasersensorclient.h"
#include <iostream>
#include "base/streamstring.h"

namespace mr
{

//Requests: 1 = GetPose, 2 = Move
//Response. Pose2D
bool LaserSensorClient::getData(LaserData& las)
{
	string request(1,1);
	string response;//protocol character=1 means Ping
	int ret=sendRequest(request,response,500);
	if(ret!=COMM_OK)
	{
		return false;
	}
	else 
	{
		StreamString str(response);
		Object* ob=str.read();//FIXME stream class
		LaserData* p=dynamic_cast<LaserData*> (ob);
		if(p)
		{
			las=*p;
			delete p;
			return true;
		}
		else
		{
			LOG_ERROR("Error message getLaserData received something but not LaserData");	
			delete ob;
			return false;
		}
	}

	return true;
}



}; //Namespace mr
