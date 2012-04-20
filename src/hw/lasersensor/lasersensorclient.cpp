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
		if(str.read(&las))
			return true;
		else
		{
			LOG_ERROR("Error message getLaserData received something but not LaserData");	
			return false;
		}
	}

	return false;
}



}; //Namespace mr
