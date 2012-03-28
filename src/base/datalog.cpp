#include "datalog.h"
#include "datasource.h"
#include <iostream>
#include "base/streamfile.h"
#include "base/logger.h"

namespace mr
{
DataLog::DataLog()
{

}
bool DataLog::registerHW(DataSource*  hw,string name,bool read)
{
	string filename=folder+"/"+name;
	filename+=".log";
	StreamFile* f=new StreamFile(filename,read);
	if(!f->good())
	{
		LOG_ERROR("Cannot open data log file");
		delete f;
		return false;
	}

	mapFiles[hw]=f;
	return true;
}
bool DataLog::unregisterHW(DataSource*  hw)
{
	if(mapFiles.find(hw)!=mapFiles.end())
	{
		delete	mapFiles[hw];
		mapFiles.erase(hw);
		return true;
	}
	return false;
}
bool DataLog::sendData(DataSource* requester,Data* data)
{
	mapFiles[requester]->write(data);//TODO: COmplete
	return true;
}
Data* DataLog::requestData(DataSource* requester)
{
	Object* obj=mapFiles[requester]->read();//TODO: COmplete
	Data* d=dynamic_cast<Data*>( obj);
	return d;
}
}; //Namespace mr