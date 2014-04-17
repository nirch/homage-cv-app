
#include	<math.h>

#include	"ImageType/ImageType.h"


static void	image6_convolution_y( image_type *im, float C[], int rC, image_type *cim );

static void	image6_convolution_x( image_type *im, float C[], int rC, image_type *cim );

static void	image6_convolution_x_1( image_type *im, float C[], int rC, image_type *cim );


image_type *
image6_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim )
{
image_type	*tim;

	if( cim == NULL )
		cim = image_create( im->row, im->column, 6, 1, NULL );


	tim = image_create( im->row, im->column, 6, 1, NULL );


	if( im->depth == 1 )
			image6_convolution_x_1( im, Cx, rC, tim );
	else
			image6_convolution_x( im, Cx, rC, tim );


	image6_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );

	return( cim );
}



static void
image6_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
short	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r,	g,	b;
short gi[32];

	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
	//	gi[i] = C[i] * ( 1<<16);
		gi[i] = C[i] * ( 1<<15);

	for( i = 0 ; i < im->row ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i, 0 );
		tp = (short *)IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ ){
			*tp++ = *p++;
			*tp++ = *p++;
			*tp++ = *p++;
		}


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 3 ){
			r = g = b = 0;
			for( k = 0, p = sp ; k < n ; k++ ){
				r += gi[k] * (*p++);
				g += gi[k] * (*p++);
				b += gi[k] * (*p++);
			}


//			*tp++ = r >> 16;
			*tp++ = r >> 15;
			*tp++ = g >> 15;
			*tp++ = b >> 15;
		}


		sp += 3*rC;
		for( j = 0 ; j < rC ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}
}


static void
image6_convolution_x_1( image_type *im, float C[], int rC, image_type *cim )
{
short	*tp;
u_char	*sp,	*p;
int	i,	j,	k,	n;
int	r,	g,	b;
int	gi[32];

	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = (short *)IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ ){
			*tp++ = *(p++) << 4;
			*tp++ = *(p++) << 4;
			*tp++ = *(p++) << 4;
		}


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 3 ){
			r = g = b = 0;
			for( k = 0, p = sp; k < n ; k++ ){
				r += gi[k] * (*p++);
				g += gi[k] * (*p++);
				b += gi[k] * (*p++);
			}


			*tp++ = r >> 12;//16;
			*tp++ = g >> 12;//16;
			*tp++ = b >> 12;//16;
		}


		sp += 3*rC;
		for( j = 0 ; j < rC ; j++ ){
			*tp++ = *(sp++) << 4;
			*tp++ = *(sp++) << 4;
			*tp++ = *(sp++) << 4;
		}
	}
}

static void
image6_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
short	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r,	g,	b;
int	gi[32];
int	align;



	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	align = im->column;


	sp = (short *)im->data;
	tp = (short *)cim->data;
	for( i = 0 ; i < rC ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}

	sp = (short *)im->data;
	for(  ; i < im->row - rC ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			r = g = b = 0;
			for( k = 0, p = sp ; k < n  ; k++ ){
				r += gi[k] * (*p);
				p += align;
				g += gi[k] * (*p);
				p += align;
				b += gi[k] * (*p);
				p += align;
			}

			*tp++ = r >> 16;
			*tp++ = g >> 16;
			*tp++ = b >> 16;
		}
	}


	sp = (short *)IMAGE_PIXEL( im, i, 0 );
	for(  ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}
}

