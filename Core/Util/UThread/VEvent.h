#ifndef VIM_EVENT_H
#define VIM_EVENT_H

#ifdef _WIN32
#ifndef _WINDOWS_
#include <windows.h>
#endif
#else
#error _WIN32 must be defined before you include criticalsection.h
#endif

class CVEvent
{
public:
	typedef HANDLE handle;


	handle m_handle;

	CVEvent(void);

	~CVEvent(void);

	void Create( LPSECURITY_ATTRIBUTES semaphoreAtt, int manualReset, int initialState, char *name);

	void Create( char *name = NULL );

	int Set();

	long Wait(long milliseconds = INFINITE );
};

#endif