/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef		_GP_LOG_
#define		_GP_LOG_

#ifdef __cplusplus
extern "C" {
#endif


void	gpLog_openR( char *dir, char *file );
void	gpLog_open( char *file );

void	gpLog_open_a( char *file );

void	gpLog_close();

void	gpLog_flush();

void	gpLog( const char *format, ... );

void	gpLogF( const char *format, ... );

void	gpTrace( int mode, const char *format, ... );

int		gpLogToFile( char *file, const char *format, ... );

extern  void gpLogFile( const char *format, ... );


void	gpLog_time( char *title, struct gpTime_type *gt );

void	gpLog_timeM( char *title, struct gpTime_type *gt, int modulo );



#ifdef _GPLOG
#define	GPLOG_OPEN( file )	gpLog_open( file )
#define	GPLOG_OPENR( dir, file )	gpLog_openR( dir, file )
#define	GPLOG_OPEN_A( file )	gpLog_open_a( file )

#define	GPLOG_CLOSE()		gpLog_close();
#define	GPLOG_FLUSH()		gpLog_flush();
#define	GPLOG( prm )		gpLog prm
#define	GPLOGF( prm )		gpLogF prm
#define	GPLOGFILE( prm )	gpLogFile prm
#define	GPTRACE( prm )		gpTrace prm
#define	GPLOG_TIME( prm )	gpLog_time prm
#define	GPLOG_TIMEM( prm )	gpLog_timeM prm
#else
#define	GPLOG_OPEN( file )
#define	GPLOG_OPENR( dir, file )
#define	GPLOG_OPEN_A( file )
#define	GPLOG_CLOSE()	
#define	GPLOG_FLUSH()
#define	GPLOG( prm )
#define	GPLOGF( prm )
#define	GPLOGFILE( prm )

#define	GPTRACE( prm )
#define	GPLOG_TIME( prm )
#define	GPLOG_TIMEM( prm )
#endif

#ifdef __cplusplus
}
#endif
#endif