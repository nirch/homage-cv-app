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

	int Init( char *dir, char *prefix, int spy = 0, int spyDt = 0 );
	int Init( char *dir, int id, int spy = 0, int spyDt = 0 );

	int Add( void *data[], int nData );

	int Add( void *data );

	virtual int Get( void *data[], int *nData );

	static void ReleaseData( void *data[], int nData );



	int SetIn();

	int WaitForOut();

	void SetSpy( int spy )	{ m_spy = spy; }
	int Spy( void *data[], int nData );

	static int Dump( void *data[], int nData, char *prefix, int index, char *suffix  );

	static int Write( void *data[], int nData, char *file  );

	static int Write( void *data[], int nData, FILE *fp );

	static int Read( void *data[], int *nData, char *file );

	static int Read( void *data[], int *nData, FILE *fp );

	static int SkipEmpty( FILE *fp );

	static int NextFile( char *file );

private:
	qData_type* CopyData( qData_type *d );


	int	m_NQ;
	int	m_nQ;
	qData_type	**m_aq;

	int	m_spy;
	char	m_dir[256];
	char	m_prefix[256];
	int		m_index;
	vTime_type m_time;
	int	m_spyDtime;




public:
	char	m_file[256];
	FILE *m_fp;

private:


	CVSemaphore m_semIn;
};

#endif __VPIPE_TASK__H_