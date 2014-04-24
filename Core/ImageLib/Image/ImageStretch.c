/*************************
 ***    ImageStrech.c  ***
 *************************/
 
#include	"ImageType/ImageType.h"



static image_type *	image4_stretch_color( image_type *sim );

 

image_type *	
image_stretch_color( image_type *sim )
{
	if( sim->depth == 1 )
		return( image1_stretch_color(sim ) );

	return( image4_stretch_color(sim ) );
}



image_type *
image1_stretch_color( image_type *sim )
{
image_type *im;
int h[256];
u_char  *p,	*tp;
int	 i,      j;
int	min,	max;
float	a,	b;
 
 
 
	p = (u_char *)sim->data;
	min = max = *p;

	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++, p++ ){
			if( *p < min )	min = *p;
			else	if( *p > max )	max = *p;
		}

	
	a = 256.0 / ( max - min );
	b = -min * a;
	for( i = 0 ; i < 256 ; i++ ){
		h[i] = a * i + b;
		if( h[i] < 0 )	h[i] = 0;
		if( h[i] > 255 ) h[i] = 255;
	}
 

	im= image_create( sim->row, sim->column, 1, 1, NULL );
	p = (u_char *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, p++ ){

			*tp++ = h[*p];
		}

	return( im );
}
	


static image_type *
image4_stretch_color( image_type *sim )
{
image_type *im;
int h[256];
u_int  *p,	*tp;
int	 i,      j;
int	g,	min,	max;
float	a,	b;
 
 
 
	p = (u_int *)sim->data;
	min = max = IMAGE4_GREEN(*p);;

	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++, p++ ){
			g = IMAGE4_GREEN(*p);
			if( g < min )	min = g;
			else	if( g > max )	max = g;
		}

	
	a = 256.0 / ( max - min );
	b = -min * a;
	for( i = 0 ; i < 256 ; i++ ){
		h[i] = a * i + b;
		if( h[i] < 0 )	h[i] = 0;
		if( h[i] > 255 ) h[i] = 255;
	}
 

	im= image_create( sim->row, sim->column, 4, 1, NULL );
	p = (u_int *)sim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, p++ ){
			g = IMAGE4_GREEN(*p);
			*tp++ = IMAGE4_RGB( h[g], h[g], h[g] );
		}

	return( im );
}