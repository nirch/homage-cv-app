//#define _GPLOG

#include "Ulog/Log.h"
#include "VMutex.h"




CVMutex::CVMutex()
{
#ifdef WIN32
	m_mutex = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

#else
	GPLOGF(("<CVMutex ..."));
	int ret = pthread_mutex_init(&m_mutex, NULL);
	GPLOGF((" %d>", ret ));

#endif
}

CVMutex::~CVMutex()
{
#ifdef WIN32
	CloseHandle(m_mutex);
#else
	pthread_mutex_destroy( &m_mutex );

#endif
}

void CVMutex::Lock()
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

void CVMutex::Unlock()
{
#ifdef WIN32
	ReleaseMutex( m_mutex );

#else
	GPLOGF(("<Unlock ..."));
	int ret = pthread_mutex_unlock( &m_mutex );
	GPLOGF((" %d>", ret));

#endif
}

