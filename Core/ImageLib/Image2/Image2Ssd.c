 /**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"



float
image2_ssd(	image_type *im0, int i0, int j0,
						image_type *im1, int i1, int j1,
						int width, int height )
{
short	*sp0;
short	*sp1;
int	i,	j;
int	tmp;
int	algin0,	algin1;
double	sum;

	if( width <= 0 )	width = im0->width - j0;
	if( height <= 0 )	height = im0->height - j0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			tmp = (*sp1)  - (*sp0);
			sum += tmp*tmp;

			sp0++;
			sp1++;
		}
	}

	return( sum/( (float)(width * height)) );
}


float
image2_ssd0( image_type *im0, int i0, int j0,
		   image_type *im1, int i1, int j1,
		   int width, int height )
{
short	*sp0;
short	*sp1;
int	i,	j;
int	tmp;
int	algin0,	algin1;
double	sum;


	if( width <= 0 )	width = im0->width - j0;
	if( height <= 0 )	height = im0->height - j0;


	algin0 = im0->column - width;
	algin1 = im1->column - width;

	sp0 = (short*)IMAGE_PIXEL( im0, i0, j0 );
	sp1 = (short*)IMAGE_PIXEL( im1, i1, j1 );


	sum = 0;
	for( i = 0 ; i < height ; i++, sp0 += algin0, sp1 += algin1 ){

		for( j = 0 ; j < width ; j++ ){

			tmp = (*sp1)  - (*sp0);

			if( tmp < 0 )	sum -= tmp;
			else	sum += tmp;
			sp0++;
			sp1++;
		}
	}

	return( sum/( (float)(width * height)) );
}


