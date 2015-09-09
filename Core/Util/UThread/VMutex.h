#ifndef VIM_MUTEX_H
#define VIM_MUTEX_H



#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#include <pthread.h>
#endif







class CVMutex
{
#ifdef WIN32
	HANDLE m_mutex;
#else
	pthread_mutex_t m_mutex;
#endif
public:
	CVMutex();

	~CVMutex();

	void Lock();
	
	void Unlock();
};

#endif