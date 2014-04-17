/*********************
 ***	XmlStr.c   ***
 *********************/

#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Uigp/igp.h"



char * 
xmlStr_extarct_tag_data( char *s, char *tagName, char **data, int *bytes, int Falloc )
{
char	tname[256];
char	*start,	*end;

	sprintf( tname, "<%s>", tagName );
	start = strstr ( s, tname );
	if( start == NULL )	return( NULL );
	start += strlen( tname );

	sprintf( tname, "</%s>", tagName );
	end = strstr ( start, tname );

	if( end == NULL )	return( NULL );

	*bytes = end-start;
	*data = start;

	if( Falloc == 0 )
		return( end );


	*data = (char *)malloc( *bytes + 2 );
	memcpy( *data, start, *bytes );
	(*data)[*bytes] = 0;

	end += strlen( tname );

	return( end );
}



char * 
xmlStr_skip_tag( char *s, char *tagName )
{
char	tname[256];
char	*start,	*end;

	sprintf( tname, "<%s>", tagName );
	start = strstr ( s, tname );
	if( start == NULL )	return( NULL );
	start += strlen( tname );

	sprintf( tname, "</%s>", tagName );
	end = strstr ( start, tname );
	start += strlen( tname );

	return( end );
}


char * 
xmlStr_get_tag( char *s, char *tagName, char *data )
{
char	tname[256];
char	*start,	*end;
int	len;

	sprintf( tname, "<%s>", tagName );
	start = strstr ( s, tname );
	if( start == NULL )	return( NULL );
	start += strlen( tname );

	sprintf( tname, "</%s>", tagName );
	end = strstr ( start, tname );

	if( end == NULL )	return( NULL );

	len = end-start;
	
	memcpy( data, start, len );
	data[len] = 0;

	end += strlen( tname );

	return( end );
}




