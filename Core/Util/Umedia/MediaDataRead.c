/****************************
 ***	MediaDataRead.c   ***
 ****************************/
#include <stdio.h>
#include <string.h>

#include "Uigp/igp.h"

#include "MediaDataType.h"




mediaData_type *
mediaData_read_file( char *file )
{
char	extension[256];
mediaData_type *md;
FILE	*fp;
int	bytes,	mdType;

	gp_filename_extract_extension( file, extension, 256 );
	mdType = mediaData_mime2format( extension );
	if( mdType < 0 )	return( NULL );


	bytes = gpFile_size( file );


	if( (fp = fopen(file,"rb")) == NULL )
		return( NULL );


	md = mediaData_alloc( bytes );	

	fread( md->data, sizeof( u_char ), md->bytes, fp );

	fclose( fp );



	md->type = mdType;

	mediaData_mime( md, md->mime );


	return( md );	
}




void
mediaData_write_to_file( mediaData_type *md, char *fileName )
{
char	file[256],	extension[256];
FILE *fp;

	strcpy( file, fileName );

	if( md->type > 0 ){
		mediaData_format2extension( md->type, extension );
		gp_filename_force_extension( file, extension );
	}

	if( (fp = fopen(file,"wb")) == NULL )
		return;
	

	fwrite( md->data, sizeof( u_char ), md->bytes, fp );

	fclose( fp );
}
