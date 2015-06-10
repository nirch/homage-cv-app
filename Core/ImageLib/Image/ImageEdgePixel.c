/******************************
 ***    ImageConvolution.c  ***
 *****************************/
 
#include "ImageType/ImageType.h"
#include        "Umat/Mat2fType.h"
 
 
 
image_type *
image_edge_pixel( image_type *sim, mat2f_type *m, float T )
{
image_type	*im;
u_char  *p;
int     row,    col,    i,      j,	d;
float	g;
 

	if( sim->depth != 1 )	return( NULL );


	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );
 
	d = m->ni / 2;
 

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			if( i-d < 0 || i + d > row || j-d < 0 || j + d > col ){
					*p++ = 0;
					continue;
				}
 
			g = image_mat2f_inner( sim, i-d, j-d, m );

			*p = ( ABS(g) < T )? 0 : 255;

			p++;

		}

	return( im );
}



image_type *
image_edge_gradient( image_type *sim, mat2f_type *mx, mat2f_type *my, float T )
{
image_type	*im;
u_char  *p;
int     row,    col,    i,      j;
float	dx,	dy,	g;
int	d;
 

	if( sim->depth != 1 )	return( NULL );


	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );
 
	d = mx->ni / 2;
 

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			if( i-d < 0 || i + d > row || j-d < 0 || j + d > col ){
				*p++ = 0;
				continue;
			}
 
			dx = image_mat2f_inner( sim, i-d, j-d, mx );
			dy = image_mat2f_inner( sim, i-d, j-d, my );
			g = dx*dx + dy*dy;
			
		
			*p = ( g < T )? 0 : 255;

			p++;

		}

	return( im );
}



image_type *
image_edge_pixel_value( image_type *sim, mat2f_type *m, float a, float b )
{
image_type	*im;
u_char  *p;
int     row,    col,    i,      j,	d;
float	g;
 

	if( sim->depth != 1 )	return( NULL );


	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );
 
	d = m->ni / 2;
 

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			if( i-d < 0 || i + d > row || j-d < 0 || j + d > col ){
					*p++ = 0;
					continue;
				}
 
			g = image_mat2f_inner( sim, i-d, j-d, m );
			g = b * g + a;
			*p = PUSH_TO_RANGE( g, 0, 255 );


			p++;

		}

	return( im );
}


image_type *
image_edge_gradient_value( image_type *sim, mat2f_type *mx, mat2f_type *my, float a, float b )
{
image_type	*im;
u_char  *p;
int     row,    col,    i,      j;
float	dx,	dy,	g;
int	d;
 

	if( sim->depth != 1 )	return( NULL );


	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );
 
	d = mx->ni / 2;
 

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			if( i-d < 0 || i + d > row || j-d < 0 || j + d > col ){
				*p++ = 0;
				continue;
			}
 
			dx = image_mat2f_inner( sim, i-d, j-d, mx );
			dy = image_mat2f_inner( sim, i-d, j-d, my );
			g = dx*dx + dy*dy;
			
		
			g = b * g + a;
			*p = PUSH_TO_RANGE( g, 0, 255 );

			p++;

		}

	return( im );
}
