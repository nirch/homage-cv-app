#ifndef THREAD_H
#define THREAD_H

#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#error _WIN32 must be defined before you include thread.h
#endif


#include "UThread/VEvent.h"


class CVThread
{
	static DWORD WINAPI ThreadFunc(LPVOID pv)
	{
		try
		{
		(reinterpret_cast<CVThread *>(pv))->Run();
		}
		catch(...)
		{
		}
	return 0;
	}
public:
//	typedef DWORD threadid;



	CVThread(void);

	virtual ~CVThread(void);

	static long GetThreadId();
	
	static void Sleep(long milliseconds=1);
	
	long StartThread( int flag=0 );	// flag == CREATE_SUSPENDED

	int ResumeThread();

	int SuspendThread();
	
	virtual void Run()=0;

	HANDLE m_handle;




	void SetEventBegin();
	long WaitEventBegin(  long milliseconds = INFINITE  );

	void SetEventEnd();
	long WaitEventEnd( long milliseconds = INFINITE  );



	CVEvent	m_eBegin;
	CVEvent	m_eEnd;

};

#endif





