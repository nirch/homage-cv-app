/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_GP_LOG_FILE__
#define		_GP_LOG_FILE__

#include	<string.h>


#include "Utime/GpTime.h"


//#include "Uthread/VSemaphore.h"



class CLogFile {
public:
	CLogFile();
	virtual ~CLogFile();


	void SetLive( int live )	{ m_live = live; }

	int Init( char *prefix, char *dir );

	void SetFile( char *file );

	void NewFile();


	void Write( const char *fmt, ... );

	void Write( char *title, gpTime_type *gt );

	void Write( char *title, gpTime_type *gt, int modulo );


	void Write( char *msg );

	void Flush();

private:
	int Open();

	int Close();


private:
	int	m_live;

	char	m_prefix[256];
	char	m_dir[256];

	char	m_file[256];

	FILE	*m_fp;



private:
	class CVSemaphore *m_semIn;

};

#endif