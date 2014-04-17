/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_GP_LOG_FILE__
#define		_GP_LOG_FILE__

#include	<string.h>


#include "Utime/GpTime.h"


//#include "Uthread/VSemaphore.h"



class CVLogger {
public:
	CVLogger();
	virtual ~CVLogger();


	void SetLive( int live )	{ m_live = live; }
	void SetFrame( int frame )	{ m_frame = frame; }
	void SetTime( int time )	{ m_time = time; }

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
	int m_frame;
	int	m_time;

	char	m_prefix[256];
	char	m_dir[256];

	char	m_file[256];

	FILE	*m_fp;



private:
	class CVSemaphore *m_semIn;

};


#define VLOG( prm )	 { if(m_logger != NULL ) m_logger->Write prm;  }


#endif