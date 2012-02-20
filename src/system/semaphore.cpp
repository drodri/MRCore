
#include "semaphore.h"

#ifndef _WIN32
#include <errno.h>
#include <time.h>
#include <memory.h>
#endif

namespace mr
{

ostream& operator<<(ostream& os, const Semaphore& s)
{
	#ifdef _WIN32
	
	#else
//	int sval;
//	sem_t sem=s.sem;
//	int r= sem_getvalue(&sem, &sval);
//	if(r!=0)
//		os<<"Error, could not get sem value ";
//	else
//		os<<"Sem value: "<<sval;
	#endif
	return os;
}
	

Semaphore::Semaphore(int init,int max)
{
	#ifdef _WIN32
	  ghSemaphore = CreateSemaphore(NULL, init, max, NULL);
	  if (ghSemaphore == NULL)
	  {
		cout<<"*** ERROR - CreateSemaphore() failed \n"<<endl;
	  }
	#else
	memset( &sem, 0, sizeof( sem_t ) );
	if (sem_init(&sem, 0, init) == -1) 
		cout<<"*** ERROR - CreateSemaphore() failed \n"<<endl;

	#endif
}

Semaphore::~Semaphore()
{
	#ifdef _WIN32
		 CloseHandle(ghSemaphore);
	#else
		sem_close(&sem);
	#endif
}

bool Semaphore::wait(int timeout)
{
	#ifdef _WIN32
		if(timeout<0)
			timeout=INFINITE;
		 int ret=WaitForSingleObject(ghSemaphore, timeout);
		 if(ret!=WAIT_OBJECT_0)
			return false;
		 return true;
	#else
		if(timeout<0)
		{
			if(-1==sem_wait(&sem))
				return false;
			return true;
		}
		else
		{
			struct timespec ts;
			if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
			{
               cout<<"clock_gettime"<<endl;
               return false;
		   }
		   int secs=timeout/1000;
		   ts.tv_sec+=secs;
		   timeout-=secs*1000;
			ts.tv_nsec+=1e6*timeout;
			if(ts.tv_nsec>1e9)
			{
				ts.tv_nsec-=1e9;
				ts.tv_sec++;
			}
			int ret=sem_timedwait(&sem, &ts);
			if(-1==ret)
			{
				int err=errno;
				if(err==EINTR) cout<<"The call was interrupted by a signal handler."<<endl;
				else if(err==EINVAL) cout<<"sem is not a valid semaphore."<<endl;
				else if(err==EAGAIN)cout<<"The operation could not be performed without blocking"<<endl;
				else if(err==ETIMEDOUT) cout<<"The call timed out before the semaphore could be locked."<<endl;
				else cout<<"Unexpected error "<<err<<endl;
				return false;
			}
			return true;
		}	
	#endif
}

void Semaphore::signal()
{
	#ifdef _WIN32
		 ReleaseSemaphore(ghSemaphore, 1, NULL);
	#else
		if (sem_post(&sem) == -1)
		{
		cout<<"*** ERROR - SemPost"<<endl;
	 
	  }
	#endif
}
};
