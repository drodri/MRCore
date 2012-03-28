#include "lasersensorfile.h"
#include <iostream>
#include "base/streamstring.h"
#include "base/datalog.h"

namespace mr
{

//Requests: 1 = GetPose, 2 = Move
//Response. Pose2D
bool LaserSensorFile::getData(LaserData& las)
{
	Data* d=dataLog->requestData(this);
	if(!d)
		return false;

	LaserData* p=dynamic_cast<LaserData*> (d);
	if(!p)
		return false;

	las=*p;
	delete p;
	return true;
}
	//save to datalog
bool LaserSensorFile::setData(const LaserData& las)
{
	LaserData aux=las;
	return dataLog->sendData(this,&aux);
	
}


}; //Namespace mr
