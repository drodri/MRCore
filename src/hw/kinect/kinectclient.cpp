#include "kinectclient.h"
#include <iostream>
#include "base/streamstring.h"

namespace mr
{

//Requests: 1 = GetPose, 2 = Move
//Response. Pose2D
bool KinectClient::getData(PointCloud& las)
{
	string request(1,1);
	string response;//protocol character=1 means Ping
	int ret=sendRequest(request,response,1000);
	if(ret!=COMM_OK)
	{
		return false;
	}
	else 
	{
		StreamString str(response);
		Object* ob=str.read();//FIXME stream class
		PointCloud* p=dynamic_cast<PointCloud*> (ob);
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
