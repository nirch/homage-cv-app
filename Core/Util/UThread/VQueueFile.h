/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __VQUEUE_FILE_H_
#define __VQUEUE_FILE_H_


#include "UThread/VThread.h"
#include "Uthread/VSemaphore.h"

#include "Utime/GpTime.h"

#include "qData.h"

#include	"VQueue.h"




class CVQueueFile : public CVQueue
{

public:
	CVQueueFile( int no );

	~CVQueueFile(void);

	int Init( char *file, int skip );

	int Get( void *data[], int *nData );


private:

//	FILE *m_fp;

private:


};

#endif __VPIPE_TASK__H_