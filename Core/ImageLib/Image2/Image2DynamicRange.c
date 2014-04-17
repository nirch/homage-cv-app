/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"

#define         PIXEL_12_TO_8( x )   \
	      		( ( (x) <= 0 )? 0 : (( (x) > 4096 )? 255 : ((x)>>4) ) )



void
image2_dynamic_range( image_type *im, box2i *box, float h0, float h1, float maxD )
{
short	*tp;
int	i,	j;

int	h[4096];
int	tmp;
float	a0,	a1,	a,	b;



	image2_histogram( im, box,  h );


	histogram_range( h, 4096, h0, h1, &a0, &a1 );



	if( maxD > 0 && a1 < 4096 - maxD*16 )	a1 = 4096 - maxD*16;
	


	a = 4095/(a1-a0);
	b = - a* a0;

	

	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = a * (*tp) + b;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );
		}
	}
}



void
image2_range( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 )
{
int	h[4096];

	image2_histogram( im, box,  h );


	histogram_range( h, 4096, h0, h1, a0, a1 );
}


int
image2_histogram( image_type *im, box2i *box, int h[4096] )
{
short	*sp;
int	i,	j,	n,	tmp;
int	i0,	i1,	j0,	j1;


	imageA_box( im , box, &j0, &i0, &j1, &i1 );




	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;

	
	for( i = i0, n = 0 ; i < i1 ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i, j0 );
		for( j = j0 ; j < j1; j++ ){
			tmp = *sp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;
			n++;
		}
	}

	return( n );
}