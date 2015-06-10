#include	<stdio.h>
#include	<string.h>
#include	"ImageType/ImageType.h"



static int	image4_copy_in_image3( image_type *sim, int x0, int y0, image_type *im );

static int	image4_copy_in_VFLIP_image3( image_type *sim, int x0, int y0, image_type *im );



int
image4_copy_in( image_type *sim, int x0, int y0, image_type *im )
{


	//if( sim->depth == 1 ){
	//}


	if( sim->depth == 3 ){
		image4_copy_in_image3( sim, 0, 0, im );
		return( 1 );
	}


	if( sim->depth == 4 ){
		image_copy_in( sim, 0, 0, im );
		return( 1 );
	}


	return( -1 );
}


static int
image4_copy_in_image3( image_type *sim, int x0, int y0, image_type *im )
{
	u_char	*sp;
	u_int	*tp;
	int	i,	j,	width,	height;


	width = sim->width;
	if( x0 + sim->width > im->width )
		width = im->width - x0;
	

	height = sim->height;
	if( y0+ sim->height > im->height )
		height = im->height - y0;





	for( i = 0 ; i < height ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE4_PIXEL( im, i+y0, x0 );

		for( j = 0 ; j < width ; j++, sp += 3 )
			*tp++ = IMAGE4_RGB( *sp, *(sp+1), *(sp+2) );
	}

	return( 1 );
}





int
image4_copy_in_VFLIP( image_type *sim, int x0, int y0, image_type *im )
{


	//if( sim->depth == 1 ){
	//}


	if( sim->depth == 3 ){
		image4_copy_in_VFLIP_image3( sim, 0, 0, im );
		return( 1 );
	}


	if( sim->depth == 4 ){
		image_copy_in_VFLIP( sim, 0, 0, im );
		return( 1 );
	}


	return( -1 );
}


static int
image4_copy_in_VFLIP_image3( image_type *sim, int x0, int y0, image_type *im )
{
	u_char	*sp;
	u_int	*tp;
	int	i,	j,	width,	height;


	width = sim->width;
	if( x0 + sim->width > im->width )
		width = im->width - x0;


	height = sim->height;
	if( y0+ sim->height > im->height )
		height = im->height - y0;





	for( i = 0 ; i < height ; i++ ){
		sp = IMAGE_PIXEL( sim, height-1-i, 0 );
		tp = IMAGE4_PIXEL( im, i+y0, x0 );

		for( j = 0 ; j < width ; j++, sp += 3 )
			*tp++ = IMAGE4_RGB( *sp, *(sp+1), *(sp+2) );
	}

	return( 1 );
}
