/**************************
 ***   ImagePrint.cpp   ***
 **************************/
#include	<stdio.h>
#include	<math.h>
#include	"Uigp/igp.h"


#include	"ImageType/ImageType.h"

#include	"ImageMark.h"




void
image1_print( image_type *sim, int i0, int j0, int n )
{
int	i,	j;
u_char	*sp;

	for( i = i0-n ; i <= i0+n ; i++ ){
		for( j = j0-n ; j <= j0+n ; j++ ){
			sp = IMAGE_PIXEL( sim, i, j );
			fprintf( stderr, "%d   ", (int)*sp );
		}
		fprintf( stderr, "\n" );
	}
	fprintf( stderr, "\n" );
}


void
imageF_print( image_type *sim, int i0, int j0, int n )
{
int	i,	j;
float	*sp;

	for( i = i0-n ; i <= i0+n ; i++ ){
		for( j = j0-n ; j <= j0+n ; j++ ){
			sp = (float *)IMAGE_PIXEL( sim, i, j );
			fprintf( stderr, "%.2f   ", *sp );
		}
		fprintf( stderr, "\n" );
	}
	fprintf( stderr, "\n" );
}


void
image2_print( image_type *sim, int i0, int j0, int n )
{
int	i,	j;
short	*sp;

	for( i = i0-n ; i <= i0+n ; i++ ){
		for( j = j0-n ; j <= j0+n ; j++ ){
			sp = (short *)IMAGE_PIXEL( sim, i, j );
			fprintf( stderr, "%.2f   ", (float)*sp/16.0 );
		}
		fprintf( stderr, "\n" );
	}
	fprintf( stderr, "\n" );
}
