/***********************
 ***   ImageBAnd.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Ubox/Box2d.h"


#include "ImageType/ImageType.h"


static image_type *	image1_band( image_type *sim, int iBand, image_type *im );
static image_type *	image2_band( image_type *sim, int iBand, image_type *im );
static image_type *	image4_band( image_type *sim, int iBand, image_type *im );



image_type *
image_band( image_type *sim, int iBand, image_type *im )
{
	if( sim->channel == 1 )
		return( sim );

	if( IMAGE_DATA_BYTE( sim ) == 1 ){
		im = image1_band( sim, iBand, im );
		return( im );
	}


	if( IMAGE_DATA_BYTE( sim ) == 2 ){
		im = image2_band( sim, iBand, im );
		return( im );
	}

	if( IMAGE_DATA_BYTE( sim ) == 4 ){
		im = image4_band( sim, iBand, im );
		return( im );
	}

	return( NULL );
}



static image_type *
image1_band( image_type *sim, int iBand, image_type *im )
{
int	i,	j,	skip;
u_char	*sp,	*tp;

	int type = IMAGE_TYPE( sim );
	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT(type, 1), 1 );

	skip = IMAGE_CHANNEL( sim );
	sp = sim->data + iBand;

	tp = im->data;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += skip , tp++ ){
			*tp = *sp;
		}
	}

	return( im );
}


static image_type *
image2_band( image_type *sim, int iBand, image_type *im )
{
	int	i,	j,	skip;
	u_short	*sp,	*tp;

	int type = IMAGE_TYPE( sim );
	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT(type, 1), 1 );

	skip = IMAGE_CHANNEL( sim );
	sp = sim->data_us + iBand;

	tp = im->data_us;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += skip , tp++ ){
			*tp = *sp;
		}
	}

	return( im );
}


static image_type *
image4_band( image_type *sim, int iBand, image_type *im )
{
	int	i,	j,	skip;
	u_int	*sp,	*tp;

	int type = IMAGE_TYPE( sim );
	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT(type, 1), 1 );

	skip = IMAGE_CHANNEL( sim );
	sp = sim->data_ui + iBand;

	tp = im->data_ui;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += skip , tp++ ){
			*tp = *sp;
		}
	}

	return( im );
}
