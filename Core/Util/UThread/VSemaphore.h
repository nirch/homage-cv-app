#ifndef VIM_SEMAPHORE_H
#define VIM_SEMAPHORE_H

#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#error _WIN32 must be defined before you include criticalsection.h
#endif

class CVSemaphore
{
public:
	typedef HANDLE handle;
//	typedef void *handle;

	handle m_hSemaphore;

	CVSemaphore(void);

	~CVSemaphore(void);

//	void Create( LPSECURITY_ATTRIBUTES semaphroeAtt, long initialCount, long maxCount, LPCTSTR name);
	void Create( LPSECURITY_ATTRIBUTES semaphroeAtt, long initialCount, long maxCount, char * name);

	long WaitForSingleObject(long milliseconds = INFINITE );

	int Release(long releaseCount, long* previousCount = NULL );
};

#endif