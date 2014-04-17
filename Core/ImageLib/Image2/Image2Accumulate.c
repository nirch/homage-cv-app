 /*****************************
 ***	ImageAccumulate.c   ***
 ******************************/

#include	"ImageType/ImageType.h"


float 
image2_accumulate( image_type *im0, int i0, int j0,
				  image_type *im1, int i1, int j1,
				  int width, int height )
{
short	*sp0,	*sp1;
int	i,	j;
float	sum;
int	algin0,	algin1;



	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			sum += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	sum /= 256;

	return( sum/( width * height) );
}


float 
image2_accumulateX( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height )
{
short	*sp0,	*sp1;
int	i,	j;
float	sum;
int	algin0,	algin1;

	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			sum += (j+0.5)*(*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	sum /= 256;

	return( sum/( width * height) );
}


float 
image2_accumulateY( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height )
{
short	*sp0,	*sp1;
int	i,	j;
float	sum,	sum1;
int	algin0,	algin1;

	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum1 = 0;
		for( j = 0 ; j < width ; j++ ){

			sum1 += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}

		sum += ( i+0.5)*sum1;
	}

	sum /= 256;

	return( sum/( width * height) );
}


float 
image2_accumulate_21(	image_type *im0, int i0, int j0,
						image_type *im1, int i1, int j1,
						int width, int height )
{
short	*sp0;
u_char	*sp1;
int	i,	j;
float	sum;
int	algin0,	algin1;

	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			sum += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	sum /= 16.0;
	return( sum/( width * height) );
}



void 
image2_accumulate_0_X_Y_XX_XY_YY( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height,
					double a[6] )
{
short	*sp0,	*sp1;
double	sum,	sumX,	sumXX,	tmp;
int		i,	j;
int	algin0,	algin1;


	a[0] = a[1] = a[2] = a[3] = a[4] = a[5] = 0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );

	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum = 0;
		sumX = 0;
		sumXX = 0;
		for( j = 0 ; j < width ; j++ ){

			tmp = (*sp1) * (*sp0);
			sum += tmp;
			sumX += j * tmp;
			sumXX += j*j*tmp;
			sp0++;
			sp1++;
		}

		a[0] += sum;
		a[1] += sumX + 0.5 * sum;
		a[2] += ( i+ 0.5)* sum;
		a[3] += sumXX + sumX + 0.25*sum;
		a[4] += ( i + 0.5 )*(sumX + 0.5 * sum);
		a[5] += ( i + 0.5 )*( i + 0.5 ) * sum;
	}

	tmp = 1.0 / ( 256*width *height );
	a[0] *= tmp;
	a[1] *= tmp;
	a[2] *= tmp;
	a[3] *= tmp;
	a[4] *= tmp;
	a[5] *= tmp;
}



void 
image2_accumulate_0_X_Y_21( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height,
					double a[3] )
{
short	*sp0;
u_char	*sp1;
double	sum,	sumX,	tmp;
int		i,	j;
int	algin0,	algin1;


	a[0] = a[1] = a[2] = 0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (u_char*)IMAGE_PIXEL( im1, i1, j1 );

	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum = 0;
		sumX = 0;
		for( j = 0 ; j < width ; j++ ){

			tmp = (*sp1) * (*sp0);
			sum += tmp;
			sumX += j * tmp;
			sp0++;
			sp1++;
		}

		a[0] += sum;
		a[1] += sumX + 0.5 * sum;
		a[2] += ( i+ 0.5)* sum;
	}

	tmp = 1.0 / ( 16*width *height );
	a[0] *= tmp;
	a[1] *= tmp;
	a[2] *= tmp;
}



void 
image2M_accumulate_0_X_Y_XX_XY_YY( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height, image_type *mim,
					double a[6] )
{
short	*sp0,	*sp1;
u_char	*mp;
double	sum,	sumX,	sumXX,	tmp;
int		i,	j,	no;
int	algin0,	algin1;


	a[0] = a[1] = a[2] = a[3] = a[4] = a[5] = 0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );

	mp = mim->data;

	for( i = 0, no = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum = 0;
		sumX = 0;
		sumXX = 0;
		for( j = 0 ; j < width ; j++, mp++ ){
			if( *mp == 0 )	continue;

			tmp = (*sp1) * (*sp0);
			sum += tmp;
			sumX += j * tmp;
			sumXX += j*j*tmp;
			sp0++;
			sp1++;

			no++;
		}

		a[0] += sum;
		a[1] += sumX + 0.5 * sum;
		a[2] += ( i+ 0.5)* sum;
		a[3] += sumXX + sumX + 0.25*sum;
		a[4] += ( i + 0.5 )*(sumX + 0.5 * sum);
		a[5] += ( i + 0.5 )*( i + 0.5 ) * sum;
	}

	tmp = 1.0 / ( 256*no );
	a[0] *= tmp;
	a[1] *= tmp;
	a[2] *= tmp;
	a[3] *= tmp;
	a[4] *= tmp;
	a[5] *= tmp;
}


