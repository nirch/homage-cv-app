/**************************
 ***	mediaDataToolCo.c	***
 **************************/
#include	<string.h>
#include	"Uigp/igp.h"

#include	"MediaDataType.h"




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

	if( md->type < 0 )
		md->type = mediaData_format_from_extension( md->location );
}


void
mediaData_set_id( mediaData_type *md, char *id )
{
	if( md->id != NULL )
		free( md->id );

	md->id = strdup( id );
}