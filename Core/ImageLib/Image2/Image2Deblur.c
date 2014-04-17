
#include	<math.h>

#include	"ImageType/ImageType.h"

#define	RC	32



static void	image2_deblur_bilateral_step( image_type *sim, image_type *im,
											float weight, float sigma_x, float lamda );

static void	image2_deblur_step( image_type *sim, image_type *im, float beta );






image_type *
image2_deblur( image_type *imY, float sigma,
					float beta,
					float blSigma_x, float blLamda, int step )
{
image_type	*im,	*im2,	*im3,	*im4;
float	g[RC*2+1];
float	w;
int	i,	rC;
	

	rC = (int)(3*sigma);
	if( rC < 1 )	rC = 1;
	guasian( rC, sigma, g );


	
	im2 = im3 = im4 = NULL;

	imY = image_make_copy( imY, NULL );
	im = image_make_copy( imY, NULL );

	for( i = 0; i < step ; i++ ){

		im2 = image2_convolution_separably( imY, g, g, rC, im2 );

		im3 = image2_subtruct( im2, im, im3 );

		im4 = image2_convolution_separably( im3, g, g, rC, im4 );


		if( step > 1 )
			w = beta - (beta -0.1)/(step-1)*i;
		else	w = beta;

		if( blSigma_x != 0 && blLamda != 0 )
			image2_deblur_bilateral_step( imY, im4, w, blSigma_x, blLamda );
		else
			image2_deblur_step( imY, im4, w );

	}


	image_destroy( im, 1 );
	image_destroy( im2, 1 );
	image_destroy( im3, 1 );
	image_destroy( im4, 1 );

	return( imY );
}


static void
image2_deblur_bilateral_step( image_type *sim, image_type *im, float beta, float sigma_x, float lamda )
{
image_type	*cim;
float	w[5];
int	wMat[50],	r,	*fp;
int	i,	j,	n,	m;
int	tmp;
short	*sp,	*sp0,	*tp,	*cp,	t0;

int	align,	align0;
int	k;

	guasian( 2, sigma_x, w );
	fp = &wMat[0];
	for( i = 0 ; i < 5 ; i++ )
		for( j = 0 ; j < 5 ; j++, fp += 2 ){

			if( i != 2 || j != 2 )
				*fp = lamda *w[i] * w[j] * 256;
			else	*fp = 0;
			*(fp+1) = - *fp;

		}




	cim = image_make_copy( sim, NULL );

	align = im->column - 5;
	align0 = 2*im->column + 2;

	for( i = 2; i < im->row-2 ; i++ ) {
		sp = (short*)IMAGE_PIXEL( im, i, 2 );
		cp = (short*)IMAGE_PIXEL( cim, i, 2 );
		tp = (short*)IMAGE_PIXEL( sim, i, 2 );
		for( j = 2; j < im->column-2 ; j++ ){


			fp = &wMat[0];
			sp0 = cp - align0;

			t0 = *cp;
			for( m = 0, r = 0 ; m < 5 ; m++, sp0 += align ){
				for( n = 0 ; n < 5 ; n++, sp0++, fp += 2 ){

					tmp = t0 - *(sp0);

//					if( tmp < 0 )	r -= *fp;
//					else	if( tmp > 0 )
//								r += *fp;


					k = ( tmp >> 31 ) &0x01;
					r += *(fp+k);

				}
			}
 
			r >>= 4;



			tmp = *sp++;
			tmp =  *tp -  ( tmp + r) *beta;
			*tp++ = tmp;

			cp++;
		}
	}


	image_destroy( cim, 1 );
}




/*
static void
image2_deblur_bilateral_step( image_type *sim, image_type *im, float beta, float sigma_x, float lamda )
{
image_type	*cim;
float	w[5];
float	wMat[5][5],	r,	*fp;
int	i,	j,	n,	m;
int	tmp;
short	*sp,	*sp0,	*tp,	*cp,	t0;

int	align,	align0;


	guasian( 2, sigma_x, w );
	for( i = 0 ; i < 5 ; i++ )
		for( j = 0 ; j < 5 ; j++ ){
			wMat[i][j] = lamda *w[i] * w[j] * 16;
		}



	cim = image_make_copy( sim, NULL );

	align = im->column - 5;
	align0 = 2*im->column + 2;

	for( i = 2; i < im->row-2 ; i++ ) {
		sp = (short*)IMAGE_PIXEL( im, i, 2 );
		cp = (short*)IMAGE_PIXEL( cim, i, 2 );
		tp = (short*)IMAGE_PIXEL( sim, i, 2 );
		for( j = 2; j < im->column-2 ; j++ ){


			fp = &wMat[0][0];
//			sp0 = sp - align0;
			sp0 = cp - align0;

			t0 = *cp;
			for( m = 0, r = 0 ; m < 5 ; m++, sp0 += align ){
				for( n = 0 ; n < 5 ; n++, sp0++, fp++ ){

					tmp = t0 - *(sp0);

					if( tmp < 0 )	r -= *fp;
					else	if( tmp > 0 )	r += *fp;


				}
			}



			tmp = *sp++;
			tmp =  *tp -  ( tmp -2048 + r) *beta;
			*tp++ = tmp;

			cp++;
		}
	}


	image_destroy( cim, 1 );
}
*/


