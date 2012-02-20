// Mutex.h: interface for the Mutex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUTEX_H__EC32BC43_0DB5_4284_9D26_730A50ECF4B5__INCLUDED_)
#define AFX_MUTEX_H__EC32BC43_0DB5_4284_9D26_730A50ECF4B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _WIN32
//	#include <afx.h>
	#define _WINSOCKAPI_ //prevent the inclusion of sockets
	#include <windows.h>
#else
	#include <pthread.h>
#endif

namespace mr
{
///non-copyable, non movable
class Mutex  
{
public:
	Mutex();
	virtual ~Mutex();
	
	void Unlock();
	void Lock();


protected:
#ifdef _WIN32
	CRITICAL_SECTION m;
#else
	pthread_mutex_t m;
#endif

private:
	Mutex(const Mutex& m);///non-copyable, non movable
	void operator=(const Mutex& m);///non-copyable, non movable

};

};
#endif // !defined(AFX_MUTEX_H__EC32BC43_0DB5_4284_9D26_730A50ECF4B5__INCLUDED_)
