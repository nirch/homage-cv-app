/*********************
 ***    DumpGp.c   ***
 *********************/
#include	<stdio.h> 
#include	<stdarg.h>
#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"

#ifndef WIN32
#include	<sys/times.h>
#else
#include	<time.h>
#endif

#include	"LogFile.h"

#include "Uthread/VSemaphore.h"


CLogFile::CLogFile()
{
	m_live = 1;

	m_fp = NULL;

	m_semIn = new CVSemaphore();
	m_semIn->Create( NULL, 0, 1, NULL);

	m_semIn->Release( 1 );
}

CLogFile:: ~CLogFile()
{

}


int CLogFile::Init( char *prefix, char *dir )
{
	strcpy( m_dir, dir );
	strcpy( m_prefix, prefix );

	NewFile();

	Open();

	return( 1 );
}

void CLogFile::SetFile( char *file )
{
	strcpy( m_file, file );

}


void CLogFile::NewFile()
{
	//char	bfile[256];

	//sprintf( bfile, "%s/%s-%%.2d.log", m_dir, m_prefix );
	//int iFile = gpFile_unuse_id( bfile, 0, m_file );

	char	buf[256];
	time_t ltime;
	time(&ltime);
	gpTime_string( buf, ltime, 2 );


	sprintf( m_file, "%s/%s-%s.log", m_dir, m_prefix, buf );


}


int CLogFile::Open()
{
	if( m_fp != NULL )
		return( 1 );

	if( ( m_fp = fopen( m_file, "ab")) == NULL )
		return( -1 );
		
	return( 1 );
}


int CLogFile::Close()
{
	if( m_fp == NULL )
		return( 1 );

	fclose( m_fp );
	m_fp = NULL;

	return( 1 );
}


void CLogFile::Write( const char *fmt, ... )
{
	if( m_live  == 0 )	return;

	va_list args;
	va_start( args, fmt);

	char	msg[1024];
	vsnprintf( msg, 1024, fmt, args );

	Write( msg );
}


void CLogFile::Write( char *msg )
{
	m_semIn->WaitForSingleObject();

	if( Open() < 0 ){
		m_semIn->Release( 1 );
		return;
	}

	fprintf( m_fp, msg );

	m_semIn->Release( 1 );
}


void CLogFile::Flush()
{
	m_semIn->WaitForSingleObject();

	if( m_fp != NULL )
		fflush( m_fp );

	m_semIn->Release( 1 );
}


void CLogFile::Write( char *title, gpTime_type *gt )
{
char	msg[1024];
	if( m_live  == 0 )	return;


	if( gpTime_sprint( msg, title, gt ) > 0 )
		Write( msg );
}


void CLogFile::Write( char *title, gpTime_type *gt, int modulo )
{
	
	if( gt->no == 0 || (gt->no%modulo) != 0 )
		return;


	Write( title, gt );

	Flush();


	gpTime_init( gt );
}