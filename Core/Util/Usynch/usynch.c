#include "usynch.h"

#if defined WIN32

int synch_init( LPCRITICAL_SECTION lpCS )
{
	__try 
	{ 
		InitializeCriticalSection( lpCS ); 
	}
    __except( GetExceptionCode() == STATUS_NO_MEMORY ) 
	{ 
		return SYNCH_ERR; 
	}
	return SYNCH_OK;
}

int synch_enter( LPCRITICAL_SECTION lpCS )
{
	__try 
	{ 
		EnterCriticalSection( lpCS ); 
	}
    __except( GetExceptionCode() == STATUS_INVALID_HANDLE ) 
	{ 
		return SYNCH_ERR; 
	}
	return SYNCH_OK;
}

#endif // WIN32
