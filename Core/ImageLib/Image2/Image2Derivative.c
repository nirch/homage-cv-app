/******************************
 ***   Image2Derivative.c   ***
 ******************************/
#include	<math.h>
#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"


image_type *
image2_derivative_x( image_type *sim, image_type *im )
{
short	*sp,	*tp;
int	i,	j;

//	if( im == NULL )
	im = image_recreate( im, sim->row, sim->column, 2, 1 );


	for( i = 0 ; i < im->row ; i++ ){
		sp = (short*)IMAGE_PIXEL( sim, i, 0 );
		tp = (short*)IMAGE_PIXEL( im, i, 0 );

		*tp++ = *(sp+1) - *sp;
		sp++;
		for( j = 1 ; j < im->column-1 ; j++, sp++ )
			*tp++ = (*(sp+1) - *(sp-1) )>>1;

		*tp++ = *(sp) - *(sp-1);
	}

	return( im );
}


image_type *
image2_derivative_y( image_type *sim, image_type *im )
{
short	*sp0,	*sp1,	*tp;
int	i,	j;

//	if( im == NULL )
//		im = image_create( sim->row, sim->column, 2, 1, NULL );
	im = image_recreate( im, sim->row, sim->column, 2, 1 );


	tp = (short*)im->data;

	sp0 = (short*)IMAGE_PIXEL( sim, 0, 0 );
	sp1 = (short*)IMAGE_PIXEL( sim, 1, 0 );
	for( j = 0 ; j < im->column ; j++, sp0++, sp1++ )
		*tp++ = *sp1 - *sp0;


	sp0 = (short*)IMAGE_PIXEL( sim, 0, 0 );
	sp1 = (short*)IMAGE_PIXEL( sim, 2, 0 );
	for( i = 1 ; i < im->row-1 ; i++ ){
//		for( j = 1 ; j < im->column-1 ; j++, sp0++, sp1++ )
		for( j = 0 ; j < im->column ; j++, sp0++, sp1++ )
			*tp++ = (*(sp1) - *(sp0) )>>1;
	}


	sp1 = sp0 + im->column;
	for( j = 0 ; j < im->column ; j++, sp0++, sp1++ )
		*tp++ = *sp1 - *sp0;


	return( im );
}