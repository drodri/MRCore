/**********************************************************************
 *
 * This code is part of the MRcore projec
 * Author:  the MRCore group
 *
 * MRcore is licenced under the Common Creative License,
 * Attribution-NonCommercial-ShareAlike 3.0
 *
 * You are free:
 *   - to Share - to copy, distribute and transmit the work
 *   - to Remix - to adapt the work
 *
 * Under the following conditions:
 *   - Attribution. You must attribute the work in the manner specified
 *     by the author or licensor (but not in any way that suggests that
 *     they endorse you or your use of the work).
 *   - Noncommercial. You may not use this work for commercial purposes.
 *   - Share Alike. If you alter, transform, or build upon this work,
 *     you may distribute the resulting work only under the same or
 *     similar license to this one.
 *
 * Any of the above conditions can be waived if you get permission
 * from the copyright holder.  Nothing in this license impairs or
 * restricts the author's moral rights.
 *
 * It is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 **********************************************************************/

#ifndef __DATALOG_H_
#define __DATALOG_H_

#include "data/data.h"
#include "base/streamfile.h"
#include "system/filesystem.h"

#include <fstream>
using namespace std;
using namespace mr;


namespace mr
{
class DataSource;
class DataLog
{
	friend class DataSource;
public:
	bool isOpen(){return folder!="";}
	bool sendData(DataSource* requester,Data* data);
	Data* requestData(DataSource* requester);
	virtual bool open(string folder)=0;
protected:	
	bool registerHW(DataSource*  hw,string name,bool read);
	bool unregisterHW(DataSource*  hw);
	
	//each data log is strictly associated with one folder
	DataLog();	
	map<DataSource*, StreamFile*> mapFiles;
	string folder;

};
class DataLogOut: public DataLog
{
public:	
	//each data log is strictly associated with one folder
	DataLogOut(){}
	bool open(string f)
	{
		if(!createDirectory(f))
			return false;

		folder=f;
		return true;
	}
	

};
class DataLogIn: public DataLog
{
public:	
	//each data log is strictly associated with one folder
	DataLogIn(){;}
	bool open(string f)
	{
		if(!existDirectory(f))
			return false;

		folder=f;
		return true;
	}
	

};

}; //end namespace mr

#endif
