
template <class T>
Thread<T>::Thread()
{
	threadId=INVALID_HANDLE;
}

template <class T>
void Thread<T>::Start(T_function f, T* pthis ,void* d)
{
	//Copy the data into the structure
	ThreadData* data=new ThreadData;
	data->function=f;
	data->object=pthis;
	data->data=d;			

	#ifdef _WIN32
	//	threadId=(HANDLE)_beginthread(Launch, 0, data);
		threadId=(HANDLE)_beginthreadex(NULL,0,Launch,data,0,NULL);
	#else
		pthread_create(&threadId,NULL,Launch, (void *) data);
	#endif
}
template <class T>
void Thread<T>::Join()
{
	if(threadId==INVALID_HANDLE)return; //Do nothing
	#ifdef _WIN32
		int ret=WaitForSingleObject( threadId, 1000 ); 
		if(ret!=WAIT_OBJECT_0)
		{
			if(ret==WAIT_TIMEOUT)
				LOG_ERROR("-------------Error join TIMEOUT threadID: "<<threadId);
			else if(ret==WAIT_ABANDONED)
				LOG_ERROR("-------------Error join ABANDONED "<<ret<<" therdID: "<<threadId);
			else
				LOG_ERROR("-------------BAD  Error join! "<<ret<<" therdID: "<<threadId);
		}
		else 
		{
			threadId=INVALID_HANDLE;
		}
	#else
		pthread_join(threadId,NULL);
	#endif
}

template <class T>
#ifdef _WIN32
	unsigned int __stdcall Thread<T>::Launch(void *p)
#else
	void* Thread<T>::Launch(void *p)
#endif
{
	ThreadData* data=(ThreadData*)p;
	T_function function=data->function;
	T* object=data->object;
	void* datos=data->data;
	delete data;
	((*object).*function)(datos);
	#ifdef _WIN32
		return 1;
	#else
		return 0;
	#endif
}

