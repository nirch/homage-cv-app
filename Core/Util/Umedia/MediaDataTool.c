/**************************
 ***	mediaDataTool.c	***
 **************************/
#include	<string.h>
#include	"Uigp/igp.h"

#include	"MediaDataType.h"



mediaData_type *
mediaData_alloc( int bytes )
{
	mediaData_type *md = (mediaData_type *)malloc(sizeof(mediaData_type));
	memset(md, 0, sizeof(mediaData_type));

	md->mime[0] = 0;
	md->location = NULL;
	md->id = NULL;

	md->bytes = 0;
	md->data = NULL;

	md->sec = -1;

	md->type = -1;

	if( bytes > 0 ){
		md->bytes = bytes;
		md->data = (char *)malloc( sizeof(char)*md->bytes );
	}



	return md;
}



void
mediaData_destroy(mediaData_type * md)
{
	if( md->data != NULL ) 
	{
		free( md->data );
		md->data = NULL;
	}

	if( md->location != NULL )
		free( md->location );

	if( md->id != NULL )
		free( md->id );

	free (md);
}


mediaData_type *
mediaData_make_copy( mediaData_type *smd )
{
mediaData_type *md;

	md = mediaData_alloc( smd->bytes );

	md->type = smd->type;

	md->bytes = smd->bytes;

	memcpy( md->data, smd->data, smd->bytes );

	md->sec = smd->sec;

	if( smd->id != NULL )
		md->id = strdup( smd->id);


	if( smd->location != NULL )
		md->location = strdup( smd->location );


	strcpy( md->mime, smd->mime );


	return( md );
}


mediaData_type *
mediaData_create( int mediaType, char *data, int bytes )
{
mediaData_type *md;

	md = mediaData_alloc( 0 );

	md->type = mediaType;

	md->bytes = bytes;

	md->data = data;


	return( md );
}



mediaData_type *
mediaData_createM( char *mime, char *data, int bytes )
{
	mediaData_type *md;

	md = mediaData_alloc( 0 );


	strncpy( md->mime, mime, 64 );

	md->type = mediaData_mime2format( md->mime );

	md->bytes = bytes;

	md->data = data;


	return( md );
}


void
mediaDataA_destroy(mediaData_type * md[], int nMd )
{
int	i;

	for( i = 0 ; i < nMd ; i++ )
		mediaData_destroy( md[i] );
}

