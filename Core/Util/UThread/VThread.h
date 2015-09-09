#ifndef THREAD_H
#define THREAD_H

#ifdef WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#include "UThread/VEvent.h"
#else
#include <pthread.h>
#endif





class CVThread
{
#ifdef WIN32
	static DWORD WINAPI ThreadFunc(LPVOID pv)
#else
    static void *  ThreadFunc( void * pv)
#endif
	{
//		try
//		{
		(reinterpret_cast<CVThread *>(pv))->Run();
//		}
//		catch(...)
//		{
//		}
	return 0;
	}
public:
//	typedef DWORD threadid;



	CVThread(void);

	virtual ~CVThread(void);

	static long GetThreadId();
	
	static void Sleep(long milliseconds=1);
	
	int StartThread( int flag=0 );	// flag == CREATE_SUSPENDED

	int ResumeThread();

	int SuspendThread();
	
	virtual void Run()=0;




#ifdef _WIN32
	void SetEventBegin();
	long WaitEventBegin(  long milliseconds = INFINITE  );

	void SetEventEnd();
	long WaitEventEnd( long milliseconds = INFINITE  );
#endif

#ifdef _WIN32
	HANDLE m_handle;

	CVEvent	m_eBegin;
	CVEvent	m_eEnd;
#else 
	pthread_t       m_threadId;
#endif

};

#endif





