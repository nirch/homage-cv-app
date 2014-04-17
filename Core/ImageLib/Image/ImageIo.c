/**********************
 ***   Image6Io.c   ***
 **********************/
#include	<string.h>
#include	<stdio.h>

#include	"ImageType/ImageType.h"

#include	"ImageType/ImageTool.h"




int
image_write_im( image_type *im, char *file )
{
FILE	*fp;
int	channel,	type;

	gp_filename_force_extension( file, ".im" );
	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	fwrite( &im->column, 1, 4, fp );

	fwrite( &im->row, 1, 4, fp );

//	fwrite( &im->depth, 1, 4, fp );

	channel = im->channel;
	type = IMAGE_TYPE(im);
	fputc( channel, fp );
	fputc( type, fp );
	fputc( 0, fp );
	fputc( 0, fp );




	fwrite( im->data, im->depth, im->row*im->column, fp );

	fclose( fp );

	return( 1 );
}


image_type *
image_read_im( char *file )
{
FILE	*fp;
image_type *im;
int	width,	height;
int	channel,	type;

	if( (fp = fopen( file, "rb" )) == NULL )
		return( NULL );


	fread( &width, 1, 4, fp );

	fread( &height, 1, 4, fp );

//	fread( &depth, 1, 4, fp );


	channel	= fgetc( fp );
	type	= fgetc( fp );

	fgetc( fp );
	fgetc( fp );

	im = image_alloc( width, height, channel, type, 1 );
	
	fread( im->data, im->depth, im->row*im->column, fp );

	fclose( fp );

	return( im );
}




image_type *
image_read_raw( char *file, int width, int height, int foramt )
{
FILE	*fp;
image_type *im;

	if( (fp = fopen( file, "rb" )) == NULL )
		return( NULL );

	im = image_create( height, width, foramt, 1, NULL );

	fread( im->data, im->depth, im->row*im->column, fp );

	fclose( fp );

	return( im );
}
