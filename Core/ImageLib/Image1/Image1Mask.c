
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


static void	image1_mask_area_x( image_type *sim, int rC, image_type *im );


static void	image1_mask_area_y( image_type *sim, int rC, image_type *im );




image_type *
image1_mask_area( image_type *sim, int rC, image_type *im )
{
image_type	*tim;

	im = image_reallocL( sim, im );

	tim = image_alloc( sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );

	image1_mask_area_x( sim, rC, tim );

	image1_mask_area_y( tim, rC, im );

	image_destroy( tim, 1 );


	image_mask( im, sim, 0, im );

	return( im );
}


static void
image1_mask_area_x( image_type *sim, int rC, image_type *im )
{
u_char	*sp,	*sp0,	*sp1;
u_short	*tp;
int	i,	j;
int	sum;

	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGEUS_PIXEL( im, i, 0 );

		sp0 = sp - rC;
		for( j = 0, sum = 0 ; j < rC ; j++, sp ++ )
			sum += ( *sp != 0 );

		sp1 = sp;
		for( j = 0 ; j < im->width ; j++, tp++, sp0++, sp1++ ){
			if( j > rC )
				sum -= ( *sp0 != 0 );
			if( j+rC < sim->height )
				sum += ( *sp1 != 0 );


			*tp = sum;
		}
	}
}


static void
image1_mask_area_y( image_type *sim, int rC, image_type *im )
{
u_short	*sp,	*sp0,	*sp1;
u_char	*tp;
int	i,	j;
int	sum;

	float	t;
	t = ( 2*rC +1)*( 2*rC +1);
	t = 255 / t;

	for( j = 0 ; j < im->width ; j++ ){
		sp = IMAGEUS_PIXEL( sim, 0, j );
		tp = IMAGE_PIXEL( im, 0, j );

		sp0 = sp - rC*im->width;
		for( i = 0, sum = 0 ; i < rC ; i++, sp += sim->width )
			sum += *sp;

		sp1 = sp;
		for( i = 0 ; i < im->height ; i++, tp += im->width, sp0 += sim->width, sp1 += sim->width ){
			if( i > rC )
				sum -= *sp0;
			if( i+rC < sim->height )
				sum += *sp1;


			*tp = sum*t;
		}


	}
}




