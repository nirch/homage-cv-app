#ifndef VIM_SEMAPHORE_H
#define VIM_SEMAPHORE_H

#include <stdio.h>

#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
//#include <fcntl.h>	/* For O_* constants */
//#include <sys/stat.h>		/* For mode constants */
#include <semaphore.h>
#endif

class CVSemaphore
{
public:



	CVSemaphore(void);

	~CVSemaphore(void);

#ifdef _WIN32
//	void Create( LPSECURITY_ATTRIBUTES semaphroeAtt, long initialCount, long maxCount, LPCTSTR name);
	void Create( LPSECURITY_ATTRIBUTES semaphroeAtt, long initialCount, long maxCount, char * name);

	long WaitForSingleObject(long milliseconds = INFINITE );

	
#endif


	void Create( long initialCount, long maxCount );

	long Wait( long milliseconds = 0xFFFFFFFF );

	int Release(long releaseCount, long* previousCount = NULL );

#ifdef _WIN32


	typedef HANDLE handle;
//	typedef void *handle;

	handle m_hSemaphore;
#else
	sem_t m_sem;
#endif

};

#endif