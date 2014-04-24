/****************************
 ***    ImageHistogram.c  ***
 ***************************/
 
#include	"ImageType/ImageType.h"
 
 
 
int
image_histogram( image_type *im, int h[256] )
{
u_char  *p;
int     row,    col,    i,      j;
 

	if( im->depth != 1 )	return( 0 );

	for( i = 0 ; i < 256 ; i++ )	h[i] = 0;

	row = IMAGE_ROW(im);
	col = IMAGE_COLUMN(im);
 
 
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
 
			h[ *p ] ++;
			p++;
		}
	}

	return( 1 );
}  




image_type *
image_histogram_equlization( image_type *sim )
{
image_type	*im;
int	T[256],	H[256];
int	pixles,	sum;
u_char	*p,	*sp;
int	i,	j;

	image_histogram( sim, H );

	pixles = IMAGE_ROW(sim) * IMAGE_COLUMN(sim);


	for( i = 0, sum = 0 ; i < 256 ; i++ ){
		sum += H[i];
		T[i] = (int)(255 * sum / (float)pixles);
	}


	im = image_create( IMAGE_ROW(sim), IMAGE_COLUMN(sim), 1, 1, NULL );
 
	sp = (u_char *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
			*p++ = T[ *sp++ ];

	return( im );
}


image_type *
image_histogram_curve( image_type *sim, float partL, int limL, 
			float partR, int limR )
{
image_type	*im;
int	H[256];
int	pixles, sum;
int	partR_pixles, partL_pixles;
u_char	*p,	*sp;
int	i, j, left, right;
float rel;

	image_histogram( sim, H );

	pixles = IMAGE_ROW(sim) * IMAGE_COLUMN(sim);

	partL_pixles = ((float)pixles)*partL/100.0F;
	if ( limL < 0 || limL > 255 ) 
		limL = 255;
	left = limL;
	
	for( i = 0, sum = 0 ; i <= limL; i++ ){
		sum += H[i];
		if ( sum > partL_pixles )
		{
			left = i;
			break;
		}
	}

	partR_pixles = ((float)pixles)*partR/100.0F;
	if ( limR < 0 || limR > 255 ) 
		limR = 0;
	right = limR;

	for( i = 255, sum = 0 ; i >= limR ; i-- ){
		sum += H[i];
		if ( sum > partR_pixles )
		{
			right = i;
			break;
		}
	}

	rel = 255.0F/((float)(right - left));

	im = image_create( IMAGE_ROW(sim), IMAGE_COLUMN(sim), 1, 1, NULL );
 
	sp = (u_char *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
		{
			*p++ = *sp <= left ? 0 : 
				( *sp >= right ? 255 : (*sp - left)*rel );
			sp++;
		}

	return( im );
}


