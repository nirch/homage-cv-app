/*********************
 ***    DumpGp.c   ***
 *********************/
#include	<stdio.h> 
#include	<stdarg.h>
#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"

#include	"LogFile.h"

CLogFile::CLogFile()
{
	m_live = 1;
}

CLogFile:: ~CLogFile()
{

}


void CLogFile::Init( char *prefix, char *dir )
{
	strcpy( m_dir, dir );
	strcpy( m_prefix, prefix );

	NewFile();
}

void CLogFile::SetFile( char *file )
{
	strcpy( m_file, file );

}


void CLogFile::NewFile()
{
	char	bfile[256];

	sprintf( bfile, "%s/%s-%%.2d.log", m_dir, m_prefix );
	int iFile = gpFile_unuse_id( bfile, 0, m_file );
}



void CLogFile::Write( const char *fmt, ... )
{
FILE	*fp;

	if( m_live  == 0 )	return;

	if( ( fp = fopen( m_file, "ab")) == NULL )
		return;

	va_list args;
	va_start( args, fmt);

	char	msg[1024];
	vsnprintf( msg, 1024, fmt, args );

	fprintf( fp, msg );

	fclose( fp );


}