#include "lasersensor3dclient.h"
#include <iostream>
#include "base/streamstring.h"

namespace mr
{

//Requests: 1 = GetPose, 2 = Move
//Response. Pose2D
bool LaserSensor3DClient::getData(LaserData3D& las)
{
	string request(1,1);
	string response;//protocol character=1 means Ping
	int ret=sendRequest(request,response,6000);
	if(ret!=COMM_OK)
	{
		LOG_ERROR("Error request LaserData: "<<ret);
		return false;
	}
	else 
	{
		StreamString str(response);
		Object* ob=str.read();//FIXME stream class
		LaserData3D* p=dynamic_cast<LaserData3D*> (ob);
		if(p)
		{
			las=*p;
			delete p;
			return true;
		}
		else
		{
			LOG_ERROR("Error mesnaje getLaserData");	
			delete ob;
			return false;
		}
	}

	return true;
}



}; //Namespace mr
