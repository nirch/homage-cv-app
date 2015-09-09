#ifndef VIM_CONDITION_MUTEX_H
#define VIM_CONDITION_MUTEX_H



#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#include <pthread.h>
#endif







class CVConditionMutex
{
#ifdef WIN32
	HANDLE m_mutex;
#else
	pthread_mutex_t m_mutex;
	pthread_cond_t  m_cond;
#endif
public:
	CVConditionMutex();

	~CVConditionMutex();

	void Lock();
	
	void Unlock();

	int Wait();

	int Wait( int msec );

	int Signal();
};

#endif