void 
image2M_accumulate_0_X_Y_21( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height, image_type *mim,
					double a[3] )
{
short	*sp0;
u_char	*sp1,	*mp;
double	sum,	sumX,	tmp;
int		i,	j,	no;
int	algin0,	algin1;


	a[0] = a[1] = a[2] = 0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (u_char*)IMAGE_PIXEL( im1, i1, j1 );

	mp = mim->data;

	for( i = 0, no = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum = 0;
		sumX = 0;
		for( j = 0 ; j < width ; j++, mp++ ){
			if( *mp == 0 )	continue;

			tmp = (*sp1) * (*sp0);
			sum += tmp;
			sumX += j * tmp;
			sp0++;
			sp1++;
		}

		a[0] += sum;
		a[1] += sumX + 0.5 * sum;
		a[2] += ( i+ 0.5)* sum;
	}

	tmp = ( no > 0 )? 1.0 / ( 16*no ) : 0 ;
	a[0] *= tmp;
	a[1] *= tmp;
	a[2] *= tmp;
}




void 
image2_accumulate_0_X_Y_XX_XY_YY_w( image_type *im0, int i0, int j0,
								 image_type *im1, int i1, int j1,
								 image_type *wim,
								 int width, int height,
								 double a[6] )
{
	short	*sp0,	*sp1;
	float	*wp;
	double	sum,	sumX,	sumXX,	tmp;
	int		i,	j;
	int	algin0,	algin1;


	a[0] = a[1] = a[2] = a[3] = a[4] = a[5] = 0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );
	wp = wim->data_f;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum = 0;
		sumX = 0;
		sumXX = 0;
		for( j = 0 ; j < width ; j++, wp++ ){

			tmp = (*sp1) * (*sp0) * *wp;
			sum += tmp;
			sumX += j * tmp;
			sumXX += j*j*tmp;
			sp0++;
			sp1++;
		}

		a[0] += sum;
		a[1] += sumX + 0.5 * sum;
		a[2] += ( i+ 0.5)* sum;
		a[3] += sumXX + sumX + 0.25*sum;
		a[4] += ( i + 0.5 )*(sumX + 0.5 * sum);
		a[5] += ( i + 0.5 )*( i + 0.5 ) * sum;
	}

	tmp = 1.0 / ( 256*width *height );
	a[0] *= tmp;
	a[1] *= tmp;
	a[2] *= tmp;
	a[3] *= tmp;
	a[4] *= tmp;
	a[5] *= tmp;
}


void 
image2_accumulate_0_X_Y_21_w( image_type *im0, int i0, int j0,
						   image_type *im1, int i1, int j1,
						   image_type *wim,
						   int width, int height,
						   double a[3] )
{
	short	*sp0;
	u_char	*sp1;
	float	*wp;
	double	sum,	sumX,	tmp;
	int		i,	j;
	int	algin0,	algin1;


	a[0] = a[1] = a[2] = 0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (u_char*)IMAGE_PIXEL( im1, i1, j1 );

	wp = wim->data_f;

	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		sum = 0;
		sumX = 0;
		for( j = 0 ; j < width ; j++, wp++ ){

			tmp = (*sp1) * (*sp0) * *wp;
			sum += tmp;
			sumX += j * tmp;
			sp0++;
			sp1++;
		}

		a[0] += sum;
		a[1] += sumX + 0.5 * sum;
		a[2] += ( i+ 0.5)* sum;
	}

	tmp = 1.0 / ( 16*width *height );
	a[0] *= tmp;
	a[1] *= tmp;
	a[2] *= tmp;
}




