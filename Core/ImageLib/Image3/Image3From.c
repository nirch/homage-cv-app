/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"


static image_type *	image3_from_imageS( image_type *sim, image_type *im );
static image_type *	image3_from_imageS3( image_type *sim, image_type *im );

static image_type *	image3_from_imageUS3( image_type *sim, image_type *im );
static image_type *	image3_from_imageUS( image_type *sim, image_type *im );

static image_type *	image3_from_imageUI3( image_type *sim, image_type *im );


image_type *
image3_from( image_type *sim, image_type *im )
{

	if( sim->format == IMAGE_FORMAT_US3 ){
		im = image3_from_imageUS3( sim, im );
		return( im );
	}


	if( sim->format == IMAGE_FORMAT_US ){
		im = image3_from_imageUS( sim, im );
		return( im );
	}

	if( sim->format == IMAGE_FORMAT_UI3 ){
		im = image3_from_imageUI3( sim, im );
		return( im );
	}


	if( sim->depth == 1 ){
		im = image3_from_image1( sim, im );
		return( im );
	}


	if( sim->depth == 4 ){
		im = image3_from_image4( sim, im );
		return( im );
	}

	if( sim->depth == 6 ){
		im = image3_from_imageS3( sim, im );
		return( im );
	}

	if( sim->depth == 2 ){
		im = image3_from_imageS( sim, im );
		return( im );
	}


	if( sim->depth == 3 ){
		if( im == NULL )
			return( sim );

		im = image_make_copy( sim, im );
		return( im );
	}


	return( NULL );
}




image_type *
image3_from_image4( image_type *sim, image_type *im )
{
u_char	*tp;
u_int	*sp;
int	i,	j;


//	if( im == NULL )
//		im = image_create( sim->row, sim->column, 3, 1, NULL );

	im = image_realloc( im , sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );


	sp = (u_int *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){
			*tp++ = IMAGE4_RED(*sp );
			*tp++ = IMAGE4_GREEN(*sp );
			*tp++ = IMAGE4_BLUE(*sp );
		}

	return( im );
}



image_type *
image3_from_image1( image_type *sim, image_type *im )
{
u_char	*tp;
u_char	*sp;
int	i,	j;

	im = image_recreate( im, sim->row, sim->column, 3, 1 );


	sp = sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){
			*tp++ = *sp;
			*tp++ = *sp;
			*tp++ = *sp;
		}

		return( im );
}



static image_type *
image3_from_imageS3( image_type *sim, image_type *im )
{
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;

	im = image_recreate( im, sim->row, sim->column, 3 , 1 );

	sp = (short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

	return( im );
}


static image_type *
image3_from_imageS( image_type *sim, image_type *im )
{
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;

	im = image_recreate( im, sim->row, sim->column, 3 , 1 );

	sp = (short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp >> 4;
			sp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 255 );
			*tp++ = tmp;
			*tp++ = tmp;
			*tp++ = tmp;
		}
	}

	return( im );
}




static image_type *
image3_from_imageUS3( image_type *sim, image_type *im )
{
u_short	*sp;
u_char	*tp;
int	i,	j;

	im = image_recreate( im, sim->row, sim->column, 3 , 1 );

	sp = sim->data_us;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < 3*im->column ; j++ ){

			*tp++ = (*sp++) >> 8;
		}
	}

	return( im );
}

static image_type *
image3_from_imageUS( image_type *sim, image_type *im )
{
	u_short	*sp;
	u_char	*tp,	tmp;
	int	i,	j;

	im = image_recreate( im, sim->row, sim->column, 3 , 1 );

	sp = sim->data_us;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			tmp = (*sp++) >> 8;
			*tp++ = tmp;
			*tp++ = tmp;
			*tp++ = tmp;


		}
	}

	return( im );
}

static image_type *
image3_from_imageUI3( image_type *sim, image_type *im )
{
int	i,	j;
u_int	*sp;
u_char	*tp;

	im = image_recreate( im, sim->height, sim->width, 3, 1 );

	sp = sim->data_ui;
	tp = im->data;
	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->channel*im->width ; j++, sp++, tp++ )
			*tp = PUSH_TO_RANGE( *sp, 0, 255 );
	}

	return( im );
}