
#include	<stdio.h>
#include	<math.h>
#include	<string.h>





#include	"Uigp/igp.h"

#include "ImageType/ImageType.h"
#include "ImageType/PaletteType.h"

#include "Png/ImageWritePng.h"


static void	read_command_line(int argc, char *argv[], char *vimFile, char *outFile );


int	image_read_png_cunck( FILE *fp, void **data, int *bytes );


int
main( int argc, char **argv )
{
char	pngFile[256],	outFile[256];
FILE	*fp;
int	ret;
char	*data;
int	bytes;
char	buf[10];
	read_command_line( argc, argv, pngFile, outFile );




//	image_read_png_cuncks( pngFile );

	if( (fp = fopen( pngFile, "rb" )) == NULL )
		return( -1 );

	fread( buf, 1, 8, fp );
	ret = 1;
	while( ret == 1 )
		ret = image_read_png_cunck( fp, &data, &bytes );



	return( 1 );
}


int		png_get_uint_32( u_char buf[] );

int
image_read_png_cunck( FILE *fp, void **data, int *bytes ) 
{
char	chunk_length[4];
int	length;

	fread( chunk_length, 1, 4, fp );
	length = png_get_uint_32(chunk_length);

	*data = malloc( length +25 );
	fread( *data, 1, length+25, fp );
//	png_crc_read(png_ptr, *data, length+4 );

	*bytes = length;

	return( 1 );
}

//#ifndef PNG_READ_BIG_ENDIAN_SUPPORTED
/* Grab an unsigned 32-bit integer from a buffer in big-endian format. */
int /* PRIVATE */
png_get_uint_32( u_char *buf)
{
   int i = ((int)(*buf) << 24) +
      ((int)(*(buf + 1)) << 16) +
      ((int)(*(buf + 2)) << 8) +
      (int)(*(buf + 3));

   return (i);
}

image_type *
image_cycle( int row, int col, int t1, int t2 )
{
image_type	*im;
int	i,	j,	x,	y,	t0;
u_char	*tp;

	t1 *= t1;
	t2 *= t2;

	im = image_create( row, col, 1, 1, NULL );

	tp = im->data;
	for( i = 0 ; i < row ; i++ ){
		x = i - 0.5 *row;
		for( j = 0 ; j < col ; j++ ){
			y = j - 0.5 *col;
			t0 = x*x + y* y;
			if( t0 < t1 )
				*tp++ = 0;
			else
				if( t0 < t2 )
					*tp++ = 1;
				else	*tp++ = 255;
		}
	}

	return( im );
}













static char *Usage = 
	"Usage: vimKframe   vimFile  outFile";



static void	read_command_line(int argc, char *argv[], char *vimFile, char *outFile )
{
int     k;

	k = 1;
	


	if (k < argc) {
		sprintf( vimFile, "%s", argv[k]);
		k++;
	}
	else	error("read_command_line", Usage);


	if (k < argc) {
		sprintf( outFile, "%s", argv[k]);
		k++;
	}
	else	error("read_command_line", Usage);
}
