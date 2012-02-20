#pragma once

#include <iostream>
#include "../config.h"

#ifdef WIN32
		#define _WINSOCKAPI_ //prevent the inclusion of sockets
		#include <windows.h>
		typedef DWORD tictoc;
#else
	#ifdef HAVE_CLOCK_GETTIME
		#include <time.h>
		typedef struct timespec tictoc;
	#else
		#ifdef HAVE_GETTIMEOFDAY
			#include <sys/time.h>
			typedef struct timeval tictoc;
		#else
			#include <ctime>
			typedef time_t tictoc;
		#endif
	#endif
#endif





using namespace std;

namespace mr{
class MRTime
{
	friend ostream& operator<<(ostream& os, const MRTime& t);

public:
	MRTime(void){}
	~MRTime(void){}

	//update internal value with the current machine time
	void tic();
	//returns the number of elapsed milliseconds since last call to now()
	//if micros=true returns the number of elapsed microseconds
	long int toc(bool micros=false);

	MRTime operator-(const MRTime& t);
	inline bool operator>(const MRTime& t)
	{
		return ((seconds>t.seconds) || (seconds==t.seconds && microseconds>t.microseconds));
	}
	inline bool operator==(const MRTime& t)
	{
		return (seconds==t.seconds && microseconds == t.microseconds);
	}
	inline bool operator<(const MRTime& t)
	{
		return ((seconds<t.seconds) || (seconds==t.seconds && microseconds<t.microseconds));
	}

//	struct timeb t1,t2;
	int seconds;
	int microseconds;
	//If true, microseconds will be shown, otherwise, only milliseconds
	static bool microPrecision;
	
	void   precistic();   /* start timing. */
	double precistoc();   /* stop  timing. */

private:
	tictoc tv;
	
};

}
