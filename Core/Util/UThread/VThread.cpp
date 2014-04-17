#include "VThread.h"

CVThread::CVThread(void)
{
}

CVThread::~CVThread(void)
{
}

long CVThread::GetThreadId()
{
	return ::GetCurrentThreadId();
}

void CVThread::Sleep(long milliseconds)
{
	::Sleep(milliseconds);
}

long CVThread::StartThread( int flag )
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



int CVThread::ResumeThread()
{
int	ret;

	ret = ::ResumeThread( m_handle );

	return ret;
}


int CVThread::SuspendThread()
{
int	ret;

	ret = ::SuspendThread( m_handle );

	return ret;
}






void CVThread::SetEventBegin()
{
	m_eBegin.Set();
}

void CVThread::SetEventEnd()
{
	m_eEnd.Set();
}

long CVThread::WaitEventBegin( long milliseconds )
{
	return m_eBegin.Wait( milliseconds );
}

long CVThread::WaitEventEnd( long milliseconds )
{
	return m_eEnd.Wait( milliseconds );

}
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

