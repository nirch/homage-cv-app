
#include	<math.h>

#include	"ImageType/ImageType.h"


static void	image1_pyramid_x( image_type *im, image_type *cim );

static void	image1_pyramid_y( image_type *im, image_type *cim );


image_type *
image1_pyramid( image_type *im )
{
image_type	*cim,	*tim;

	cim = image_create( im->row/2, im->column/2, 1, 1, NULL );

	tim = image_create( im->row, cim->column, 1, 1, NULL );

	image1_pyramid_x( im, tim );

	image1_pyramid_y( tim, cim );

	image_destroy( tim, 1 );


	return( cim );
}


static void
image1_pyramid_x( image_type *im, image_type *cim )
{
u_char	*sp,	*tp;
int	i,	j;


	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = IMAGE_PIXEL( cim, i, 0 );

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
image1_pyramid_y( image_type *im, image_type *cim )
{
u_char	*sp,	*tp;
int	i,	j;
int	l1,	l2;

	l1 = cim->column;
	l2 = cim->column*2;

	for( j = 0 ; j < cim->column ; j++ ){

		sp = IMAGE_PIXEL( im, 0, j );
		tp = IMAGE_PIXEL( cim, 0, j );

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

