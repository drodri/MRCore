// Mutex.cpp: implementation of the Mutex class.
//
//////////////////////////////////////////////////////////////////////

#include "mutex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace mr;

Mutex::Mutex()
{
#ifdef _WIN32
 InitializeCriticalSection( &m);
#else
   pthread_mutex_init( &m, 0 );
#endif
}

Mutex::~Mutex()
{
#ifdef _WIN32
	DeleteCriticalSection( &m);
#else
	pthread_mutex_destroy( &m );
#endif
}

void Mutex::Lock()
{
#ifdef _WIN32
	EnterCriticalSection( &m );
#else
 pthread_mutex_lock( &m );
#endif
}

void Mutex::Unlock()
{
#ifdef _WIN32
	 LeaveCriticalSection( &m );
#else
	pthread_mutex_unlock( &m );
#endif
}
