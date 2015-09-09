#include "VSemaphore.h"

CVSemaphore::CVSemaphore(void)
{
}

CVSemaphore::~CVSemaphore(void)
{
}


void CVSemaphore::Create( long initialCount, long maxCount )
{
//	m_hSemaphore = ::CreateSemaphore(semaphoreAtt, initialCount, maxCount, name);

	sem_init( &m_sem, 0, initialCount);

}


long CVSemaphore::WaitForSingleObject(long milliseconds)
{
//	return ::WaitForSingleObject(m_hSemaphore, milliseconds);

	sem_wait( &m_sem );

	return( 1 );
}


int CVSemaphore::Release()
{
//	 return ::ReleaseSemaphore(m_hSemaphore, releaseCount, previousCount);

	  sem_post( &m_sem );

	  return( 1 );
}
