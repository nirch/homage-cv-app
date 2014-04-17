 /**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"

float
image1_ssd(	image_type *im0, int i0, int j0,
						image_type *im1, int i1, int j1,
						int width, int height )
{
u_char	*sp0;
u_char	*sp1;
int	i,	j;
int	sum,	tmp;
int	algin0,	algin1;

	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = IMAGE_PIXEL( im0, i0, j0 );
	sp1 = IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			tmp = (int)(*sp1)  - (int) (*sp0);
			sum += tmp*tmp;

			sp0++;
			sp1++;
		}
	}

	return( sum/( (float)(width * height)) );
}

void 
image1_ssd_av(	image_type *im0, int i0, int j0,
			image_type *im1, int i1, int j1,
			int width, int height, float *av, float *ssd )
{
u_char	*sp0;
u_char	*sp1;
int	i,	j;
int	sum,	sum1,	tmp;
int	algin0,	algin1;

	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = IMAGE_PIXEL( im0, i0, j0 );
	sp1 = IMAGE_PIXEL( im1, i1, j1 );


	sum1 = sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			tmp = (int)(*sp1)  - (int) (*sp0);
			sum1 += tmp;
			sum += tmp*tmp;

			sp0++;
			sp1++;
		}
	}

	*av = sum1 / ( (float)(width * height));
	*ssd = sum / ( (float)(width * height));

	*ssd -= *av * *av;
}

void
image1_ssd0( image_type *sim1, image_type *sim0,
			int x0, int y0, int width, int height, float *av, float *ssd0 )
{
	u_char	*sp0,	*sp1;
	int	i,	j;
	float	sum,	sAv,	tmp;



	if( y0 + height >= sim0->row )
		height = sim0->row - y0;

	if( x0 + width > sim0->column )
		width = sim0->column - x0;


	sum = sAv = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			tmp = (int)(*sp1)  - (int) (*sp0);

			sAv += tmp;

			if( tmp < 0 )	sum -= tmp;
			else	sum += tmp;


			sp0++;
			sp1++;
		}
	}


	*av = sAv / ( width * height );
	*ssd0 = sum/( width * height);
}


void
image1M_ssd0( image_type *sim1, image_type *sim0, image_type *mim,
			int x0, int y0, int width, int height, float *av, float *ssd0 )
{
	u_char	*sp0,	*sp1,	*mp;
	int	i,	j,	n;
	float	sum,	sAv,	tmp;



	if( y0 + height >= sim0->row )
		height = sim0->row - y0;

	if( x0 + width > sim0->column )
		width = sim0->column - x0;


	sum = sAv = 0;
	for( i = 0, n = 0 ; i < height ; i++ ){
		sp0 = IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = IMAGE_PIXEL( sim1, i+y0, x0 );
		mp = IMAGE_PIXEL( mim, i+y0, x0 );

		for( j = 0 ; j < width ; j++, mp++ ){
			if( *mp == 0 )	continue;

			tmp = (int)(*sp1)  - (int) (*sp0);

			sAv += tmp;

			if( tmp < 0 )	sum -= tmp;
			else	sum += tmp;
			
			n++;

			sp0++;
			sp1++;
		}
	}


	*av = sAv / n ;
	*ssd0 = sum / n;
}

