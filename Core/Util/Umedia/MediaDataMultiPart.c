/**************************
 ***	mediaDataToolCo.c	***
 **************************/
#include	<string.h>
#include	"Uigp/igp.h"

#include	"MediaDataType.h"


static int	mediaData_multipart_pack_1( mediaData_type *md, u_char *data );

static  int		mediaData_multipart_parse_1( u_char **data, char *end, mediaData_type **md );



int 
mediaData_multipart_pack( mediaData_type *amd[], int nMd, u_char **data, int *nData )
{
int	i,	size,	n;
	
	size = 0;
	for( i = 0 ; i < nMd ; i++ )
		size += amd[i]->bytes + 1000;



	*data = (u_char *)malloc( size *sizeof(u_char) );

	n = 0;
	for( i = 0 ; i < nMd ; i++ )
		n += mediaData_multipart_pack_1( amd[i], &(*data)[n] );


	n += sprintf( &(*data)[n], "--boundary-content\r\n" );


	*nData = n;


	return( n );
}




static int 
mediaData_multipart_pack_1( mediaData_type *md, u_char *data )
{
int	n;


	n = 0;

	n += sprintf( &data[n], "--boundary-content\r\n", md->mime );


	n += sprintf( &data[n], "Content-Type: %s\r\n", md->mime );

	if( md->location != NULL )
		n += sprintf( &data[n], "Content-Location: %s\r\n", md->location );


	n += sprintf( &data[n], "Content-Length: %d\r\n", md->bytes );

	n += sprintf( &data[n], "\r\n" );


	memcpy( &data[n], md->data, md->bytes );

	n += md->bytes;

	n += sprintf( &data[n], "\r\n" );


	return( n );
}




int 
mediaData_multipart_parse( u_char *data, int nData, mediaData_type *amd[], int *nMd )
{
	mediaData_type *md;
	u_char	*p;


	p = data;
	while( mediaData_multipart_parse_1( &p, data + nData, &md ) > 0 ){
		amd[(*nMd)++] = md;
	}

	return( *nMd );
}


static  int 
mediaData_multipart_parse_1( u_char **data, char *end, mediaData_type **md )
{
char	mime[256];
char	location[256];
int		length;
char	*p,	*p0;


	p0 = *data;

	p = gpStr_token_skip( p0, end, "--boundary-content\r\n", -1 );
	if( p ==  NULL )	return( -1 );
	p0 = p;



	length = -1;
	mime[0] = 0;
	location[0] = 0;

	while( 1 ){
		p = gpStr_token( p0, end, "\r\n", 2 );
		if( p == p0 )	break;


		if( strncmp( p0, "Content-Type:", 13 ) == 0 ){
			sprintf( &p0[13], "%s", mime );
			p0 = p+2;
			continue;
		}

		if( strncmp( p0, "Content-Location:", 17 ) == 0 ){
			sprintf( &p0[17], "%s", location );
			p0 = p+2;
			continue;
		}

		if( strncmp( p0, "Content-Length:", 15 ) == 0 ){
			sprintf( &p0[15], "%d", &length );
			p0 = p+2;
			continue;
		}
	}



	*md = mediaData_alloc( length );

	memcpy( (*md)->data, p0, (*md)->bytes );

	*data = p0 + (*md)->bytes;

	return( 1 );
}

