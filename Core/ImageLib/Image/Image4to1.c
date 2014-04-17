/**********************
 *** Image4to1.c   ***
 **********************/
#include	"ImageType/ImageType.h"

image_type *
image_4to1( image_type *sim )
{
image_type *im;
u_char	*tp;
u_int	*sp;
int	r,	g,	b;
int	i,	j;

	im = image_create( sim->row, sim->column, 1, 1, NULL );

	sp = (u_int *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0; j < im->column ; j++ ){

			r = IMAGE4_RED( *sp );
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );
			sp++;
			
			r = r >> 6;
			g = g >> 6;
			b = b >> 6;
		
			*tp++ = (((r)<<4) | ((g)<<2) | (b) );
		}

	return( im );
}


image_type *
image_1to4( image_type *sim )
{
image_type *im;
u_int	*tp;
u_char	*sp;
int	r,	g,	b;
int	i,	j;

	im = image_create( sim->row, sim->column, 4, 1, NULL );

	sp = sim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0; j < im->column ; j++ ){

			r = ( ((*sp) >> 4 ) &0x3 );
			g = ( ((*sp) >> 2 ) &0x3 );
			b = ( (*sp) &0x3 );
			sp++;

			r = (r << 6) + 32;
			g = (g << 6) + 32;
			b = (b << 6) + 32;

			*tp++ = IMAGE4_RGB( r, g, b );
		}

	return( im );
}