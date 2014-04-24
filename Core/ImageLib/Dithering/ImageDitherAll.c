#include <stdlib.h>
#include <time.h>

#include "ImageType/ImageType.h"

#include "ImageDithering.h"


image_type * 
image_dithering_error_floyd_color( image_type *im, image_type *new_im,
									   int bpp);

typedef image_type * (*image_dithering)
				( image_type *im, image_type *new_im );

image_dithering	Image_dithering[4][5] = {
	{NULL, NULL, NULL, NULL, NULL },
	{image_dithering_random,
		image_dithering_pattern1,
		image_dithering_pattern2,
		NULL, NULL},
	{image_dithering_ordered_2x2,
		image_dithering_ordered_3x3_1,
		image_dithering_ordered_3x3_2,
		image_dithering_ordered_4x4,
		NULL},
	{image_dithering_error_simpler,
		image_dithering_error_floyd,
		image_dithering_error_burke,
		image_dithering_error_jarvis,
		image_dithering_error_stucki } };


image_type * 
image_dithering_all( image_type *im, image_type *new_im )
{
image_type	*dim;
int			row, col;
int			i, j;
const int	w_sp = 30;
const int	h_sp = 10;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	dim = image_create( row, col, 4, 1, NULL );

	if ( new_im == NULL )
		new_im = image_create( (row+h_sp)*4, (col+w_sp)*5, 4, 1, NULL );

	image4_const( new_im, 0x00FFFFFF );

	image_copy_in( im, w_sp/2, h_sp/2, new_im );

	for ( i = 0; i < 4; i++ )
	for ( j = 0; j < 5; j++ )
	{
		image4_const( dim, 0x00FFFFFF );
		if ( Image_dithering[i][j] )
		{
			dim = (*Image_dithering[i][j])( im, dim );
			image_copy_in( dim, w_sp/2+(dim->column+w_sp)*j, h_sp/2+(dim->row+h_sp)*i, new_im );
		}
	}

	image_destroy( dim, 1 );


	return( new_im );	
}

image_type * 
image_dithering_floyd_color_bpp( image_type *im, image_type *new_im )
{
image_type	*dim;
int			row, col;
int			i, j;
const int	w_sp = 30;
const int	h_sp = 10;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	dim = image_create( row, col, 4, 1, NULL );

	if ( new_im == NULL )
		new_im = image_create( (row+h_sp)*2, (col+w_sp)*5, 4, 1, NULL );

	image4_const( new_im, 0x00FFFFFF );

	image_copy_in( im, w_sp/2, h_sp/2, new_im );

	i = 1;
	for ( j = 0; j < 5; j++ )
	{
		image4_const( dim, 0x00FFFFFF );
		dim = image_dithering_error_floyd_color( im, dim, j+1 );
		image_copy_in( dim, w_sp/2+(dim->column+w_sp)*j, h_sp/2+(dim->row+h_sp)*i, new_im );
	}

	image_destroy( dim, 1 );
	
	return( new_im );	
}

/////////////////////////////////////////////////
//for differrent  image_type *im ( from differrent divorse )
image_type * 
image_dithering_all_err( image_type *im, image_type *new_im, int *h )
{
image_type	*dim;
int			row, col;
int			j;
const int	w_sp = 30;
const int	h_sp = 10;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if ( new_im == NULL )
	{
		new_im = image_create( (row+h_sp)*7, (col+w_sp)*6, 4, 1, NULL );
		image4_const( new_im, 0x00FFFFFF );
	}

	image_copy_in( im, w_sp/2, h_sp/2+(*h), new_im );

	dim = image_create( row, col, 4, 1, NULL );

	for ( j = 0; j < 5; j++ )
	{
		image4_const( dim, 0x00FFFFFF );
		if ( Image_dithering[3][j] )
		{
			dim = (*Image_dithering[3][j])( im, dim );
			image_copy_in( dim, w_sp/2+(dim->column+w_sp)*(j+1), h_sp/2+(*h), new_im );
		}
	}

	(*h) += (dim->row+h_sp);

	image_destroy( dim, 1 );

	return( new_im );	
}

