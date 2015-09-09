/********************
 ***	GpMsg.c   ***
 ********************/

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32 
#include  <windows.h>
#ifndef WINCE //Grisha 10.05.2005
#include  <Wincon.h>
#endif
#endif


#include "igp.h"


static void urte_msg( char *title, char *msg )
{

#ifdef WIN32

	DWORD dwRet = 0;

#ifndef WINCE //Grisha 10.05.2005
	char buff[256];
	dwRet = GetConsoleTitle(buff, 256);
#endif

	if( dwRet != 0 )
		fprintf( stderr , "%s:  %s\n", title, msg );
	else {
#ifdef _UNICODE
		wchar_t buf[1024];
		wsprintf(buf, L"%S", msg);
		MessageBox( 0, buf, L"Urte Msg", MB_OK | MB_ICONEXCLAMATION);;
#else		
#ifndef WINCE
		MessageBox( 0, msg, title, MB_OK | MB_ICONEXCLAMATION);
#endif
#endif
	}
  
#else
#ifndef PALMOS
  fprintf( stderr , msg );
#endif
#endif
}


void error( char *sender ,char *msg )
{
//char error_msg[1024];


//	sprintf( error_msg, "Error from: %s - %s\n", sender, msg );
//	urte_msg( error_msg );
	urte_msg( sender, msg );

	exit(1);
}


void gpError( char *sender, char *fmt, ... )
{
	char msg[1024];

	va_list args;
	va_start( args, fmt);


	vsnprintf( msg, 1024, fmt, args );


	urte_msg( sender, msg );

	exit(1);
}


void warning( char *sender, char *msg )
{
//char warning_msg[256];

//	sprintf( warning_msg, "Warning from: %s - %s\n", sender, msg );
//	urte_msg( warning_msg );
	urte_msg( sender, msg );
}



void gpWarning( char *sender, char *fmt, ... )
{
	char msg[1024];

	va_list args;
	va_start( args, fmt);


	vsnprintf( msg, 1024, fmt, args );

	urte_msg( sender, msg );
}

int gpYesNo( char *title, char *fmt, ... )
{
#ifdef WIN32
	char msg[1024];

	va_list args;
	va_start( args, fmt);


	vsnprintf( msg, 1024, fmt, args );

//	urte_msg( sender, msg );

	if( MessageBox( 0, title, msg, MB_YESNO| MB_ICONEXCLAMATION) != IDYES )
		return( -1 );
#endif

	return( 1 );
}



// inData = "WND-MSG ID-ID"
void
gpSendWndMessage( char *inData, int state )
{
#ifdef _WIN32
int	hWnd,	msgId, id;
char	data[512];

	strcpy( data, inData );

	if( gp_strtok_int( data, "-", &hWnd ) < 0 )
		return;

	if( gp_strtok_int( NULL, "-", &msgId ) < 0 )
		return;

	if( gp_strtok_int( NULL, " \t\r\n", &id ) < 0 )
		return;


	PostMessage((HWND)hWnd, msgId, id, state );
#endif
}


/*	for debug */

void
puchar( u_char *a, int no )
{
int i;
	for( i = 0 ; i < no ; i++ )
	{
#ifndef PALMOS
		fprintf( stderr, "%4d   ", (int)a[i] );
#endif
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}


void
pchar( char *a, int no )
{
int i;
	for( i = 0 ; i < no ; i++ )
	{
#ifndef PALMOS
		fprintf( stderr, "%4d   ", (int)a[i] );
#endif
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}


void
pushort( u_short *a, int no )
{
int i;
	for( i = 0 ; i < no ; i++ )
	{
#ifndef PALMOS
		fprintf( stderr, "%4d   ", (int)a[i] );
#endif
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}

void
pshort( short *a, int no )
{
int i;
	for( i = 0 ; i < no ; i++ )
	{
#ifndef PALMOS
		fprintf( stderr, "%4d   ", (int)a[i] );
#endif
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}

void
pint( int *a, int no )
{
int i;
	for( i = 0 ; i < no ; i++ )
	{
#ifndef PALMOS
		fprintf( stderr, "%4d   ", (int)a[i] );
#endif
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}

void
pfloat( float *a, int no )
{
int i;
	for( i = 0 ; i < no ; i++ )
	{
#ifndef PALMOS
		fprintf( stderr, "%.2f   ", a[i] );
#endif
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}



void
pbit( int code, int no )
{
	while ( no-- != 0 ) {

		if ( code & 1L )
		{
#ifndef PALMOS
			fprintf( stderr, "1 " );
#endif
		}
		else
		{
#ifndef PALMOS
			fprintf( stderr, "0 " );
#endif
		}

		code >>= 1;
	}
#ifndef PALMOS
	fprintf( stderr, "\n");
#endif
}



void
wno( char * name, int no )
{
char	msg[256];
	sprintf( msg, "%d", no );
	warning( name, msg );
}

void
wnof( char * name, float	no )
{
char	msg[256];
	sprintf( msg, "%f", no );
	warning( name, msg );
}