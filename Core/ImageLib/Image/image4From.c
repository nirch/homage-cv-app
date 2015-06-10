/**************************
 ***	Image4From.c	***
 *************************/

#include "ImageType/ImageType.h"


static image_type *	image4_from_image3( image_type *sim, image_type *im );

static  image_type *	image4_from_imageS( image_type *sim, image_type *im );

static image_type *	image4_from_imageS3( image_type *sim, image_type *im );

static image_type *	image4_from_imageUS3( image_type *sim, image_type *im );
static image_type *	image4_from_imageUS( image_type *sim, image_type *im );


image_type *
image4_from( image_type *sim, image_type *im )
{
	
	if( sim->format == IMAGE_FORMAT_US3 ){
		im = image4_from_imageUS3( sim, im );
		return( im );
	}

	if( sim->format == IMAGE_FORMAT_US ){
		im = image4_from_imageUS( sim, im );
		return( im );
	}

	if( sim->depth == 1 ){
		im = image4_from_image1( sim, im );
		return( im );
	}

	if( sim->depth == 2 ){
		im = image4_from_imageS( sim, im );
		return( im );
	}


	if( sim->depth == 3 ){
		im = image4_from_image3( sim, im );
		return( im );
	}
	if( sim->depth == 6 ){
		im = image4_from_imageS3( sim, im );
		return( im );
	}

	if( sim->depth == 4 ){
		if( im == NULL )
			return( sim );
			
		im = image_make_copy( sim, im );
		return( im );
	}


	return( NULL );
}


static image_type *
image4_from_image3( image_type *sim, image_type *im )
{
u_int	*tp;
u_char	*sp;
int	i,	j;


	im = image_recreate( im, sim->row, sim->column, 4, 1 );


	tp = (u_int *)im->data;
	sp = sim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			*tp++ = IMAGE4_RGB( *sp, *(sp+1), *(sp+2) );
		}

		return( im );
}

static image_type *
image4_from_imageS3( image_type *sim, image_type *im )
{
u_int	*tp;
short	*sp;
int	i,	j;
int	r,	g,	b;


	im = image_recreate( im, sim->row, sim->column, 4, 1 );


	tp = (u_int *)im->data;
	sp = sim->data_s;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			r = (*sp++)>>4;
			g = (*sp++)>>4;
			b = (*sp++)>>4;


			*tp++ = IMAGE4_RGB( r, g, b );
		}

		return( im );
}

image_type *
image4_from_image1( image_type *sim, image_type *im )
{
u_char	*sp;
u_int	*p;

int	i,	j;
int	y;

	if( sim->palette != NULL ){
		im = image_8to24( sim, sim->palette, im );
		return( im );
	}


	im = image_recreate( im, sim->height, sim->width, 4, 1 );

	sp = (u_char *)sim->data;
	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			y = *sp++;
			*p++ = IMAGE4_RGB( y, y, y );
		}
	}

	return( im );
}


static  image_type *
image4_from_imageS( image_type *sim, image_type *im )
{
short	*sp;
u_int	*p;

	int	i,	j;
	int	y;


	im = image_recreate( im, sim->height, sim->width, 4, 1 );

	sp = sim->data_s;
	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			y = *sp++;
			y >>= 4;
			*p++ = IMAGE4_RGB( y, y, y );
		}
	}

	return( im );
}




static image_type *
image4_from_imageUS3( image_type *sim, image_type *im )
{
	u_int	*tp;
	u_short	*sp;
	int	i,	j;
	int	r,	g,	b;


	im = image_recreate( im, sim->row, sim->column, 4, 1 );


	tp = (u_int *)im->data;
	sp = sim->data_s;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			r = (*sp++)>>8;
			g = (*sp++)>>8;
			b = (*sp++)>>8;


			*tp++ = IMAGE4_RGB( r, g, b );
		}

		return( im );
}


static image_type *
image4_from_imageUS( image_type *sim, image_type *im )
{
	u_int	*tp;
	u_short	*sp;
	int	i,	j;
	int	y;


	im = image_recreate( im, sim->row, sim->column, 4, 1 );


	tp = (u_int *)im->data;
	sp = sim->data_s;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			y = (*sp++)>>8;


			*tp++ = IMAGE4_RGB( y, y, y );
		}

		return( im );
}


#ifdef _AA_
image_type *
image4_to_android( image_type *sim, image_type *im )
{
u_char	*sp,	*tp,	sp0,	sp2;
	int	i,	j;

	if( sim->depth == 3 ){
		im = image3_to_android( sim, im );
		return( im );
	}

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;
	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, tp += 4, sp += 4 ){

			
			sp0 = sp[0];
			sp2 = sp[2];

			tp[0] = sp2;
			tp[1] = sp[1];
			tp[2] = sp0;
			tp[3] = sp[3];
		}
	}

	return( im );
}
#endif