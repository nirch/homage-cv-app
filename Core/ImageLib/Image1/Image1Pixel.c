 /*************************
 ***	Image1Pixel.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"



int
image1_nPixel_nonzero( image_type *sim )
{
u_char	*sp;
int	i,	j;
int	no;


	sp = sim->data;
	for( i = 0, no = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, sp++ )
			if( *sp != 0 )	no++;
	}

	return( no );
}


int
image1_nPixel_range( image_type *sim, int T0, int T1 )
{
	u_char	*sp;
	int	i,	j;
	int	no;


	sp = sim->data;
	for( i = 0, no = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, sp++ )
			if( *sp >= T0 && *sp <= T1 )
				no++;
	}

	return( no );
}




int 
image1B_nPixel_range( image_type *sim, box2i *b, int T0, int T1 )
{
int	x0,	y0,	x1,	y1;
u_char	*sp;
int	i,	j,	no;


	if( b != NULL ){
		if( (x0 = b->x0) < 0 )	x0 = 0;
		if( (x1 = b->x1) > sim->width )	x1 = sim->width;

		if( (y0 = b->y0) < 0 )	y0 = 0;
		if( (y1 = b->y1) > sim->height )	y1 = sim->height;
	}
	else	{
		x0 = y0 = 0;
		x1 = sim->width;
		y1 = sim->height;
	}



	no = 0;
	for( i = y0 ; i < y1 ; i++ ){

		sp = IMAGE_PIXEL( sim, i, x0 );

		for( j = x0 ; j < x1 ; j++, sp++ )
			if( *sp >= T0 && *sp <= T1 )
				no++;

	}

	return( no );
}


int 
image1B_nPixel_rangeN( image_type *sim, box2i *b, int T0, int T1, int nT )
{
	int	x0,	y0,	x1,	y1;
	u_char	*sp;
	int	i,	j,	no;


	if( b != NULL ){
		if( (x0 = b->x0) < 0 )	x0 = 0;
		if( (x1 = b->x1) > sim->width )	x1 = sim->width;

		if( (y0 = b->y0) < 0 )	y0 = 0;
		if( (y1 = b->y1) > sim->height )	y1 = sim->height;
	}
	else	{
		x0 = y0 = 0;
		x1 = sim->width;
		y1 = sim->height;
	}



	no = 0;
	for( i = y0 ; i < y1 ; i++ ){

		sp = IMAGE_PIXEL( sim, i, x0 );

		for( j = x0 ; j < x1 ; j++, sp++ ){
			if( *sp >= T0 && *sp <= T1 )
				no++;
		}

		if( no > nT )
			return(i);

	}

	return( y1 );
}