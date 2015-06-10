
#include	<math.h>

#include	"ImageType/ImageType.h"


static void	image1_convolution_x( image_type *im, float C[], int rC, image_type *cim );
static void	image1_convolution_y( image_type *im, float C[], int rC, image_type *cim );


static void	imagef_convolution_x( image_type *im, float C[], int rC, image_type *cim );
static void	imagef_convolution_x_sub( image_type *im, image_type *im1, float C[], int rC, image_type *cim );
static void	imagef_convolution_y( image_type *im, float C[], int rC, image_type *cim );



void
image1_convolution_separably( image_type *im,
			float Cy[], float Cx[], int rC,
			image_type *cim )
{
image_type	*tim;

	tim = image_create( im->row, im->column, 1, 1, NULL );

	image1_convolution_x( im, Cx, rC, tim );

	image1_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );
}


static void
image1_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*tp,	*p;
int	i,	j,	k,	n;
float	sum;

	n = 2*rC+1;

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = IMAGE_PIXEL( cim, i, rC );
		for( j = 0 ; j < im->column-n+1 ; j++, sp++, tp++ ){

			for( k = 0, sum = 0, p = sp; k < n ; k++, p++ )
				sum += C[k] * (*p);

			*tp = sum;
		}
	}
}

static void
image1_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*tp,	*p;
int	i,	j,	k,	n;
float	sum;


	n = 2*rC+1;


//	for( j = 0 ; j < im->column ; j++ ){
	for( j = rC ; j < im->column - rC ; j++ ){

		sp = IMAGE_PIXEL( im, 0, j );
		tp = IMAGE_PIXEL( cim, rC, j );

		for( i = 0 ; i < im->row-n+1 ; i++, sp += im->column, tp += cim->column ){

			for( k = 0, sum = 0, p = sp; k < n  ; k++, p += im->column )
				sum += C[k] * (*p);

			*tp = sum;
		}
	}
}





void
imagef_convolution2( image_type *im,
			float Cy[], float Cx[], int rC,
			image_type *cim )
{
image_type	*tim;

	tim = image_create( im->row, im->column, 4, 1, NULL );

	imagef_convolution_x( im, Cx, rC, tim );

	imagef_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );
}


void
imagef_convolution2_sub( image_type *im, image_type *im1,
			float Cy[], float Cx[], int rC,
			image_type *cim )
{
image_type	*tim;

	tim = image_create( im->row, im->column, 4, 1, NULL );

	imagef_convolution_x_sub( im, im1, Cx, rC, tim );

	imagef_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );
}


static void
imagef_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
u_char	*sp,	*p;
float	*tp;
int	i,	j,	k,	n;
float	sum;

	n = 2*rC+1;

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		tp = (float *)IMAGE_PIXEL( cim, i, rC );
		for( j = 0 ; j < im->column-n+1 ; j++, sp++, tp++ ){

			for( k = 0, sum = 0, p = sp; k < n ; k++, p++ )
				sum += C[k] * (*p);

			*tp = sum;
		}
	}
}



static void
imagef_convolution_x_sub( image_type *im, image_type *im1, float C[], int rC, image_type *cim )
{
u_char	*sp,	*sp1,	*p,	*p1;
float	*tp;
int	i,	j,	k,	n;
float	sum;

	n = 2*rC+1;

	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );
		sp1 = IMAGE_PIXEL( im1, i, 0 );
		tp = (float *)IMAGE_PIXEL( cim, i, rC );
		for( j = 0 ; j < im->column-n+1 ; j++, sp++, sp1 ++, tp++ ){

			for( k = 0, sum = 0, p = sp, p1 = sp1; k < n ; k++, p++, p1++ )
				sum += C[k] * ((int)(*p) - (int)(*p1));

			*tp = sum;
		}
	}
}



static void
imagef_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
float	*sp,	*tp,	*p;
int	i,	j,	k,	n;
float	sum;


	n = 2*rC+1;


//	for( j = 0 ; j < im->column ; j++ ){
	for( j = rC ; j < im->column - rC ; j++ ){

		sp = (float *)IMAGE_PIXEL( im, 0, j );
		tp = (float *)IMAGE_PIXEL( cim, rC, j );

		for( i = 0 ; i < im->row-n+1 ; i++, sp += im->column, tp += cim->column ){

			for( k = 0, sum = 0, p = sp; k < n  ; k++, p += im->column )
				sum += C[k] * (*p);

			*tp = sum;
		}
	}
}
