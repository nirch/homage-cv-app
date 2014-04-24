#include <memory.h>


#include "ImageType/ImageType.h"
#include "ImageDithering.h"


#define COLOR_RANGE_0_256(c) (((c) > 0xFF) ? 0xFF : \
									( ((c) < 0) ? 0 : (c) ))

static image_type * 
image4_dithering_floyd(image_type * im, image_type *new_im );

static image_type * 
image1_dithering_floyd(image_type * im, image_type *new_im );

/*
			X	7
		3	5	1
*/


image_type * 
image_dithering_floyd(image_type * im, image_type *new_im )
{
	if ( im->depth == 4 )
		return image4_dithering_floyd( im, new_im );
	else
		return image1_dithering_floyd( im, new_im );

}


static image_type * 
image4_dithering_floyd(image_type * im, image_type *new_im )
{
int row, col, column2;
u_char	*tp;
u_int   *sp;
int	i,	j;

int y_sum;
int y_set;
int *y_er[2];
int *y_per[2];
int y_error;
u_int   Y, R, G, B;
char order;


	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 1, 1, NULL );
	memset( new_im->data, 0, row*col );

	tp = new_im->data;

	column2 = col+2;

	y_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( y_er[0], 0, column2*sizeof(int) );
	y_er[1] = y_er[0] + column2;
	
	order = 0;

	for( i = 0; i < row; i++, order = !order){

		y_per[0] = y_er[order]+1;
		y_per[1] = y_er[!order]+1;
		memset( y_per[1], 0, col*sizeof(int) ); 

		sp = IMAGE4_PIXEL(im, i, 0 );

		for (j = 0; j < col; j++){

			if ( ABS(*(y_per[0])) > 0x800 ) *(y_per[0]) = *(y_per[0])/2;

			R = IMAGE4_RED(*sp);
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
			sp++;

			y_sum = Y   + ( (*(y_per[0])) >>4 );

			if ( y_sum < 0x80 )
				y_set =  0;
			else
				y_set =  0xFF;

			*tp++ = (u_char)y_set;
			
			y_error = y_sum - y_set;  //r_real

			*(y_per[0] + 1) += y_error*7;
			*(y_per[1])		+= y_error*5;
			*(y_per[1] + 1) += y_error;
			*(y_per[1] - 1) += y_error*3;

			y_per[0]++;
			y_per[1]++;
		}
	}


	free( y_er[0] );

	return( new_im );
}


static image_type * 
image1_dithering_floyd(image_type * im, image_type *new_im )
{
int row, col, column2;
u_char	*tp;
u_char	*sp;
int	i,	j;

int y_sum;
int y_set;
int *y_er[2];
int *y_per[2];
int y_error;
char order;


	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 1, 1, NULL );
	memset( new_im->data, 0, row*col );

	tp = new_im->data;

	column2 = col+2;

	y_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( y_er[0], 0, column2*sizeof(int) );
	y_er[1] = y_er[0] + column2;
	
	order = 0;

	for( i = 0; i < row; i++, order = !order){

		y_per[0] = y_er[order]+1;
		y_per[1] = y_er[!order]+1;
		memset( y_per[1], 0, col*sizeof(int) ); 

		sp = IMAGE_PIXEL(im, i, 0 );

		for (j = 0; j < col; j++){

			if ( ABS(*(y_per[0])) > 0x800 ) *(y_per[0]) = *(y_per[0])/2;

			y_sum = ( *sp )   + ( (*(y_per[0])) >>4 );
			sp++;

			if ( y_sum < 0x80 )
				y_set =  0;
			else
				y_set =  0xFF;

			*tp++ = (u_char)y_set;
			
			y_error = y_sum - y_set;  //r_real

			*(y_per[0] + 1) += y_error*7;
			*(y_per[1])		+= y_error*5;
			*(y_per[1] + 1) += y_error;
			*(y_per[1] - 1) += y_error*3;

			y_per[0]++;
			y_per[1]++;
		}
	}

	free( y_er[0] );
	return( new_im );
}


