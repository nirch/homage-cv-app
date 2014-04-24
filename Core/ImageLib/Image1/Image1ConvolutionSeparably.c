
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


static void	image1_convolution_y( image_type *im, float C[], int rC, image_type *cim );

static void	image1_convolution_x( image_type *im, float C[], int rC, image_type *cim );


#define	MAX_RC	100


image_type *
image1_convolution_guasian( image_type *sim,
							 float sigma,
							 image_type *cim )
{
int	rC;
float	g[MAX_RC];

	rC = (int)(3*sigma);
	if( rC < 1 )	rC = 1;
	guasian( rC, sigma, g );


	cim = image1_convolution_separably( sim, g, g, rC, cim );

	return( cim );
}







//#define _TEST_
#ifndef _TEST_
image_type *
image1_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim )
{
image_type	*tim;

	if( cim == NULL )
		cim = image_create( im->row, im->column, 1, 1, NULL );


	tim = image_create( im->row, im->column, 1, 1, NULL );

	image1_convolution_x( im, Cx, rC, tim );

	image1_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );

	return( cim );
}
#else
image_type *
image1_convolution_separably( image_type *im,
                             float Cy[], float Cx[], int rC,
                             image_type *cim )
{
static image_type	*tim = NULL ;
    
	if( cim == NULL )
		cim = image_create( im->row, im->column, 1, 1, NULL );
    
    
	tim = image_recreate( tim, im->row, im->column, 1, 1 );
    
	image1_convolution_x( im, Cx, rC, tim );
    
	image1_convolution_y( tim, Cy, rC, cim );
    
//	image_destroy( tim, 1 );
    
	return( cim );
}
#endif


static void
image1_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r;
int	gi[MAX_RC];

	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ )
			*tp++ = *p++;


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 1 ){

			for( k = 0, r = 0, p = sp; k < n ; k++ ){
				r += gi[k] * (*p++);
			}

			r >>= 16;

			*tp++ = PUSH_TO_RANGE( r, 0, 255 );
		}


		sp += rC;
		for( j = 0 ; j < rC ; j++ )
			*tp++ = *sp++;

	}
}



static void
image1_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r;
int	gi[MAX_RC];
int	align;



	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	align = im->column;


	sp = im->data;  // IMAGE_PIXEL( im, 0, 0 );
	tp = cim->data;	// IMAGE_PIXEL( cim, 0, 0 );
	for( i = 0 ; i < rC ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}

	sp = im->data;
	for(  ; i < im->row - rC ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 1 ){
			for( k = 0, r = 0, p = sp; k < n  ; k++, p += align ){
				r += gi[k] * (*p);
			}

			r >>= 16;

			*tp++ = PUSH_TO_RANGE( r, 0, 255 );
		}
	}


	sp = IMAGE_PIXEL( im, i, 0 );
	for(  ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}
}