static void
image2_deblur_step( image_type *sim, image_type *im, float beta )
{

int	i,	j;
int	tmp;
short	*sp,	*tp;



	tp = (short*)sim->data;
	sp = (short*)im->data;

	for( i = 0; i < im->row ; i++ ) {
		for( j = 0; j < im->column; j++ ){

			tmp = *sp++;
			tmp =  *tp -  ( tmp) *beta;
			*tp++ = tmp;
		}
	}
}






/*
for every pixel X[i][j] do:
 
    bilateral_term = 0
    for m=-P to P
        for n=-P to P
            t1 = sign(X[i][j] - X[i-m][j-n])
            t2 = sign(X[i][j] - X[i+m][j+n])
            f = alpha^(abs(m)+abs(n)
            bilateral_term += f*(t1 + t2)
        end
    end
*/

/*
static void	image6_bitral( image_type *sim, image_type *im, float beta, float alpha, float lamda );


void
image6_deblur_bitral( image_type *sim, int rC, float sigma,
					 float alpha, float beta, float lamda, int step )
{
image_type	*im,	*im2,	*im3,	*im4;
float	g[RC*2+1];
float	w;
int	i;
	
	guasian( rC, sigma, g );


	im2 = im3 = im4 = NULL;

	im = image_make_copy( sim, NULL );

	for( i = 0; i < step ; i++ ){

		im2 = image6_convolution_separably( sim, g, g, rC, im2 );

		im3 = image6_subtruct( im2, im, im3 );

		im4 = image6_convolution_separably( im3, g, g, rC, im4 );

//		w = b;
//		image6_aco( sim, im4, w, -128 );- ( b -0.1)
		w = beta - (beta -0.1)/(step-1)*i;
		image6_bitral( sim, im4, w, alpha, lamda );

	}

	image_destroy( im, 1 );
	image_destroy( im2, 1 );
	image_destroy( im3, 1 );
	image_destroy( im4, 1 );
}


static void
image6_bitral( image_type *sim, image_type *im, float beta, float alpha, float lamda )
{
float wMat[5][5],	w[5];
int	i,	j,	l,	k,	n,	m;
int	tmp;
short	*sp,	*sp0,	*tp;
float	r,	g,	b,	f;

	w[0] = w[4] = alpha*alpha;
	w[1] = w[3] = alpha;
	w[2] = 1;

	for( i = 0 ; i < 5 ; i++ )
		for( j = 0 ; j < 5 ; j++ ){
			wMat[i][j] = w[i] * w[j];
		}




	tp = (short*)sim->data;
	sp = (short*)im->data;

	for( i = 0; i < im->row ; i++ ) {
		for( j = 0; j < im->column; j++ ){//, tp += 3, sp += 3){

			r = g = b = 0;


			for( k = -2, m = 0 ; m < 5 ; k++, m++ ){
				if( i+k < 0 || i+k >= im->row )	continue;
				sp0 = (short*)IMAGE_PIXEL( im, i+k, j-2 );
				for( l = -2, n = 0 ; n < 5 ; l++, n++, sp0 += 3 ){
					if( j+l < 0 || j+l >= im->column )	continue;

					f = wMat[m][n];


					tmp = (int)*(sp) - (int)*(sp0);
					if( tmp < 0 )	r -= f;
					else	if( tmp > 0 )	r += f;

					tmp = (int)*(sp+1) - (int)*(sp0+1);
					if( tmp < 0 )	g -= f;
					else	if( tmp > 0 )	g += f;


					tmp = (int)*(sp+2) - (int)*(sp0+2);
					if( tmp < 0 )	b -= f;
					else	if( tmp > 0 )	b += f;
				}
			}



			tmp = *sp++;
			tmp =  *tp -  ( tmp -2048 + lamda*r) *beta;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4096 );

			tmp = *sp++;
			tmp =  *tp -  ( tmp -2048 + lamda*g ) *beta;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4096 );;


			tmp = *sp++;
			tmp =  *tp -  ( tmp -2048 + lamda*b ) *beta;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4096 );;
		}
	}
}

*/
