
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"ImageFTool.h"



static void	imageF_convolution_y( image_type *im, float C[], int rC, image_type *cim );

static void	imageF_convolution_x( image_type *im, float C[], int rC, image_type *cim );

image_type *
imageF_convolution_guasian( image_type *sim,
						   float sigma,
						   image_type *cim )
{
	int	rC;
	float	g[MAX_RC];

	rC = (int)(3*sigma);
	if( rC < 1 )	rC = 1;
	guasian( rC, sigma, g );


	cim = imageF_convolution_separably( sim, g, g, rC, cim );

	return( cim );
}



image_type *
imageF_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim )
{
image_type	*tim;


	cim = image_recreate( cim, im->row, im->column, IMAGE_FORMAT_F, 1 );


	tim = image_create( im->row, im->column, 4, 1, NULL );

	imageF_convolution_x( im, Cx, rC, tim );

	imageF_convolution_y( tim, Cy, rC, cim );

	image_destroy( tim, 1 );

	return( cim );
}


static void
imageF_convolution_x( image_type *im, float C[], int rC, image_type *cim )
{
float	*sp,	*tp,	*p;
int	i,	j,	k,	n;
float	r;


	n = 2*rC+1;


	for( i = 0 ; i < im->row ; i++ ){
		sp = (float *)IMAGE_PIXEL( im, i, 0 );
		tp = (float *)IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ )
			*tp++ = *p++;


		for( j = 0 ; j < im->column-n+1 ; j++, sp += 1 ){

			for( k = 0, r = 0, p = sp; k < n ; k++ ){
				r += C[k] * (*p++);
			}



			*tp++ = r;
		}


		sp += rC;
		for( j = 0 ; j < rC ; j++ )
			*tp++ = *sp++;

	}
}



static void
imageF_convolution_y( image_type *im, float C[], int rC, image_type *cim )
{
float	*sp,	*tp,	*p;
int	i,	j,	k,	n;
float	r;
int	align;



	n = 2*rC+1;



	align = im->column;


	sp = im->data_f;  // IMAGE_PIXEL( im, 0, 0 );
	tp = cim->data_f;	// IMAGE_PIXEL( cim, 0, 0 );
	for( i = 0 ; i < rC ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}

	sp = im->data_f;
	for(  ; i < im->row - rC ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 1 ){
			for( k = 0, r = 0, p = sp; k < n  ; k++, p += align ){
				r += C[k] * (*p);
			}


			*tp++ = r;
		}
	}


	sp = (float *)IMAGE_PIXEL( im, i, 0 );
	for(  ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}
}


image_type *	imageFM_convolution_separably( image_type *im, image_type *mim,
							  float Cy[], float Cx[], int rC,
							  image_type *cim );

static void	imageFM_convolution_x( image_type *im, image_type *mim, float C[], int rC, image_type *cim );
static void	imageFM_convolution_y( image_type *im, image_type *mim, float C[], int rC, image_type *cim );


image_type *
imageFM_convolution_guasian( image_type *sim,
							image_type *mim,
						   float sigma,
						   image_type *cim )
{
	int	rC;
	float	g[MAX_RC];

	rC = (int)(3*sigma);
	if( rC < 1 )	rC = 1;
	guasian( rC, sigma, g );


	cim = imageFM_convolution_separably( sim, mim, g, g, rC, cim );

	return( cim );
}


image_type *
imageFM_convolution_separably( image_type *im, image_type *mim,
							 float Cy[], float Cx[], int rC,
							 image_type *cim )
{
	image_type	*tim;


	cim = image_recreate( cim, im->row, im->column, IMAGE_FORMAT_F, 1 );


	tim = image_create( im->row, im->column, 4, 1, NULL );

	imageFM_convolution_x( im, mim, Cx, rC, tim );

	imageFM_convolution_y( tim, mim, Cy, rC, cim );

	image_destroy( tim, 1 );

	return( cim );
}


static void
imageFM_convolution_x( image_type *im, image_type *mim, float C[], int rC, image_type *cim )
{
float	*sp,	*tp,	*p;
u_char	*mp, *mp1;
int	i,	j,	k,	n,	no;
float	r,	w;


	no = 2*rC+1;


	for( i = 0 ; i < im->row ; i++ ){
		sp = (float *)IMAGE_PIXEL( im, i, 0 );
		mp = IMAGE_PIXEL( mim, i, 0 );
		tp = (float *)IMAGE_PIXEL( cim, i, 0 );


		for( j = 0, p = sp ; j < rC ; j++ )
			*tp++ = *p++;


		for( j = 0 ; j < im->column-no+1 ; j++, sp += 1, mp += 1 ){

			for( k = 0, w = 0, n = 0, r = 0, p = sp, mp1 = mp ; k < no ; k++,  p++ ){
				if( (*mp1++) != 0 ){
					r += C[k] * (*p);
					w += C[k];
					n++;
				}
			}

			if( n < no && w > 0  )
				r /= w;


			*tp++ = r;
		}


		sp += rC;
		for( j = 0 ; j < rC ; j++ )
			*tp++ = *sp++;

	}
}



static void
imageFM_convolution_y( image_type *im, image_type *mim, float C[], int rC, image_type *cim )
{
float	*sp,	*tp,	*p;
u_char	*mp,	*mp1;
int	i,	j,	k,	n,	no;
float	w;
float	r;
int	align;



	no = 2*rC+1;



	align = im->column;


	sp = im->data_f;  // IMAGE_PIXEL( im, 0, 0 );
	tp = cim->data_f;	// IMAGE_PIXEL( cim, 0, 0 );

	for( i = 0 ; i < rC ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}

	sp = im->data_f;
	mp = mim->data;
	for(  ; i < im->row - rC ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 1, mp++ ){
			for( k = 0, r = 0, p = sp, mp1 = mp, w  = 0, n = 0; k < no  ; k++, p += align, mp1 += align ){
				if( *mp1 != 0 ){
					r += C[k] * (*p);
					w += C[k];
					n++;
				}
			}

			if( n < no && w > 0  )
				r /= w;


			*tp++ = r;
		}
	}


	sp = (float *)IMAGE_PIXEL( im, i, 0 );
	for(  ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}
}