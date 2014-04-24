/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_GP_LOG_FILE__
#define		_GP_LOG_FILE__

#include	<string.h>


class CLogFile {
public:
	CLogFile();
	virtual ~CLogFile();


	void SetLive( int live )	{ m_live = live; }

	void Init( char *prefix, char *dir );

	void SetFile( char *file );

	void NewFile();


	void Write( const char *fmt, ... );


private:
	int	m_live;

	char	m_prefix[256];
	char	m_dir[256];

	char	m_file[256];
};

#endif