float 
image2_accumulateA( image_type *im0, image_type *im1, box2i *b )
{
short	*sp0,	*sp1;
int	i,	j;
double	sum;
int	i0,	i1,	j0,	j1;

	imageA_box( im0, b, &j0, &i0, &j1, &i1 );


	sp0 = im0->data_s;
	sp1 = im1->data_s;


	sum = 0;
	for( i = i0 ; i < i1 ; i++ ){
		sp0 = (short*)IMAGE_PIXEL( im0, i, j0 );
		sp1 = (short*)IMAGE_PIXEL( im1, i, j0 );

		for( j = j0 ; j < j1 ; j++ ){

			sum += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	sum /= 256;

	return( sum/( im0->width * im0->height) );
}




float
image2_accumulate_wight( image_type *im1,
						image_type *im2,
						int i0, int j0,
						image_type *imw )
{
float	*w;
short	*tp1,	*tp2;
int	align,	alignw;
float	sum,	sw,	sum1;
int	width,	height,	i,	j;


	tp1 = (short*)IMAGE_PIXEL( im1, i0, j0 );
	tp2 = (short*)IMAGE_PIXEL( im2, i0, j0 );

	w = (float*)imw->data;


	width = im1->column - j0;
	if( width > imw->column )	width = imw->column;

	height = im1->row - i0;
	if( height > imw->row )	height = imw->row;


	align = im1->column - width;
	alignw = imw->column - width;

	sum = sw = 0;
	for( i = 0 ; i < height ; i++, tp1 += align, tp2 += align, w += alignw ){
		sum1 = 0;
		for( j = 0 ; j < width ; j++, w++, tp1++, tp2++ ){
			sum1 += (*w) * (*tp1) * ( *tp2);
			sw += *w;
		}

		sum += sum1 / 256;
	}

	sum /= sw;

	return( sum );
}


float
image2_accumulate_wight2( image_type *im1,
						image_type *im2,
						int i0, int j0,
						image_type *imw )
{
	float	*w;
	short	*tp1,	*tp2;
	int	align,	alignw;
	float	sum,	sw,	sum1;
	int	width,	height,	i,	j;


	tp1 = (short*)IMAGE_PIXEL( im1, i0, j0 );
	tp2 = (short*)IMAGE_PIXEL( im2, i0, j0 );

	w = (float*)imw->data;


	width = im1->column - j0;
	if( width > imw->column )	width = imw->column;

	height = im1->row - i0;
	if( height > imw->row )	height = imw->row;


	align = im1->column - width;
	alignw = imw->column - width;

	sum = sw = 0;
	for( i = 0 ; i < height ; i++, tp1 += align, tp2 += align, w += alignw ){
		sum1 = 0;
		for( j = 0 ; j < width ; j++, w++, tp1++, tp2++ ){
			sum1 += (*w) * (*w)* (*tp1) * ( *tp2);
			sw += *w;
		}

		sum += sum1 / 256;
	}

	sum /= sw;

	return( sum );
}


float
image2_accumulate_wight_21( image_type *im1, image_type *im2, int i0, int j0, image_type *imw )
{
	float	*w;
	short	*tp1;
	u_char	*tp2;
	int	align,	alignw;
	float	sum,	sw,	sum1;
	int	width,	height,	i,	j;

	tp1 = (short*)IMAGE_PIXEL( im1, i0, j0 );
	tp2 = IMAGE_PIXEL( im2, i0, j0 );

	w = (float*)imw->data;


	width = im1->column - j0;
	if( width > imw->column )	width = imw->column;

	height = im1->row - i0;
	if( height > imw->row )	height = imw->row;


	align = im1->column - width;
	alignw = imw->column - width;

	sum = sw = 0;
	for( i = 0 ; i < height ; i++, tp1 += align, tp2 += align, w += alignw ){
		sum1 = 0;
		for( j = 0 ; j < width ; j++, w++, tp1++, tp2++ ){
			sum1 += (*w) * (*tp1) * ( *tp2);
			sw += *w;
		}

		sum += sum1 /16;
	}

	sum /= sw;

	return( sum );
}



float
image2_accumulate_wight_21_measure( image_type *imDx, image_type *imDy,
									image_type *im, image_type *imt,
									float u, float v,
									int i0, int j0, image_type *imw )
{
float	*w;
short	*spDx,	*spDy;
u_char	*sp,	*spt;
int	align,	alignw;
float	sum,	sw,	sum1,	t;
int	width,	height,	i,	j;


	spDx = (short*)IMAGE_PIXEL( imDx, i0, j0 );
	spDy = (short*)IMAGE_PIXEL( imDy, i0, j0 );


	sp = IMAGE_PIXEL( im, i0, j0 );
	spt = IMAGE_PIXEL( imt, i0, j0 );

	w = (float*)imw->data;


	width = im->column - j0;
	if( width > imw->column )	width = imw->column;

	height = im->row - i0;
	if( height > imw->row )	height = imw->row;


	align = im->column - width;
	alignw = imw->column - width;

	u /= 16;
	v /= 16;

	sum = sw = 0;
	for( i = 0 ; i < height ; i++, spDx += align, spDy += align, sp += align, spt += align, w += alignw ){
		sum1 = 0;
		for( j = 0 ; j < width ; j++, w++, spDx++, spDy++, sp++, spt++ ){
			t = ( u* *spDx + v* *spDy + *sp - *spt);
			sum1 += *w *t*t ;
			sw += *w;
		}

		sum += sum1;
	}

	sum /= sw;

	return( sum );
}