/*************************
 ***   Image4Ndrgi.c   ***
 *************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"VegetationIndex.h"

// ndi    -  normalized difference indices
// ndrgi  -  normalized difference indices

static image_type *	image4_ndrgiF( image_type *sim, image_type *im );
static image_type *	image3_to_ndrgiF( image_type *sim, image_type *im );

static image_type *	imageUS3_to_ndrgiF( image_type *sim, image_type *im );


image_type *
image_ndrgiF( image_type *sim, image_type *im )
{
	if( sim->depth == 4 ){
		im = image4_ndrgiF( sim, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_to_ndrgiF( sim, im );
		return( im );
	}

	if( sim->depth == 6 ){
		im = imageUS3_to_ndrgiF( sim, im );
		return( im );
	}

	return( NULL );
}


static image_type *
image4_ndrgiF( image_type *sim, image_type *im )
{
u_int	*sp;
float	*tp;
int	i,	j;
int	R,	G,	B;
float	ndvi;
	


	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT_F, 1 );

	sp = sim->data_ui;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){

			R = IMAGE4_RED( *sp );
			G = IMAGE4_GREEN( *sp );
			B = IMAGE4_BLUE( *sp );

			if( G + R > 0 )
				ndvi = (G - R ) /(float)( R + G );
			else	ndvi = -1.0;

			*tp++ = ndvi;
		}
	}

	return( im );
}


static image_type *
image3_to_ndrgiF( image_type *sim, image_type *im )
{
u_char	*sp;
float	*tp;
int	i,	j;
int	R,	G,	B;
float	ndvi;
	


	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT_F, 1 );


	sp = (u_char *)sim->data;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( G + R > 0 )
				ndvi = (G - R ) /(float)( R + G );
			else	ndvi = -1.0;

			*tp++ = ndvi;


		}
	}

	return( im );
}


static image_type *
imageUS3_to_ndrgiF( image_type *sim, image_type *im )
{
u_short	*sp;
float	*tp;
int	i,	j;
int	R,	G,	B;
float	ndvi;
	


	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT_F, 1 );


	sp = sim->data_us;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( G + R > 0 )
				ndvi = (G - R ) /(float)( R + G );
			else	ndvi = -1.0;

			*tp++ = ndvi;
		}
	}

	return( im );
}