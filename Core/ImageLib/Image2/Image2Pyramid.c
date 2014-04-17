
#include	<math.h>

#include	"ImageType/ImageType.h"

static void	image2_pyramid_x( image_type *im, image_type *cim );

static void	image2_pyramid_y( image_type *im, image_type *cim );


static void	image2_pyramid_up_x( image_type *im, image_type *cim );

static void	image2_pyramid_up_y( image_type *im, image_type *cim );




// 1 4 6 4 1
image_type *
image2_pyramid_N( image_type *im )
{
image_type	*cim,	*tim;

	cim = image_create( im->row/2, im->column/2, 2, 1, NULL );

	tim = image_create( im->row, cim->column, 2, 1, NULL );

	image2_pyramid_x( im, tim );

	image2_pyramid_y( tim, cim );

	image_destroy( tim, 1 );


	return( cim );
}


static void
image2_pyramid_x( image_type *im, image_type *cim )
{
short	*sp,	*tp;
int	i,	j;


	for( i = 0 ; i < im->row ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i, 0 );
		tp = (short *)IMAGE_PIXEL( cim, i, 0 );

		*tp++ = (*(sp) + *(sp)*4L + *(sp) *6L + *(sp+1)*4L + *(sp+2))>>4;
		sp++;
		sp++;
		for( j = 1 ; j < cim->column-1 ; j++ ){
			*tp++ = (*(sp-2) + *(sp-1)*4L + *(sp) *6L + *(sp+1)*4L + *(sp+2))>>4;
			sp++;
			sp++;
		}
		*tp++ = (*(sp-2) + *(sp-1)*4L + *(sp) *6L + *(sp)*4L + *(sp))>>4;
	}
}




static void
image2_pyramid_y( image_type *im, image_type *cim )
{
short	*sp,	*tp;
int	i,	j;
int	l1,	l2;

	l1 = cim->column;
	l2 = cim->column*2;

	for( j = 0 ; j < cim->column ; j++ ){

		sp = (short *)IMAGE_PIXEL( im, 0, j );
		tp = (short *)IMAGE_PIXEL( cim, 0, j );

		*tp = (*(sp) + *(sp)*4L + *(sp) *6L + *(sp+l1)*4L + *(sp+l2))>>4;
		tp += l1;
		sp += l2;
		for( i = 1 ; i < cim->row-1 ; i++ ){
			*tp = (*(sp-l2) + *(sp-l1)*4L + *(sp) *6L + *(sp+l1)*4L + *(sp+l2) )>>4;
			tp += l1;
			sp += l2;
		}
		*tp++ = (*(sp-l2) + *(sp-l1)*4L + *(sp) *6L + *(sp)*4L + *(sp) )>>4;
	}
}



// 1 4 6 4 1
image_type *
image2_pyramid_up( image_type *im )
{
image_type	*cim,	*tim;

	cim = image_create( im->row*2, im->column*2, 2, 1, NULL );

	tim = image_create( im->row, cim->column, 2, 1, NULL );

	image2_pyramid_up_x( im, tim );

	image2_pyramid_up_y( tim, cim );

	image_destroy( tim, 1 );


	return( cim );
}


static void
image2_pyramid_up_x( image_type *im, image_type *cim )
{
short	*sp,	*tp;
int	i,	j;


	for( i = 0 ; i < im->row ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i, 0 );
		tp = (short *)IMAGE_PIXEL( cim, i, 0 );

		*tp++ = ((0) + (0)*4L + *(sp) *6L + (0)*4L + *(sp+1))/7;

		*tp++ = ((0) + *(sp)*4L + (0) *6L + *(sp+1)*4L + (0))>>3;
		sp++;


		for( j = 2 ; j < cim->column-2 ; j += 2 ){
//			*tp++ = (*(sp-2) + *(sp-1)*4L + *(sp) *6L + *(sp+1)*4L + *(sp+2))>>4;
			*tp++ = (*(sp-1) + (0)*4L + *(sp) *6L + (0)*4L + *(sp+1))>>3;

			*tp++ = ((0) + *(sp)*4L + (0) *6L + *(sp+1)*4L + (0))>>3;

			sp++;
		}


		*tp++ = (*(sp-1) + (0)*4L + *(sp) *6L + (0)*4L + (0))/7;

		*tp++ = *sp;// (*(0) + *(sp)*4L + *(0) *6L + *(0)*4L + *(0))>>2;

		sp++;
	}
}




static void
image2_pyramid_up_y( image_type *im, image_type *cim )
{
short	*sp,	*tp;
int	i,	j;
int	l1;

	l1 = im->column;

	for( j = 0 ; j < im->column ; j++ ){

		sp = (short *)IMAGE_PIXEL( im, 0, j );
		tp = (short *)IMAGE_PIXEL( cim, 0, j );


		*tp = ((0) + (0)*4L + *(sp) *6L + (0)*4L + *(sp+l1) )/7;
		tp += l1;

		*tp = ((0) + *(sp)*4L + (0) *6L + *(sp+l1)*4L + (0) )>>3;
		tp += l1;

		sp += l1;

		for( i = 2 ; i < cim->row-2 ; i += 2 ){
			*tp = (*(sp-l1) + (0)*4L + *(sp) *6L + (0)*4L + *(sp+l1) )>>3;
			tp += l1;

			*tp = ((0) + *(sp)*4L + (0) *6L + *(sp+l1)*4L + (0) )>>3;
			tp += l1;

			sp += l1;
		}


		*tp = (*(sp-l1) + (0)*4L + *(sp) *6L + (0)*4L + (0) )/7;
		tp += l1;

		*tp = *sp;
		tp += l1;

		sp += l1;
	}
}