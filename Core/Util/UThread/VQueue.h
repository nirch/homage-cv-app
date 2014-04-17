/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __VQUEUE_H_
#define __VQUEUE_H_


#include "UThread/VThread.h"
#include "Uthread/VSemaphore.h"

#include "Utime/GpTime.h"

#include "qData.h"

#ifdef _AA_
typedef struct qData_type {

	int	from;		// from

	int dataSize;
	
	vTime_type	timeStamp;




}	qData_type;
#endif


class CVQueue
{

public:
	CVQueue( int no );

	~CVQueue(void);


	int Add( void *data[], int nData );

	int Add( void *data );

	int Get( void *data[], int *nData );




	int SetIn();

	int WaitForOut();


private:
	qData_type* CopyData( qData_type *d );


	int	m_NQ;
	int	m_nQ;
	qData_type	**m_aq;


private:


	CVSemaphore m_semIn;
};

#endif __VPIPE_TASK__H_