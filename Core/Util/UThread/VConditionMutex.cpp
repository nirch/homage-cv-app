//#define _GPLOG

#include "Ulog/Log.h"
#include "VConditionMutex.h"
//#include <unistd.h>
#include <time.h>



CVConditionMutex::CVConditionMutex()
{
#ifdef WIN32
	m_mutex = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

#else
	GPLOGF(("<CVConditionMutex ..."));
	int ret = pthread_mutex_init(&m_mutex, NULL);
	pthread_cond_init (&m_cond, NULL);
	GPLOGF((" %d>", ret ));

#endif
}

CVConditionMutex::~CVConditionMutex()
{
#ifdef WIN32
	CloseHandle(m_mutex);
#else
	pthread_mutex_destroy( &m_mutex );

#endif
}

void CVConditionMutex::Lock()
{
#ifdef WIN32
	DWORD  dwWaitResult = WaitForSingleObject( 
		m_mutex,    // handle to mutex
		INFINITE);  // no time-out interval
#else
	GPLOGF(("<Lock ..."));
	int ret = pthread_mutex_lock ( &m_mutex );
	GPLOGF((" %d>", ret));

#endif
}

void CVConditionMutex::Unlock()
{
#ifdef WIN32
	ReleaseMutex( m_mutex );

#else
	GPLOGF(("<Unlock ..."));
	int ret = pthread_mutex_unlock( &m_mutex );
	GPLOGF((" %d>", ret));

#endif
}



int CVConditionMutex::Wait()
{
#ifdef WIN32

	return( -1 );
#else
	GPLOGF(("<Unlock ..."));
	int ret = pthread_cond_wait(&m_cond, &m_mutex);

	GPLOGF((" %d>", ret));
	return( 1 );

#endif
}


int CVConditionMutex::Wait( int msec )
{
#ifdef WIN32

	return( -1 );
#else

	//pthread_timestruc_t to;
	struct timespec  tw;
	tw.tv_sec = time(NULL) + ( msec /1000 );
	tw.tv_nsec = msec%1000;



	GPLOGF(("<Unlock ..."));
	int ret = pthread_cond_timedwait(&m_cond, &m_mutex, &tw);

	GPLOGF((" %d>", ret));

	if( ret != 0 )
		return( -1 );

	return( ret );

#endif
}

int CVConditionMutex::Signal()
{
#ifdef WIN32
	return( -1 );

#else
	GPLOGF(("<Unlock ..."));
	int ret = pthread_cond_signal( &m_cond );

	GPLOGF((" %d>", ret));
	return( 1 );
#endif
}