/************************
 ***   Image1Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"


static image_type *	image3_filter_box_x( image_type *sim, int N );
static image_type *	image3_filter_box_y( image_type *sim, int N, image_type *im );


image_type *
image3_filter_box( image_type *sim, int n, image_type *im )
{
image_type *tim;

	tim  = image3_filter_box_x( sim, n );

	im  = image3_filter_box_y( tim, n, im );


	image_destroy( tim, 1 );
	return( im );
}


static image_type *
image3_filter_box_x( image_type *sim, int N )
{
u_char	*sp0, *tp, *sp2;
int	i,	j, sum[3], k;
image_type * im;


	im = image_create( sim->row, sim->column, 3, 1, NULL );

	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, i, 0 );
		for( j = 0, sum[0] = sum[1] = sum[2] = 0; j < (2*N + 1); j++){
			sum[0] += *sp0++;
			sum[1] += *sp0++;
			sum[2] += *sp0++;
		}


		k = N;
		tp = IMAGE_PIXEL( im, i, N);
		*tp++ = sum[0]/(2*N + 1); 
		*tp++ = sum[1]/(2*N + 1); 
		*tp++ = sum[2]/(2*N + 1); 


		sp2 = sp0 -3*(2*N + 1);
		for( ; j < im->column ; j++ ){
			sum[0] += (*sp0++ - *sp2++);
			*tp++ = sum[0]/(2*N + 1); 

			sum[1] += (*sp0++ - *sp2++);
			*tp++ = sum[1]/(2*N + 1);

			sum[2] += (*sp0++ - *sp2++);
			*tp++ = sum[2]/(2*N + 1);

		}

		sp0 = IMAGE_PIXEL( sim, i, sim->column - N );
		for( k = 0; k < N ; k++ ){
			*tp++ = *sp0++;
			*tp++ = *sp0++;
			*tp++ = *sp0++;
		}

		sp0 = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, i, 0 );
		for( j = 0; j < N; j++){
			*tp++ = *sp0++;
			*tp++ = *sp0++;
			*tp++ = *sp0++;
		}

	}

	return( im );
}



static image_type *
image3_filter_box_y( image_type *sim, int N, image_type *im )
{
u_char	*sp0, *tp, *sp2;
int	i,	j, sum[3], k;
int	align;

	align = 3*sim->width - 3;

	im = image_recreate( im, sim->row, sim->column, 3, 1 );

	for( i = 0 ; i < im->column ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 0, i );
		for( j = 0, sum[0] = sum[1] = sum[2] = 0; j < (2*N + 1); j++){
			sum[0] += *sp0++;
			sum[1] += *sp0++;
			sum[2] += *sp0++;
			sp0 += align;

		}

		k = N;
		sp2 = sp0 - 3*(2*N + 1)*sim->width;
		tp = IMAGE_PIXEL( im, N ,i);
		*tp++ = sum[0]/(2*N + 1);
		*tp++ = sum[1]/(2*N + 1);
		*tp++ = sum[2]/(2*N + 1);
		tp += align;


		for( ; j < im->row ; j++,  sp0 += align, sp2 += align, tp += align){
			sum[0] += *sp0++ - *sp2++;
			*tp++ = sum[0]/(2*N + 1); 
			sum[1] += *sp0++ - *sp2++;
			*tp++ = sum[1]/(2*N + 1); 
			sum[2] += *sp0++ - *sp2++;
			*tp++ = sum[2]/(2*N + 1); 

		}

		sp0 = IMAGE_PIXEL( sim, sim->row - N, i );
		for( k = 0; k < N ; k++, sp0 += align, tp += align ){
			*tp++ = *sp0++;
			*tp++ = *sp0++;
			*tp++ = *sp0++;
		}

		sp0 = IMAGE_PIXEL( sim, 0, i );
		tp = IMAGE_PIXEL( im, 0, i );
		for( j = 0; j < N; j++, sp0 += align, sp2 += align, tp += align){
			*tp++ = *sp0++;
			*tp++ = *sp0++;
			*tp++ = *sp0++;
		}

	}
	return( im );
}
