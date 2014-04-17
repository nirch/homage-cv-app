/******************************
 ***	ImageComparison.c   ***
 ******************************/

#include	<math.h>

#include "ImageType/ImageType.h"


static void	image1_comparison( image_type *im0, image_type *im1,
					float *var,	/* Variabilty */
					float *dev,
					float *snr,	/* Signal to noise ratio */
					float *average,
					int *peak );
 
static void    image4_comparison( image_type *im0, image_type *im1,
					float var[],    /* Variabilty */
					float dev[],
					float snr[],    /* Signal to noise ratio */
					float average[],
					int peak[] );

static void    image3_comparison( image_type *im0, image_type *im1,
					float var[],    /* Variabilty */
					float dev[],
					float snr[],    /* Signal to noise ratio */
					float average[],
					int peak[] );





void
image_comparison( image_type *im0, image_type *im1,
		float var[],	/* Variabilty */
		float dev[],
		float snr[],	/* Signal to noise ratio */
		float average[],
		int peak[] )
{

	switch( im0->depth ){
	case 1:
		image1_comparison( im0, im1, var, dev, snr, average, peak );
	    break;

	case 3:
		image3_comparison( im0, im1, var, dev, snr, average, peak );
		break;

	case 4:
		image4_comparison( im0, im1, var, dev, snr, average, peak );
	    break;
	}
}



void
image1_comparison( image_type *im0, image_type *im1,
		float *var,	/* Variabilty */
		float *dev,
		float *snr,	/* Signal to noise ratio */
		float *average,
		int *peak )
{
u_char	*p0,	*p1;
int	row,	col,	pixels,	i,	j;
int	b0,	b1,	t;
float	m,	d;


	*var = 0;
	*dev = 0;
	*average = 0;
	*snr = 0;
	*peak = 0;

	m = 0;
	d = 0;

	row = MIN( IMAGE_ROW(im0), IMAGE_ROW(im1) );
	col = MIN( IMAGE_COLUMN(im0), IMAGE_COLUMN(im1) );


	for( i = 0 ; i < row ; i++ ){
		p0 = IMAGE_PIXEL( im0, i, 0 );
		p1 = IMAGE_PIXEL( im1, i, 0 );

		for( j = 0 ; j < col ; j++, p0++, p1++ ){
			b0 = *p0;
			b1 = *p1;

                        m += b0*b0;
 
                        t = b0 - b1;
                        d += t*t;
 
                        *average += t;
 
                        if( t < 0 ) t = -t;
                        *dev += t;
                        if( t > *peak )  *peak = t;
		}
	}




	pixels = row*col;

	*var = sqrt( d / pixels );
	*dev /= pixels;
	*average /= pixels;
	*snr =  ( d == 0 )? 0 : 20 * log10( m/d );
}




void
image4_comparison( image_type *im0, image_type *im1,
		float var[],	/* Variabilty */
		float dev[],
		float snr[],	/* Signal to noise ratio */
		float average[],
		int peak[] )
{
u_int	*p0,	*p1;
int	row,	col,	pixels,	i,	j;
int	b0,	b1,	t;
float	m[3],	d[3];


	var[0] = var[1] = var[2] = 0;
	dev[0] = dev[1] = dev[2] = 0;
	average[0] = average[1] = average[2] = 0;
	snr[0] = snr[1] = snr[2] = 0;
	peak[0] = peak[1] = peak[2] = 0;

	m[0] = m[1] = m[2] = 0;
	d[0] = d[1] = d[2] = 0;

	row = MIN( IMAGE_ROW(im0), IMAGE_ROW(im1) );
	col = MIN( IMAGE_COLUMN(im0), IMAGE_COLUMN(im1) );


	for( i = 0 ; i < row ; i++ ){
		p0 = IMAGE4_PIXEL( im0, i, 0 );
		p1 = IMAGE4_PIXEL( im1, i, 0 );

		for( j = 0 ; j < col ; j++, p0++, p1++ ){
			b0 = IMAGE4_RED(*p0);
			b1 = IMAGE4_RED(*p1);

                        m[SP_R] += b0*b0;
 
                        t = b0 - b1;
                        d[SP_R] += t*t;
 
                        average[SP_R] += t;
 
                        if( t < 0 ) t = -t;
                        dev[SP_R] += t;
                        if( t > peak[SP_R] )  peak[SP_R] = t;



			b0 = IMAGE4_GREEN(*p0);
			b1 = IMAGE4_GREEN(*p1);

                        m[SP_G] += b0*b0;
 
                        t = b0 - b1;
                        d[SP_G] += t*t;
 
                        average[SP_G] += t;
 
                        if( t < 0 ) t = -t;
                        dev[SP_G] += t;
                        if( t > peak[SP_G] )  peak[SP_G] = t;



			b0 = IMAGE4_BLUE(*p0);
			b1 = IMAGE4_BLUE(*p1);

                        m[SP_B] += b0*b0;
 
                        t = b0 - b1;
                        d[SP_B] += t*t;
 
                        average[SP_B] += t;
 
                        if( t < 0 ) t = -t;
                        dev[SP_B] += t;
                        if( t > peak[SP_B] )  peak[SP_B] = t;
		}
	}


	pixels = row*col;

	var[SP_R] = sqrt( d[SP_R] / pixels );
	dev[SP_R] /= pixels;
	average[SP_R] /= pixels;
	snr[SP_R] = ( d[SP_R] == 0 )? 0 : 20 * log10( m[SP_R]/d[SP_R] );


	var[SP_G] = sqrt( d[SP_G] / pixels );
	dev[SP_G] /= pixels;
	average[SP_G] /= pixels;
	snr[SP_G] =  ( d[SP_G] == 0 )? 0 : 20 * log10( m[SP_G]/d[SP_G] );


	var[SP_B] = sqrt( d[SP_B] / pixels );
	dev[SP_B] /= pixels;
	average[SP_B] /= pixels;
	snr[SP_B] =  ( d[SP_B] == 0 )? 0 : 20 * log10( m[SP_B]/d[SP_B] );
}



