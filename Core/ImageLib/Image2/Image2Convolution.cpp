
#include	<math.h>

#include	"ImageType/ImageType.h"
//#include	"SSEUtils.h"

//#include <xmmintrin.h>


image_type *	image2_convolution_separably_MMX5( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim );


static void	image2_convolution_y( image_type *im, float C[], int rC, image_type *cim );

static void	image2_convolution_x( image_type *im, float C[], int rC, image_type *cim );

static void	image2_convolution_x_1( image_type *im, float C[], int rC, image_type *cim );


image_type *
image2_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim )
{
image_type	*tim;

#ifdef MMX
	if( rC == 3 ){
		cim = image2_convolution_separably_MMX5( im,
						Cy, Cx, rC,
						cim );
		return( cim );
	}
#endif

	if( cim == NULL )
		cim = image_create( im->row, im->column, 2, 1, NULL );


	tim = image_create( im->row, im->column, 2, 1, NULL );


	if( im->depth == 1 )
			image2_convolution_x_1( im, Cx, rC, tim );
	else
#ifdef MMX
		if( rC == 2 )
			image2_convolution_x_MMX5( im, Cx, tim );
		else
#endif
			image2_convolution_x( im, Cx, rC, tim );


	image2_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );

	return( cim );
}

#ifdef MMX
image_type *
image2_convolution_separably_MMX5( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim )
{
	if( cim == NULL )
		cim = image_create( im->row, im->column, 2, 1, NULL );

	if( im->depth == 1 )
			image2_copy_image1( im, cim );
	else	image2_copy( im, cim );


	image2_convolution((unsigned short*)im->data , im->column * im->row,
						(unsigned short*) cim->data);
	

	return( cim );

}
#endif


static void
image2_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
short	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r;
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
		}


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 1 ){

			for( k = 0, r = 0, p = sp; k < n ; k++ ){
				r += gi[k] * (*p++);
			}


//			*tp++ = r >> 16;
			*tp++ = r >> 15;
		}


		sp += rC;
		for( j = 0 ; j < rC ; j++ ){
			*tp++ = *sp++;
		}
	}
}


static void
image2_convolution_x_1( image_type *im, float C[], int rC, image_type *cim )
{
short	*tp;
u_char	*sp,	*p;
int	i,	j,	k,	n;
int	r;
int	gi[32];

	n = 2*rC+1;

	for( i = 0 ; i < n ; i++ )
		gi[i] = C[i] * ( 1<<16);

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = (short *)IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ ){
			*tp++ = *(p++) << 4;
		}


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 1 ){

			for( k = 0, r = 0, p = sp; k < n ; k++ ){
				r += gi[k] * (*p++);
			}


			*tp++ = r >> 12;//16;
		}


		sp += rC;
		for( j = 0 ; j < rC ; j++ ){
			*tp++ = *(sp++) << 4;
		}
	}
}

static void
image2_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
short	*sp,	*tp,	*p;
int	i,	j,	k,	n;
int	r;
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
		}
	}

	sp = (short *)im->data;
	for(  ; i < im->row - rC ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 1 ){
			for( k = 0, r = 0, p = sp; k < n  ; k++, p += align ){
				r += gi[k] * (*p);

			}

			*tp++ = r >> 16;
		}
	}


	sp = (short *)IMAGE_PIXEL( im, i, 0 );
	for(  ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}
}

