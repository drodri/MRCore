// Logger.h: interface for the Utils class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "system/mutex.h"

using namespace std;
enum {LOGGER_ERROR,LOGGER_WARNING,LOGGER_INFO,LOGGER_PRINT};

//TODO, complete log level

#define LOG_ERROR(msg)	Logger(LOGGER_ERROR).Log()<<msg
#define LOG_WARNING(msg)	Logger(LOGGER_WARNING).Log()<<msg
#define LOG_INFO(msg)		Logger(LOGGER_INFO).Log()<<msg
#define LOG_PRINT(msg)	Logger(LOGGER_PRINT).Log()<<msg
#define LOG_DEBUG(msg)	

namespace mr
{

class Logger  
{
public:
	Logger(int type);
	virtual ~Logger();

	std::ostringstream& Log(){return os;}
	///Can be used to redirect std::cout to ofstream. Using ostringstream,
	static void SetFileStream(string filename="");
	static void SetStringStream(bool set=true){use_string_stream()=set;}

	static std::string GetString()
	{
		m().Lock();
		string str=string_stream().str();
		string_stream().str("");
		m().Unlock();
		return str;
	}

protected:
	std::ostringstream os;
	static std::ofstream& file()
	{
		static ofstream f;
		return f;
	}
	static ostringstream&  string_stream()
	{
		static ostringstream str;
		return str;
	}
	static bool& use_string_stream()
	{
		static bool use_str=false;
		return use_str;
	}
	static Mutex& m()
	{
		static Mutex mu;//all Logger instances uses the same Mutex
		return mu;
	}

};

};