/**************************
 ***	ImageFile.c	***
 **************************/
#include	<stdio.h>
#include	<string.h>

#include	"ImageType/ImageType.h"

void
image_file_attach_name( image_type *im, char *dir, char *name, char *extension )
{
char	fname[256];

	sprintf( fname, "%s/%s%s", dir, name, extension );

	image_file_attach_nameF( im, fname );
}


void
image_file_attach_nameF( image_type *im, char *fname )
{
	im->file = realloc( im->file, strlen( fname ) + 1 );

	strcpy( im->file, fname );
}



int
image_file_size( image_type *im )
{
int	size;

	if( im->file == NULL )	return( -1 );

	size = gpFile_size( im->file );

	return( size );
}
