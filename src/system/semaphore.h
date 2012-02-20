#pragma once

#ifdef _WIN32
//	#include <afx.h>
	#define _WINSOCKAPI_ //prevent the inclusion of sockets
	#include <windows.h>
#else
	#include <pthread.h>
	#include <semaphore.h>
#endif
#include <iostream>
using namespace std;

namespace mr
{
///non-copyable, non movable
class Semaphore 
{
	friend ostream& operator<<(ostream& os, const Semaphore& s);
public:
	Semaphore(int init,int max);
	virtual ~Semaphore();
	
	void signal();
	///\return 0 if error, 1 if success
	///\param timeout < 0 means block forever
	bool wait(int timeout=-1);//negative means forever
	  

protected:
#ifdef _WIN32
	HANDLE ghSemaphore;          // Windows semaphore
#else
	sem_t sem;
#endif

private:
	Semaphore(const Semaphore& m);///non-copyable, non movable
	void operator=(const Semaphore& m);///non-copyable, non movable

};

};
