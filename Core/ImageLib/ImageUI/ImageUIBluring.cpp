  /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"

static image_type *imageI_bluring_x( image_type *sim, int thresh );
static image_type *imageI_bluring_y( image_type *sim, int thresh );


image_type *
imageUI_bluring( image_type *sim, int n )
{
image_type *tim,	*im;

	tim  = imageI_bluring_x( sim, n );

	im  = imageI_bluring_y( tim, n );

	image_destroy( tim, 1 );
	return( im );
}



static image_type *
imageI_bluring_x( image_type *sim, int thresh )
{
//thresh = 5;
u_int	*sp0, *sp1, *sp2;
int	i,	j, sum, k;
image_type * im = image_create( sim->row, sim->column, 4, 1, NULL );

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0, sum = 0; j < (2*thresh + 1); j++){
			sp0 = (u_int *)IMAGE_PIXEL( sim, i, j );
			sum += *sp0;
		}

		k = thresh;
		sp1 = (u_int *)IMAGE_PIXEL( im, i, k++ );
		*sp1 = sum/(2*thresh + 1); 


		for( ; j < im->column ; j++ ){
			sp0 = (u_int *)IMAGE_PIXEL( sim, i, j );
			sp2 = (u_int *)IMAGE_PIXEL( sim, i, j-(2*thresh + 1) );
			sum += *sp0 - *sp2;
			sp1 = (u_int *)IMAGE_PIXEL( im, i, k++ );
			*sp1 = sum/(2*thresh + 1); 
		}


		for( ; k < im->column ; k++ ){
			sp0 = (u_int *)IMAGE_PIXEL( sim, i, k );
			sp1 = (u_int *)IMAGE_PIXEL( im, i, k );
			*sp1 = 0;
		}
		for( j = 0; j < thresh; j++){
			sp0 = (u_int *)IMAGE_PIXEL( sim, i, j );
			sp1 = (u_int *)IMAGE_PIXEL( im, i, j );
			*sp1 = 0;
		}

	}
	return( im );
}


image_type *
imageI_bluring_y( image_type *sim, int thresh )
{
u_int	*sp0, *sp1, *sp2;
int	i,	j, sum, k;
image_type * im = image_create( sim->row, sim->column, 4, 1, NULL );

	for( i = 0 ; i < im->column ; i++ ){
		for( j = 0, sum = 0; j < (2*thresh + 1); j++){
			sp0 = (u_int*)IMAGE_PIXEL( sim, j, i );
			sum += *sp0;
		}

		k = thresh;
		sp1 = (u_int *)IMAGE_PIXEL( im, k++ ,i);
		*sp1 = sum/(2*thresh + 1); 

		for( ; j < im->row ; j++ ){
			sp0 = (u_int*)IMAGE_PIXEL( sim, j, i );
			sp2 = (u_int*)IMAGE_PIXEL( sim, j-(2*thresh + 1), i );
			sum += *sp0 - *sp2;
			sp1 = (u_int*)IMAGE_PIXEL( im, k++ ,i);
			*sp1 = sum/(2*thresh + 1); 
		}

		for( ; k < im->row ; k++ ){
			sp0 = (u_int*)IMAGE_PIXEL( sim, k, i );
			sp1 = (u_int*)IMAGE_PIXEL( im, k, i );
			*sp1 = 0;
		}

		for( j = 0; j < thresh; j++){
			sp0 = (u_int*)IMAGE_PIXEL( sim, j, i );
			sp1 = (u_int*)IMAGE_PIXEL( im, j, i );
			*sp1 = 0;
		}

	}
	return( im );
}
