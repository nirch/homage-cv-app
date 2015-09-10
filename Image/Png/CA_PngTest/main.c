
#include	<stdio.h>
#include	<math.h>
#include	<string.h>





#include	"Uigp/igp.h"

#include "ImageType/ImageType.h"
#include "ImageType/PaletteType.h"

#include "PngCodec/Png/ImageWritePng.h"


static void	read_command_line(int argc, char *argv[], char *vimFile, char *outFile );


image_type *	image_cycle( int row, int col, int t1, int t2 );


int
main( int argc, char **argv )
{
char	vimFile[256],	outFile[256];
char	dir[256],	name[256],	extension[256];
image_type	*im;
palette_type *palette;


	read_command_line( argc, argv, vimFile, outFile );






	palette = palette_create_WINDOW_SYSTEM();

	palette->data[0].Red = 255;
	palette->data[0].Green = 0;
	palette->data[0].Blue = 0;

	palette->data[1].Red = 0;
	palette->data[1].Green = 255;
	palette->data[1].Blue = 0;


	palette->data[255].Red = 0;
	palette->data[255].Green = 0;
	palette->data[255].Blue = 255;

	im = image_cycle( 100, 100, 10, 20 );

//	gp_filename_split( outFile, dir, name, extension );


	image_write_png_T( im,
						palette, 255,
						outFile);

	return( 1 );
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
