/**********************
 *** Image8to24.c   ***
 **********************/
#include	"ImageType/ImageType.h"

image_type *
image_8to24( image_type *sim, palette_type *p, image_type *im )
{
u_int	*tp;
u_char	*sp;
int	color_arr[256];
int	r,	g,	b;
int	i,	j,	k;

	im = image_recreate( im, sim->row, sim->column, 4, 1 );

	sp = sim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < p->nColor ; i++ ) {
		r = p->data[i].Red;
		g = p->data[i].Green;
		b = p->data[i].Blue;
		color_arr[i] = IMAGE4_RGB( r, g, b );
	}

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0; j < im->column ; j++ ){
			k = *sp++;
			*tp++ = color_arr[k];
		}

	return( im );
}


// puts a transparent flag in every pixel
image_type *
image_8to24_transparent( image_type *sim, int transparent_index, palette_type *p )
{
image_type *im;
u_int	*tp;
u_char	*sp;
int	color_arr[256];
int	r,	g,	b;
int	i,	j,	k;

	im = image_create( sim->row, sim->column, 4, 1, NULL );

	sp = sim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < p->nColor ; i++ ) {
		r = p->data[i].Red;
		g = p->data[i].Green;
		b = p->data[i].Blue;
		color_arr[i] = IMAGE4_RGB( r, g, b );
	}

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0; j < im->column ; j++ ){
			k = *sp++;
			if ( k == transparent_index )
				*tp++ = ( color_arr[k] ) | 0xff000000;
			else 
				*tp++ = color_arr[k];
		}

	return( im );
}


void
image_8to24_copy( image_type *sim, image_type *im, int row0, int col0, palette_type *p )
{
u_int	*tp;
u_char	*sp;
int	color_arr[256];
int	r,	g,	b;
int	i,	j,	k,	align;

	sp = sim->data;
	tp = IMAGE4_PIXEL( im, row0, col0 );
	align = im->column - sim->column;
	
	for( i = 0 ; i < p->nColor ; i++ ) {
		r = p->data[i].Red;
		g = p->data[i].Green;
		b = p->data[i].Blue;
		color_arr[i] = IMAGE4_RGB( r, g, b );
	}

	for( i = 0 ; i < sim->row ; i++, tp += align ) {
		for( j = 0; j < sim->column ; j++ ){
			k = *sp++;
			*tp++ = color_arr[k];
		}
	}
}


void
image_8to24_copy_transparent( image_type *sim, image_type *im, int row0, int col0, int transparent_index, palette_type *p)
{
u_int	*tp;
u_char	*sp;
int	color_arr[256];
int	r,	g,	b;
int	i,	j,	k,	align;

	sp = sim->data;
	tp = IMAGE4_PIXEL( im, row0, col0 );
	align = im->column - sim->column;

	for( i = 0 ; i < p->nColor ; i++ ) {
		r = p->data[i].Red;
		g = p->data[i].Green;
		b = p->data[i].Blue;
		color_arr[i] = IMAGE4_RGB( r, g, b );
	}

	for( i = 0 ; i < sim->row ; i++, tp += align ) {
		for( j = 0; j < sim->column ; j++ ){
			k = *sp++;
			if ( k != transparent_index ) 
				*tp++ = color_arr[k];
			else 
				tp++;
		}
	}
}

