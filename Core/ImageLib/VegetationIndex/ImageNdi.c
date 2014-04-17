/***********************
 ***   Image4Ndi.c   ***
 ***********************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"VegetationIndex.h"


// NDI  -  normalized difference indices

static image_type *	image4_ndi( image_type *sim, image_type *im );
static image_type *	image3_to_ndi( image_type *sim, image_type *im );

static image_type *	imageUS3_to_ndi( image_type *sim, image_type *im );


image_type *
image_ndi( image_type *sim, image_type *im )
{
	if( sim->depth == 4 ){
		im = image4_ndi( sim, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_to_ndi( sim, im );
		return( im );
	}

	if( sim->depth == 6 ){
		im = imageUS3_to_ndi( sim, im );
		return( im );
	}

	return( NULL );
}


static image_type *
image4_ndi( image_type *sim, image_type *im )
{
u_int	*sp;
u_char	*tp;
int	i,	j;
int	R,	G,	B;
float	ndvi;
int	tmp;	


	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data_ui;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){

			R = IMAGE4_RED( *sp );
			G = IMAGE4_GREEN( *sp );
			B = IMAGE4_BLUE( *sp );

			if( G + R > 0 )
				ndvi = (G - R ) /(float)( R + G );
			else	ndvi = -1.0;

			tmp = (ndvi + 1.0 ) * 128;

			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );


		}
	}

	return( im );
}


static image_type *
image3_to_ndi( image_type *sim, image_type *im )
{
u_char	*sp;
u_char	*tp;
int	i,	j;
int	R,	G,	B;
float	ndvi;
int	tmp;	


	im = image_recreate( im, sim->height, sim->width, 1, 1 );


	sp = (u_char *)sim->data;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( G + R > 0 )
				ndvi = (G - R ) /(float)( R + G );
			else	ndvi = -1.0;

			tmp = (ndvi + 1.0 ) * 128;

			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );


		}
	}

	return( im );
}


static image_type *
imageUS3_to_ndi( image_type *sim, image_type *im )
{
u_short	*sp;
u_char	*tp;
int	i,	j;
int	R,	G,	B;
float	ndvi;
int	tmp;	


	im = image_recreate( im, sim->height, sim->width, 1, 1 );


	sp = sim->data_us;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( G + R > 0 )
				ndvi = (G - R ) /(float)( R + G );
			else	ndvi = -1.0;

			tmp = (ndvi + 1.0 ) * 128;

			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );


		}
	}

	return( im );
}