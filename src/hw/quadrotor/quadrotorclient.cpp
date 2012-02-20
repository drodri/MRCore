#include "quadrotorclient.h"
#include <iostream>
#include "base/streamstring.h"
#include "base/datalog.h"

namespace mr
{

//Requests: 1 = takeOff, 2 land, 3:move, 4:getData
//Response. Pose
bool QuadrotorClient::takeOff()
{
	StreamString stream;
	stream<<(char)1;
	string request=stream.getString();
	string response;
	int ret=sendRequest(request,response,500);
	if(ret!=COMM_OK)
	{
		LOG_ERROR("Error request takeoff: "<<ret);
		return false;
	}
	else 
	{
		if(response.size()!=0)
		{
			LOG_ERROR("Error mesnaje takeoff response not empty "<<response);	
			return false;
		}
	}
	return true;
}
bool QuadrotorClient::land()
{
	StreamString stream;
	stream<<(char)2;
	string request=stream.getString();
	string response;
	int ret=sendRequest(request,response,500);
	if(ret!=COMM_OK)
	{
		LOG_ERROR("Error request land: "<<ret);
		return false;
	}
	else 
	{
		if(response.size()!=0)
		{
			LOG_ERROR("Error mesnaje land response not empty "<<response);	
			return false;
		}
	}
	return true;
}

bool QuadrotorClient::move(double up, double forward,double lateral,double rot)
{
	StreamString stream;
	stream<<(char)3<<up<<forward<<lateral<<rot;
	string request=stream.getString();
	string response;
	int ret=sendRequest(request,response,500);
	if(ret!=COMM_OK)
	{
		LOG_ERROR("Error request Move: "<<ret);
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
bool QuadrotorClient::getData(QuadrotorData& data)
{
	StreamString stream;
	stream<<(char)4;
	string request=stream.getString();
	string response;
	int ret=sendRequest(request,response,500);
	if(ret!=COMM_OK)
	{
		LOG_ERROR("Error request Move: "<<ret);
		return false;
	}
	else 
	{
		StreamString str(response);
		Object* ob=str.read();//FIXME stream class
		QuadrotorData* p=dynamic_cast<QuadrotorData*> (ob);
		if(p)
		{
			data=*p;
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
	return false;
}

}; //Namespace mr
