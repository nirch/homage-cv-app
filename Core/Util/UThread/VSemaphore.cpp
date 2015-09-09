#include "VSemaphore.h"

CVSemaphore::CVSemaphore(void)
{
}

CVSemaphore::~CVSemaphore(void)
{
}

#ifdef _WIN32
//void CVSemaphore::Create(LPSECURITY_ATTRIBUTES semaphoreAtt, long initialCount, long maxCount, LPCTSTR name)
void CVSemaphore::Create(LPSECURITY_ATTRIBUTES semaphoreAtt, long initialCount, long maxCount, char *name)
{
	m_hSemaphore = ::CreateSemaphore(semaphoreAtt, initialCount, maxCount, name);
}
#endif


void CVSemaphore::Create( long initialCount, long maxCount )
{
#ifdef _WIN32
	m_hSemaphore = ::CreateSemaphore( NULL, initialCount, maxCount, NULL );
#else

	sem_init( &m_sem, 0, initialCount);
#endif

}

#ifdef _WIN32
long CVSemaphore::WaitForSingleObject(long milliseconds)
{
	return ::WaitForSingleObject(m_hSemaphore, milliseconds);
}

//int CVSemaphore::Release(long releaseCount, long* previousCount)
//{
//	 return ::ReleaseSemaphore(m_hSemaphore, releaseCount, previousCount);
//}
#endif


long CVSemaphore::Wait( long milliseconds)
{
#ifdef _WIN32
	return ::WaitForSingleObject( m_hSemaphore, milliseconds);
#else
	sem_wait( &m_sem );
	return( 1 );
#endif
}

int CVSemaphore::Release(long releaseCount, long* previousCount)
{
#ifdef _WIN32
	 return ::ReleaseSemaphore(m_hSemaphore, releaseCount, previousCount);
#else
	sem_post( &m_sem );
	return( 1 );
#endif
}