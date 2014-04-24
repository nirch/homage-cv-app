/*********************
 ***    DumpGp.c   ***
 *********************/
#include	<stdio.h> 
#include	<stdarg.h>
#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"

#include	"Log.h"

static FILE *fpLog = NULL;

void
gpLog_openR( char *dir, char *fname )
{
char	file[256];

	sprintf( file, "%s/%s", dir, fname  );

	gpLog_open( file );
}

void
gpLog_open( char *file )
{
	if( fpLog != NULL ){
		fclose( fpLog );
		fpLog = NULL;
	}

	fpLog = fopen( file, "w" );
}


void
gpLog_open_a( char *file )
{
	if( fpLog != NULL ){
		fclose( fpLog );
		fpLog = NULL;
	}

	fpLog = fopen( file, "ab" );
}


void
gpLog_close()
{
	if( fpLog == NULL )	return;

	fclose( fpLog );
	fpLog = NULL;
}


void
gpLog_flush()
{
	if( fpLog == NULL )	return;

	fflush( fpLog );
}


void gpLog( const char *format, ... )
{
	va_list args;

	if( fpLog == NULL )	return;



	va_start(args, format);

	vfprintf( fpLog, format, args);
}


void gpLogF( const char *format, ... )
{
	va_list args;

	if( fpLog == NULL )	return;



	va_start(args, format);

	vfprintf( fpLog, format, args);

	fflush( fpLog );
}



void
gpLog_time( char *title, gpTime_type *gt )
{
	if( fpLog == NULL )	return;
		
	gpTime_print( fpLog, title, gt );
}


void gpTrace( int mode, const char *format, ... )
{
va_list args;

	va_start(args, format);

	if( mode & 1 )
		vfprintf( stdout, format, args);


	if( mode & 0x2 && fpLog != NULL )
		vfprintf( fpLog, format, args);
}



int gpLogToFile( char *file, const char *format, ... )
{
	FILE *fp;

	va_list args;

	va_start(args, format);



	if( (fp = fopen ( file, "ab" )) == NULL )
		return( -1 );

	// fseek (fp, 0, SEEK_END );


	vfprintf( fp, format, args);

	fclose( fp );

	return( 1 );
}