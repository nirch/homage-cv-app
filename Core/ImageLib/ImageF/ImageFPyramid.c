
#include	<math.h>

#include	"ImageType/ImageType.h"

static void	imageF_pyramid_x( image_type *im, image_type *cim );

static void	imageF_pyramid_y( image_type *im, image_type *cim );


static void	imageF_pyramid_up_x( image_type *im, image_type *cim );

static void	imageF_pyramid_up_y( image_type *im, image_type *cim );

image_type *
imageF_pyramidL_121( image_type *sim, int level, image_type *im )
{
image_type	*tim,	*tim1;
int	i;

	tim = sim;
	for( i = 0 ; i < level-1 ; i++ ){
		tim1 = tim;
		tim = imageF_pyramid( tim );
		if( tim1 != sim )
			image_destroy( tim1, 1 );
	}

	if( im != NULL )
		image_destroy( im, 1 );
	im = imageF_pyramid( tim );

	return( im );
}


// 1 2 1
image_type *
imageF_pyramid( image_type *im )
{
image_type	*cim,	*tim;

	cim = image_create( im->row/2, im->column/2, 4, 1, NULL );

	tim = image_create( im->row, cim->column, 4, 1, NULL );

	imageF_pyramid_x( im, tim );

	imageF_pyramid_y( tim, cim );

	image_destroy( tim, 1 );


	return( cim );
}


static void
imageF_pyramid_x( image_type *im, image_type *cim )
{
float	*sp,	*tp;
int	i,	j;


	for( i = 0 ; i < im->row ; i++ ){
		sp = (float*)IMAGE_PIXEL( im, i, 0 );
		tp = (float*)IMAGE_PIXEL( cim, i, 0 );

		*tp++ = (*(sp) + *(sp) *2 + *(sp+1))*0.25;
		sp++;
		sp++;
		for( j = 1 ; j < cim->column-1 ; j++ ){
			*tp++ = (*(sp-1) + *(sp) *2 + *(sp+1))*0.25;
			sp++;
			sp++;
		}
		*tp++ = (*(sp-1) + *(sp) *2 + *(sp))*0.25;
	}
}




static void
imageF_pyramid_y( image_type *im, image_type *cim )
{
float	*sp,	*tp;
int	i,	j;
int	l1,	l2;

	l1 = cim->column;
	l2 = cim->column*2;

	for( j = 0 ; j < cim->column ; j++ ){

		sp = (float*)IMAGE_PIXEL( im, 0, j );
		tp = (float*)IMAGE_PIXEL( cim, 0, j );

		*tp = (*(sp) + *(sp) *2L + *(sp+l1))*0.25;
		tp += l1;
		sp += l2;
		for( i = 1 ; i < cim->row-1 ; i++ ){
			*tp = (*(sp-l1) + *(sp) *2 + *(sp+l1) )*0.25;
			tp += l1;
			sp += l2;
		}
		*tp++ = (*(sp-l1) + *(sp) *2 + *(sp))*0.25;
	}
}



// 1 2 1
image_type *
imageF_pyramid_up( image_type *im, image_type *cim )
{
image_type	*tim;

	if( cim == NULL )
		cim = image_create( im->row*2, im->column*2, 4, 1, NULL );

	tim = image_create( im->row, cim->column, 4, 1, NULL );

	imageF_pyramid_up_x( im, tim );

	imageF_pyramid_up_y( tim, cim );

	image_destroy( tim, 1 );


	return( cim );
}


static void
imageF_pyramid_up_x( image_type *im, image_type *cim )
{
float	*sp,	*tp;
int	i,	j;


	for( i = 0 ; i < im->row ; i++ ){
		sp = (float*)IMAGE_PIXEL( im, i, 0 );
		tp = (float*)IMAGE_PIXEL( cim, i, 0 );

		*tp++ = *(sp);

		*tp++ = (*(sp) + *(sp+1))*0.5;
		sp++;


		for( j = 2 ; j < cim->column-2 ; j += 2 ){
			*tp++ = *sp;

			*tp++ = (*(sp) + *(sp+1))*0.5;

			sp++;
		}


		*tp++ = *sp;
		*tp++ = *sp;

		sp++;
	}
}




static void
imageF_pyramid_up_y( image_type *im, image_type *cim )
{
float	*sp,	*tp;
int	i,	j;
int	l1;

	l1 = cim->column;

	for( j = 0 ; j < im->column ; j++ ){

		sp = (float*)IMAGE_PIXEL( im, 0, j );
		tp = (float*)IMAGE_PIXEL( cim, 0, j );



		*tp = *sp;
		tp += l1;

		*tp = (*(sp) + *(sp+l1) )*0.5;
		tp += l1;

		sp += l1;

		for( i = 2 ; i < cim->row-2 ; i += 2 ){
			*tp = *sp;
			tp += l1;

			*tp = (*(sp) + *(sp+l1) )*0.5;
			tp += l1;

			sp += l1;
		}


		*tp = *sp;
		tp += l1;

		*tp = *sp;
		tp += l1;

		sp += l1;
	}
}







// 1 2 1
image_type *
imageF_pyramid_up_121( image_type *im, image_type *cim )
{
float	*sp,	*tp0,	*tp1,	*tp2;
int	i,	j;


	if( cim == NULL )
		cim = image_create( im->row*2, im->column*2, 4, 1, NULL );




	// first row
	sp = (float*)IMAGE_PIXEL( im, 0, 0 );
	tp0 = (float*)IMAGE_PIXEL( cim, 0, 0 );



	*tp0 = *sp++;
	for( j = 0 ; j < cim->column-2 ; j += 2 ){
		*(tp0+2) = *sp++;
		*(tp0+1) = ( *tp0 + *(tp0+2) )*0.5;
		tp0 += 2;
	}
	*(tp0+1) = *tp0;



	for( i = 0 ; i < im->row-1 ; i++ ){
		sp = (float*)IMAGE_PIXEL( im, i+1, 0 );
		tp0 = (float*)IMAGE_PIXEL( cim, 2*i, 0 );
		tp1 = tp0 + cim->column;
		tp2 = tp1 + cim->column;



		*tp2 = *sp++;
		*tp1 = (*tp0 + *tp2)*0.5;


		for( j = 0 ; j < cim->column-2 ; j += 2 ){
			*(tp2+2) = *sp++;
			*(tp1+2) = ( *(tp0+2) + *(tp2+2) ) *0.5;


			*(tp1+1) = ( *tp1 + *(tp1+2)) *0.5;
			*(tp2+1) = ( *tp2 + *(tp2+2)) *0.5;

			tp0 += 2;
			tp1 += 2;
			tp2 += 2;
		}

		*(tp1 + 1 ) = *tp1;
		*(tp2 + 1 ) = *tp2;

	}




	// lasat row
	tp0 = (float*)IMAGE_PIXEL( cim, cim->row-2, 0 );
	tp1 = tp0 + cim->column;
	for( j = 0 ; j < cim->column ; j++ )
		*tp1++ = *tp0++;


	return( cim );
}