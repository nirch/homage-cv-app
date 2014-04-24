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

/*
mediaData_type *
mediaData_from_buffer( char *mime, char *id, char *location, char *data, int bytes, int Falloc )
{
mediaData_type *md;


	md = mediaData_alloc( 0 );

	if( mime != NULL )
		strcpy( md->mime, mime );

	if( location  != NULL )
		md->location = strdup( location );

	if( id  != NULL )
		md->id = strdup( id );

	if( mime != NULL )
		md->type = mediaData_mime2format( mime );
	else {
		md->type = mediaData_format_from_extension( location );
		mediaData_mime( md, md->mime );
	}

	md->bytes = bytes;

	if( Falloc == 1 ){
		md->data = ( char *)malloc(  md->bytes + 1 );
		memcpy( md->data, data, md->bytes );
	}
	else	md->data = data;



	return( md );
}



mediaData_type *
mediaData_get( mediaData_type *md[], int nMedia, char *id, char *location )
{
int	i;

	for( i = 0 ; i < nMedia ; i++ ){

		if( location != NULL ){
			if( md[i]->location != NULL && strcmp( md[i]->location, location ) == 0 )
				return( md[i] );

			if( md[i]->id != NULL && strcmp( md[i]->id, location ) == 0 )
				return( md[i] );
		}

		if( id != NULL ){
			if( md[i]->id != NULL && strcmp( md[i]->id, id ) == 0 )
				return( md[i] );

			if( md[i]->location != NULL && strcmp( md[i]->location, id ) == 0 )
				return( md[i] );
		}


	}

	return( NULL );
}



void
mediaData_set_location( mediaData_type *md, char *location )
{
	if( md->location != NULL )
		free( md->location );

	md->location = strdup( location );
}


void
mediaData_set_id( mediaData_type *md, char *id )
{
	if( md->id != NULL )
		free( md->id );

	md->id = strdup( id );
}
*/