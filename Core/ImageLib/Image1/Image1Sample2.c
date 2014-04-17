
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


static void	image1_sample2_1331_x( image_type *sim, image_type *im );
static void	image1_sample2_1331_y( image_type *sim, image_type *im );




image_type *
image1_sample2_1331( image_type *sim, image_type *im )
{
image_type	*tim;

	im = image_recreate( im, sim->row/2, sim->column/2, 1, 1 );


	tim = image_create( sim->row, sim->column/2, 1, 1, NULL );

	image1_sample2_1331_x( sim,  tim );

	image1_sample2_1331_y( tim, im );

	image_destroy( tim, 1 );

	return( im );
}


static void
image1_sample2_1331_x( image_type *sim, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;
int	tmp;

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, i, 0 );

		tmp = sp[0] + 3*sp[0] + 3*sp[1] + sp[2];
		*tp++ = tmp >> 3;

		sp += 2;

		for( j = 1  ; 2*j + 1 < sim->width ; j++ ){
			tmp = sp[-1] + 3*sp[0] + 3*sp[1] + sp[2];

			*tp++ = tmp >> 3;
			sp += 2;
		}

		if( j < im->width ){
			tmp = sp[-1] + 3*sp[0] + 3*sp[1] + sp[1];

			*tp++ = tmp >> 3;
			sp += 2;
		}
	}

}




static void
image1_sample2_1331_y( image_type *sim, image_type *im )
{
u_char	*sp[4],	*tp;
int	i,	j;
int	tmp;

	for( i = 0 ; i < im->row ; i++ ){
		sp[1] = IMAGE_PIXEL( sim, 2*i, 0 );
		tp = IMAGE_PIXEL( im, i, 0 );
	
		sp[0] = ( i > 0 )? sp[1] - sim->width : sp[1];
		sp[2] = sp[1] + sim->width;
		sp[3] = ( 2*i+1 < sim->height )? sp[2] + sim->width : sp[2];


		for( j = 0  ; j < im->width ; j++ ){
			tmp = *sp[0] + 3* *sp[1] + 3* *sp[2] + *sp[3];

			*tp++ = tmp >> 3;
			sp[0]++;
			sp[1]++;
			sp[2]++;
			sp[3]++;
		}
	}

}