/*
////////// ImageDitherErrExp.c //////////////////

image_type *	image_dithering_error_simpler_exp( image_type *im, image_type *new_im );

image_type *	image_dithering_error_floyd_exp( image_type *im, image_type *new_im );

image_type *	image_dithering_error_burke_exp( image_type *im, image_type *new_im );

image_type *	image_dithering_error_jarvis_exp( image_type *im, image_type *new_im );

image_type *	image_dithering_error_stucki_exp( image_type *im, image_type *new_im );

image_type *	image_dithering_error_simpler_exp1( image_type *im, image_type *new_im );

image_type *	image_dithering_error_floyd_exp1( image_type *im, image_type *new_im );

image_type *	image_dithering_error_burke_exp1( image_type *im, image_type *new_im );

image_type *	image_dithering_error_jarvis_exp1( image_type *im, image_type *new_im );

image_type *	image_dithering_error_stucki_exp1( image_type *im, image_type *new_im );

image_type *	image_dithering_error_simpler_exp2( image_type *im, image_type *new_im );

image_type *	image_dithering_error_floyd_exp2( image_type *im, image_type *new_im );

image_type *	image_dithering_error_burke_exp2( image_type *im, image_type *new_im );

image_type *	image_dithering_error_jarvis_exp2( image_type *im, image_type *new_im );

image_type *	image_dithering_error_stucki_exp2( image_type *im, image_type *new_im );

image_type *	image_dithering_error_floyd_color_exp( image_type *im, image_type *new_im,
									   int bpp);


image_dithering	Image_dithering_exp[4][5] = {
	{NULL, NULL,
		image_dithering_random,
		image_dithering_pattern1,
		image_dithering_pattern2 },
	{NULL,
		image_dithering_ordered_2x2,
		image_dithering_ordered_3x3_1,
		image_dithering_ordered_3x3_2,
		image_dithering_ordered_4x4 },
	{image_dithering_error_simpler,
		image_dithering_error_floyd,
		image_dithering_error_burke,
		image_dithering_error_jarvis,
		image_dithering_error_stucki },
	{image_dithering_error_simpler_exp2,
		image_dithering_error_floyd_exp2,
		image_dithering_error_burke_exp2,
		NULL, NULL } };


image_type * 
image_dithering_all_exp( image_type *im, image_type *new_im )
{
image_type	*dim;
int			row, col;
int			i, j;
const int	w_sp = 30;
const int	h_sp = 10;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	dim = image_create( row, col, 4, 1, NULL );

	if ( new_im == NULL )
		new_im = image_create( (row+h_sp)*6, (col+w_sp)*5, 4, 1, NULL );

	image4_const( new_im, 0x00FFFFFF );

	image4_copy_in( im, new_im, h_sp/2, w_sp/2 );

	for ( i = 0; i < 4; i++ )
	for ( j = 0; j < 5; j++ )
	{
		image4_const( dim, 0x00FFFFFF );
		if ( Image_dithering_exp[i][j] )
		{
			dim = (*Image_dithering_exp[i][j])( im, dim );
			image4_copy_in( dim, new_im, h_sp/2+(dim->row+h_sp)*i,
		                         w_sp/2+(dim->column+w_sp)*j );
		}
	}

	i = 4;
	for ( j = 0; j < 5; j++ )
	{
		image4_const( dim, 0x00FFFFFF );
		dim = image_dithering_error_floyd_color( im, dim, j+1 );
		image4_copy_in( dim, new_im, h_sp/2+(dim->row+h_sp)*i,
		                         w_sp/2+(dim->column+w_sp)*j );
	}

	i = 5;
	for ( j = 0; j < 5; j++ )
	{
		image4_const( dim, 0x00FFFFFF );
		dim = image_dithering_error_floyd_color_exp( im, dim, j+1 );
		image4_copy_in( dim, new_im, h_sp/2+(dim->row+h_sp)*i,
		                         w_sp/2+(dim->column+w_sp)*j );
	}

	image_destroy( dim, 1 );


	return( new_im );	
}

*/