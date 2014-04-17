
#include	<math.h>

#include	"ImageType/ImageType.h"


static void	image3_convolution_y( image_type *im, float C[], int rC, image_type *cim );

static void	image3_convolution_x( image_type *im, float C[], int rC, image_type *cim );


image_type *
image3_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim )
{
image_type	*tim;

	if( cim == NULL )
		cim = image_create( im->row, im->column, 3, 1, NULL );


	tim = image_create( im->row, im->column, 3, 1, NULL );

	image3_convolution_x( im, Cx, rC, tim );

	image3_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );

	return( cim );
}


static void
image3_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r,	g,	b;
int	gi[32];

	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ ){
			*tp++ = *p++;
			*tp++ = *p++;
			*tp++ = *p++;
		}


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 3 ){

			for( k = 0, r = g = b = 0, p = sp; k < n ; k++ ){
				r += gi[k] * (*p++);
				g += gi[k] * (*p++);
				b += gi[k] * (*p++);
			}

			r >>= 16;
			g >>= 16;
			b >>= 16;

			*tp++ = PUSH_TO_RANGE( r, 0, 255 );
			*tp++ = PUSH_TO_RANGE( g, 0, 255 );
			*tp++ = PUSH_TO_RANGE( b, 0, 255 );
		}


		sp += rC*3;
		for( j = 0 ; j < rC ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}
}



static void
image3_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r,	g,	b;
int	gi[32];
int	align;



	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	align = im->column*3;


	sp = im->data;  // IMAGE_PIXEL( im, 0, 0 );
	tp = cim->data;	// IMAGE_PIXEL( cim, 0, 0 );
	for( i = 0 ; i < rC ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}

	sp = im->data;
	for(  ; i < im->row - rC ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			for( k = 0, r = g = b = 0, p = sp; k < n  ; k++, p += align ){
				r += gi[k] * (*p);

				g += gi[k] * (*(p+1));

				b += gi[k] * (*(p+2));

			}

			r >>= 16;
			g >>= 16;
			b >>= 16;

			*tp++ = PUSH_TO_RANGE( r, 0, 255 );
			*tp++ = PUSH_TO_RANGE( g, 0, 255 );
			*tp++ = PUSH_TO_RANGE( b, 0, 255 );
		}
	}


	sp = IMAGE_PIXEL( im, i, 0 );
	for(  ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}
}

