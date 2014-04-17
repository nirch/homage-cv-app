/**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"




float 
image1_accumulate( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
u_char	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}


float 
imagef_accumulate( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
float	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = (float*)IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = (float*)IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}



float 
imagef_accumulateX( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
float	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = (float*)IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = (float*)IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += j*(*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}


float 
imagef_accumulateY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
float	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = (float*)IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = (float*)IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += i*(*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}



float 
imagef_accumulateXX( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
float	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = (float*)IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = (float*)IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += j*j*(*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}


float 
imagef_accumulateXY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
float	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = (float*)IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = (float*)IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += i*j*(*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}


float 
imagef_accumulateYY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
float	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = (float*)IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = (float*)IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += i*i*(*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}