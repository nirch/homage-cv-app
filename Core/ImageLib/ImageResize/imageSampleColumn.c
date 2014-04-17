/********************************
 ***	ImageSampleColumn.c   ***
 ********************************/

#include "ImageType/ImageType.h"

#include "ImageResize.h"


static image_type *	image1_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 );

static image_type *	imageUS_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 );

image_type *	
image_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 )
{
	if( sim->format == IMAGE_FORMAT_US ){
		im = imageUS_sample_column( sim, si0, d, im, i0 );
		return( im );
	}


	if( sim->depth == 1 ){
		im = image1_sample_column( sim, si0, d, im, i0 );
		return( im );
	}



	gpWarning( "image_sample_column", "Unsuported format" );

	if( im != NULL )
		image_destroy( im, 1 );



	return( NULL );
}


static image_type *
image1_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 )
{
	u_char	*sp,	*sp0,	*tp;
	int	j;
	int	k,	n;
	int	align,	sum;

	align = sim->width - d;
	tp = IMAGE_PIXEL( im, i0, 0 );


	sp0 = IMAGE_PIXEL( sim, si0, 0 );

	for( j = 0 ; j + d < im->column ; j++, sp0 += d ){

		for( k = 0, sp = sp0, sum = 0 ; k < d ; k++, sp += align ){
			for( n = 0 ; n < d ; n++ )
				sum += *sp++;
		}

		*tp++ = sum /( d*d );
	}


	return( im );
}

static image_type *
imageUS_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 )
{
u_short	*sp,	*sp0,	*tp;
int	j;
int	k,	n;
int	align,	sum;

	align = sim->width - d;
	tp = (u_short *)IMAGE_PIXEL( im, i0, 0 );


	sp0 = (u_short *)IMAGE_PIXEL( sim, si0, 0 );

	for( j = 0 ; j + d < im->column ; j++, sp0 += d ){

		for( k = 0, sp = sp0, sum = 0 ; k < d ; k++, sp += align ){
			for( n = 0 ; n < d ; n++ )
				sum += *sp++;
		}

		*tp++ = sum /( d*d );
	}


	return( im );
}