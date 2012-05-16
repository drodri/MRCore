#include "wheeledbasefile.h"
#include <iostream>
#include "base/streamstring.h"
#include "base/datalog.h"

namespace mr
{


bool WheeledBaseFile::getOdometry(Odometry& pose)
{
	Data* d=dataLog->requestData(this);
	if(!d)
		return false;

	Odometry* p=dynamic_cast<Odometry*> (d);
	if(!p)
		return false;

	pose=*p;
	delete p;

	return true;
}
bool WheeledBaseFile::move(double speed, double rot)
{

	return true;
}
	//save to datalog
bool WheeledBaseFile::setOdometry(const Odometry& pose)
{
	Odometry aux=pose;
	return dataLog->sendData(this,&aux);
	
}	

}; //Namespace mr