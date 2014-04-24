 /*********************************
 ***	Image1EqualNeighbor.c	***
 *********************************/

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


static image_type *	image1_equal_neighbor( image_type *sim, image_type *im );


int
image1_equal_neighbor_filter( image_type *sim, int nT )
{
image_type *im;
u_char	*sp,	*sp0,	*tp,	*tp0;
int	i,	j,	n,	k,	val;
int	align,	no;


	im = image1_equal_neighbor( sim, NULL );

	no = 0;
	align = sim->width - 3;
	for( i = 1 ; i < sim->row-1 ; i ++ ){
		sp = IMAGE_PIXEL( sim, i, 1 );

		tp = IMAGE_PIXEL( im, i, 1 );


		for( j = 1 ; j < sim->column-1 ; j ++, sp++, tp++ ){

			if( *tp > nT )	continue;

			val = -1;
			sp0 = sp - sim->width -1;
			tp0 = tp - im->width -1;

			for( k = 0 ; k < 3 && val <  0 ; k++, sp0 += align, tp0 += align ){
				for( n = 0 ; n < 3  ; n++, sp0++, tp0++ ){
					if( *tp0 > nT ){
						val = *sp0;
						break;
					}
				}
			}

			if( val >=0 ){
				*sp = val;
				no++;
			}
		}

	}

	image_destroy( im, 1 );

	return( no );
}



static image_type *
image1_equal_neighbor( image_type *sim, image_type *im )
{
u_char	*sp,	*sp0,	*sp1,	*tp;
int	i,	j;
int	no;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );
	image1_const( im, 0 );


	for( i = 1, no = 0 ; i < sim->row-1 ; i ++ ){
		sp = IMAGE_PIXEL( sim, i, 1 );
		sp0 = sp - sim->width;
		sp1 = sp + sim->width;

		tp = IMAGE_PIXEL( im, i, 1 );
		for( j = 1 ; j < sim->column-1 ; j ++, sp++, sp0++, sp1++, tp++ )
			*tp =	(*(sp0-1) == *sp ) + (*(sp0) == *sp ) + (*(sp0+1) == *sp ) + 
					(*(sp -1) == *sp ) +                    (*(sp +1) == *sp ) + 
					(*(sp1-1) == *sp ) + (*(sp1) == *sp ) + (*(sp1+1) == *sp );
	}

	return( im );
}






