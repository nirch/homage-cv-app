#ifndef VIM_SEMAPHORE_H
#define VIM_SEMAPHORE_H


#include "semaphore.h"


class CVSemaphore
{
public:



	CVSemaphore(void);

	~CVSemaphore(void);

	void Create( LPSECURITY_ATTRIBUTES semaphroeAtt, long initialCount, long maxCount, char * name);

	long WaitForSingleObject(long milliseconds = INFINITE );

	int Release( long releaseCount, long* previousCount = NULL );

public;
	sem_t m_sem;
};

#endif