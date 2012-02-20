// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__598631F8_DCFA_41AC_B1A4_9DB573E7F896__INCLUDED_)
#define AFX_THREAD_H__598631F8_DCFA_41AC_B1A4_9DB573E7F896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///At the moment this class is useful only to start a thread from a member function inside the same class

#ifdef _WIN32
	#include <process.h>
	#define  ThreadType HANDLE
	#define ThreadReturnType unsigned int __stdcall
	#define INVALID_HANDLE 0
#else
	#include <pthread.h>
	//#define  ThreadType  pthread_t
	typedef pthread_t ThreadType;
	#define ThreadReturnType void*
	#define INVALID_HANDLE -1
#endif

#include "base/logger.h"

template <class T>
class Thread  
{
	typedef void* (T::* T_function)(void*); 
	struct ThreadData
	{
		T_function function; 
		T* object;
		void* data;
	};
public:	
	Thread();

	void Start(T_function f, T* pthis ,void* d=NULL);
	void Join();
	ThreadType getId(){return threadId;}
private:
	ThreadType threadId;
	static ThreadReturnType Launch(void *p);
};

#include "thread.hpp"

#endif // !defined(AFX_THREAD_H__598631F8_DCFA_41AC_B1A4_9DB573E7F896__INCLUDED_)
