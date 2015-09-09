/*********************
 ***    DumpGp.c   ***
 *********************/
#include	<stdio.h> 
#include	<stdarg.h>
#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"



#include	"VLogger.h"

#include "Uthread/VSemaphore.h"


CVLogger::CVLogger()
{
	m_live = 1;

	m_time = 1;

	m_fp = NULL;

	m_period = 60*60*24;

	m_semIn = new CVSemaphore();
	m_semIn->Create( NULL, 0, 1, NULL);

	m_semIn->Release( 1 );
}

CVLogger:: ~CVLogger()
{
	Close();
}


int CVLogger::Init( char *prefix, char *dir )
{
	strcpy( m_dir, dir );
	strcpy( m_prefix, prefix );

	vTimer_init( &m_periodTimer, m_period*1000 );

	NewFile();

	Open();

	return( 1 );
}

void CVLogger::SetFile( char *file )
{
	strcpy( m_file, file );

}


void CVLogger::NewFile()
{
	//char	bfile[256];

	//sprintf( bfile, "%s/%s-%%.2d.log", m_dir, m_prefix );
	//int iFile = gpFile_unuse_id( bfile, 0, m_file );

	char	buf[256];
	gpTime_string( buf, 0, 3 );


	sprintf( m_file, "%s/%s-%s.log", m_dir, m_prefix, buf );


}


int CVLogger::Open()
{
	if( m_fp != NULL )
		return( 1 );

	if( ( m_fp = fopen( m_file, "ab")) == NULL )
		return( -1 );
		
	return( 1 );
}


int CVLogger::Close()
{
	if( m_fp == NULL )
		return( 1 );

	fclose( m_fp );
	m_fp = NULL;

	return( 1 );
}



void CVLogger::Write( char *msg )
{
	if( m_live == 0 )	return;

	m_semIn->WaitForSingleObject();


	if( vTimer_test( &m_periodTimer, 0 ) > 0 ){
		Close();
		NewFile();
		Open();
	}


	if( Open() < 0 ){
		m_semIn->Release( 1 );
		return;
	}


	if( m_time == 1 ){
		char	buf[256];
		fprintf( m_fp, "%s   ", gp_time_iso681( buf, 0 )  );
	}

	fprintf( m_fp, msg );

	m_semIn->Release( 1 );
}


void CVLogger::Flush()
{
	m_semIn->WaitForSingleObject();

	if( m_fp != NULL )
		fflush( m_fp );

	m_semIn->Release( 1 );
}




void CVLogger::Write( const char *fmt, ... )
{
	if( m_live  == 0 )	return;

	va_list args;
	va_start( args, fmt);

	char	msg[1024];
	vsnprintf( msg, 1024, fmt, args );

	Write( msg );
}






void CVLogger::Write( char *title, gpTime_type *gt )
{
char	msg[1024];
	if( m_live  == 0 )	return;


	if( gpTime_sprint( msg, title, gt ) > 0 )
		Write( msg );
}


void CVLogger::Write( char *title, gpTime_type *gt, int modulo )
{
	if( modulo <= 0 )	modulo = m_frame;

	if( modulo <= 0 || gt->no == 0 || (gt->no%modulo) != 0 )
		return;


	Write( title, gt );

	Flush();


	gpTime_init( gt );
}


void CVLogger::Write( char *title, char *title1, gpTime_type *gt, int modulo )
{
char	msg[256];

	if( modulo <= 0 )	modulo = m_frame;

	if( gt->no == 0 || (gt->no%modulo) != 0 )
		return;

	sprintf( msg, "%s-%s", title, title1 );

	Write( msg, gt );

	Flush();


	gpTime_init( gt );
}
