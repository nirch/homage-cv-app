#include "VEvent.h"

CVEvent::CVEvent(void)
{
}

CVEvent::~CVEvent(void)
{
}


void CVEvent::Create( LPSECURITY_ATTRIBUTES semaphoreAtt, int manualReset, int initialState, char *name)
{
	m_handle = CreateEvent( semaphoreAtt, manualReset, initialState, name );
}

void CVEvent::Create( char *name )
{
	m_handle = CreateEvent( NULL, FALSE, FALSE, name );
}

int CVEvent::Set()
{
	 return ::SetEvent( m_handle);
}


long CVEvent::Wait( long milliseconds )
{
	return ::WaitForSingleObject( m_handle, milliseconds);
}

