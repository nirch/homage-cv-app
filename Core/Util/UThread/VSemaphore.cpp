#include "VSemaphore.h"

CVSemaphore::CVSemaphore(void)
{
}

CVSemaphore::~CVSemaphore(void)
{
}

//void CVSemaphore::Create(LPSECURITY_ATTRIBUTES semaphoreAtt, long initialCount, long maxCount, LPCTSTR name)
void CVSemaphore::Create(LPSECURITY_ATTRIBUTES semaphoreAtt, long initialCount, long maxCount, char *name)
{
	m_hSemaphore = ::CreateSemaphore(semaphoreAtt, initialCount, maxCount, name);
}

long CVSemaphore::WaitForSingleObject(long milliseconds)
{
	return ::WaitForSingleObject(m_hSemaphore, milliseconds);
}

int CVSemaphore::Release(long releaseCount, long* previousCount)
{
	 return ::ReleaseSemaphore(m_hSemaphore, releaseCount, previousCount);
}