void
image3_comparison( image_type *im0, image_type *im1,
		float var[],	/* Variabilty */
		float dev[],
		float snr[],	/* Signal to noise ratio */
		float average[],
		int peak[] )
{
u_char	*sp0,	*sp1;
int	row,	col,	pixels,	i,	j;
int	b0,	b1,	t;
float	m[3],	d[3];


	var[0] = var[1] = var[2] = 0;
	dev[0] = dev[1] = dev[2] = 0;
	average[0] = average[1] = average[2] = 0;
	snr[0] = snr[1] = snr[2] = 0;
	peak[0] = peak[1] = peak[2] = 0;

	m[0] = m[1] = m[2] = 0;
	d[0] = d[1] = d[2] = 0;

	row = MIN( IMAGE_ROW(im0), IMAGE_ROW(im1) );
	col = MIN( IMAGE_COLUMN(im0), IMAGE_COLUMN(im1) );


	for( i = 0 ; i < row ; i++ ){
		sp0 = IMAGE_PIXEL( im0, i, 0 );
		sp1 = IMAGE_PIXEL( im1, i, 0 );

		for( j = 0 ; j < col ; j++ ){
			b0 = *sp0++;
			b1 = *sp1++;

            m[SP_R] += b0*b0;

            t = b0 - b1;
            d[SP_R] += t*t;

            average[SP_R] += t;

            if( t < 0 ) t = -t;
            dev[SP_R] += t;
            if( t > peak[SP_R] )  peak[SP_R] = t;



			b0 = *sp0++;
			b1 = *sp1++;

            m[SP_G] += b0*b0;

            t = b0 - b1;
            d[SP_G] += t*t;

            average[SP_G] += t;

            if( t < 0 ) t = -t;
            dev[SP_G] += t;
            if( t > peak[SP_G] )  peak[SP_G] = t;



			b0 = *sp0++;
			b1 = *sp1++;

            m[SP_B] += b0*b0;

            t = b0 - b1;
            d[SP_B] += t*t;

            average[SP_B] += t;

            if( t < 0 ) t = -t;
            dev[SP_B] += t;
            if( t > peak[SP_B] )  peak[SP_B] = t;
		}
	}


	pixels = row*col;

	var[SP_R] = sqrt( d[SP_R] / pixels );
	dev[SP_R] /= pixels;
	average[SP_R] /= pixels;
	snr[SP_R] = ( d[SP_R] == 0 )? 0 : 20 * log10( m[SP_R]/d[SP_R] );


	var[SP_G] = sqrt( d[SP_G] / pixels );
	dev[SP_G] /= pixels;
	average[SP_G] /= pixels;
	snr[SP_G] =  ( d[SP_G] == 0 )? 0 : 20 * log10( m[SP_G]/d[SP_G] );


	var[SP_B] = sqrt( d[SP_B] / pixels );
	dev[SP_B] /= pixels;
	average[SP_B] /= pixels;
	snr[SP_B] =  ( d[SP_B] == 0 )? 0 : 20 * log10( m[SP_B]/d[SP_B] );
}



void
image4_comparisonG( image_type *im0, image_type *im1, float *var, float *dev, float *average )
{
u_int	*p0,	*p1;
int	row,	col,	pixels,	i,	j;
int	b0,	b1,	t;
float	d;

 
	
	d = 0;
	*dev = *average = 0;

	row = MIN( IMAGE_ROW(im0), IMAGE_ROW(im1) );
	col = MIN( IMAGE_COLUMN(im0), IMAGE_COLUMN(im1) );


	for( i = 0 ; i < row ; i++ ){
		p0 = IMAGE4_PIXEL( im0, i, 0 );
		p1 = IMAGE4_PIXEL( im1, i, 0 );

		for( j = 0 ; j < col ; j++, p0++, p1++ ){
			b0 = IMAGE4_RED(*p0);
			b1 = IMAGE4_RED(*p1);

            t = b0 - b1;
            d += t*t;

			*average += t;

            if( t < 0 ) t = -t;
            *dev += t;


			b0 = IMAGE4_GREEN(*p0);
			b1 = IMAGE4_GREEN(*p1);

 
            t = b0 - b1;
            d += t*t;

			*average += t;

            if( t < 0 ) t = -t;
            *dev += t;




			b0 = IMAGE4_BLUE(*p0);
			b1 = IMAGE4_BLUE(*p1);

 
            t = b0 - b1;
            d += t*t;

			*average += t;

            if( t < 0 ) t = -t;
            *dev += t;

		}
	}


	pixels = 3*row*col;

	*var = sqrt( d / pixels );
	*dev /= pixels;
	*average /= pixels;
}