// Utils.cpp: implementation of the Utils class.
//
//////////////////////////////////////////////////////////////////////

#include "logger.h" 
#include <algorithm>
#include <iostream>
#include <fstream>

#ifdef _WIN32
#define _WINSOCKAPI_ //prevent the inclusion of sockets
#include <windows.h>
#else

#endif

using namespace mr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//Mutex Logger::m;
//std::ofstream Logger::file;
//std::ostringstream Logger::string_stream;
//bool Logger::use_string_stream=false;

Logger::Logger(int type)
{
	m().Lock();
	#ifdef _WIN32
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if(type==LOGGER_ERROR)//ROJO
	{		
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY );
	}
	if(type==LOGGER_WARNING)//VERDE
	{		
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
	}
	if(type==LOGGER_INFO)//VERDE
	{		
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN| FOREGROUND_INTENSITY );
	}
	if(type==LOGGER_PRINT)//CYAN
	{		
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		//default=WHITE
	}
	#else
	std::string color;
	if(type==LOGGER_ERROR)//ROJO
	{		
		color = "\e[1;31m"; 
	}
	if(type==LOGGER_WARNING)//VERDE
	{	
		color = "\e[1;35m"; 	
	}
	if(type==LOGGER_INFO)//MAGENTA
	{		
		color = "\e[1;32m"; 
	}
	if(type==LOGGER_PRINT)//CYAN
	{		
		color = "\e[1;36m"; 	
	}
	os<<color;
	#endif
}

Logger::~Logger()
{	 
	#ifndef _WIN32
		os << "\e[0m";
	#endif

		cout<<os.str()<<endl;
		if(file().good() && file().is_open())
			file()<<os.str()<<endl;
		if(use_string_stream())
			string_stream()<<os.str()<<endl;

	#ifdef _WIN32
		HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |FOREGROUND_GREEN);
	#endif
	m().Unlock();
}
void Logger::SetFileStream(std::string filename)
{
	if(filename=="")
		file().close();
	else
		file().open(filename.c_str());

}

