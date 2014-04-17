/***********************
 ***   ImageToY.c   ***
 ***********************/
#include	<string.h>

#include	"ImageType/ImageType.h"


static image_type *	image3_to_r( image_type *sim, image_type *im );

static image_type *	image4_to_r( image_type *sim, image_type *im );


static image_type *	imageUS3_to_r( image_type *sim, image_type *im );



image_type *	
image_to_r( image_type *sim, image_type *im )
{
	if( sim->channel ==  1 ){
		if( im != NULL && im != sim )
			image_destroy( im, 1 );
		return( sim );
	}

	if( sim->format == IMAGE_FORMAT_US3 ){
		im = imageUS3_to_r( sim, im );
		return( im  );
	}


	if( sim->format == IMAGE_FORMAT_UC3 ||  sim->depth == 3){
		im = image3_to_r( sim, im );
		return( im  );
	}

	if( sim->format == IMAGE_FORMAT_UC4 ||  sim->depth == 4 ){
		im = image4_to_r( sim, im );
		return( im  );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}




static image_type *
image3_to_r( image_type *sim, image_type *im )
{
	u_char	*sp;
	u_char	*tp;
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = (u_char *)sim->data;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){

			*tp++ = *sp;

		}
	}

	return( im );
}



static image_type *
image4_to_r( image_type *sim, image_type *im )
{
	u_int	*sp;
	u_char	*tp;
	int	i,	j;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data_ui;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){

			*tp++ = IMAGE4_RED(*sp);

		}
	}

	return( im );
}


static image_type *
imageUS3_to_r( image_type *sim, image_type *im )
{
	u_short	*sp,	*tp;
	int	i,	j;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );

	sp = sim->data_us;
	tp = im->data_us;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp += 3 ){

			*tp++ = *sp;
		}
	}

	return( im );
}