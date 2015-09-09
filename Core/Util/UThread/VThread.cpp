#ifndef WIN32
#include <unistd.h>
#endif

#include "VThread.h"



CVThread::CVThread(void)
{
}

CVThread::~CVThread(void)
{
}

long CVThread::GetThreadId()
{
#ifdef WIN32
	return ::GetCurrentThreadId();
#endif
}

void CVThread::Sleep(long milliseconds)
{
#ifdef WIN32
	::Sleep(milliseconds);
#else
	usleep(milliseconds*1000);
#endif
}

#ifdef WIN32
int CVThread::StartThread( int flag )
{
	long id;

	//if( flag == SUSPENDED )
	//	flag = CREATE_SUSPENDED

	m_handle = ::CreateThread(NULL, 0, ThreadFunc, this, flag, (DWORD*)&id );

//	::SetThreadPriority( m_handle,THREAD_PRIORITY_NORMAL  );//THREAD_PRIORITY_HIGHEST


	m_eBegin.Create();
	m_eEnd.Create();

	return id;


}
#else
int CVThread::StartThread( int flag )
{ 
	// Create the thread using POSIX routines. 
	pthread_attr_t  attr; 
//	pthread_t       posixThreadID; 
	int             returnVal; 

	returnVal = pthread_attr_init(&attr); 
//	assert(!returnVal);
	returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
//	assert(!returnVal);

	int     threadError = pthread_create( &m_threadId, &attr, &ThreadFunc, this); 

	returnVal = pthread_attr_destroy(&attr); 
//	assert(!returnVal);
	if (threadError != 0) 
	{ 
		// Report an error. 
	} 
} 
#endif



int CVThread::ResumeThread()
{
int	ret = -1;
#ifdef WIN32
	ret = ::ResumeThread( m_handle );
#endif
	return ret;

}


int CVThread::SuspendThread()
{
int	ret = -1;
#ifdef WIN32
	ret = ::SuspendThread( m_handle );
#endif

	return ret;

}




#ifdef WIN32

void CVThread::SetEventBegin()
{
#ifdef WIN32
	m_eBegin.Set();
#endif
}

void CVThread::SetEventEnd()
{
#ifdef WIN32
	m_eEnd.Set();
#endif
}

long CVThread::WaitEventBegin( long milliseconds )
{
#ifdef WIN32
	return m_eBegin.Wait( milliseconds );
#else
	return( -1 );
#endif
}

long CVThread::WaitEventEnd( long milliseconds )
{
#ifdef WIN32
	return m_eEnd.Wait( milliseconds );
#else
	return( -1 );
#endif
}
#endif
/*
HANDLE WINAPI CreateEvent(
__in_opt  LPSECURITY_ATTRIBUTES lpEventAttributes,
__in      BOOL bManualReset,
__in      BOOL bInitialState,
__in_opt  LPCTSTR lpName
);

DWORD WINAPI ResumeThread(
__in  HANDLE hThread
);
*/

