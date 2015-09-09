/************************
 ***   Image1From.c   ***
 ************************/

#include	"ImageType/ImageType.h"

#include	"Image1Tool.h"

static image_type *	image1_from_image2( image_type *sim, image_type *im );

static image_type *	image_from_imageUS( image_type *sim, image_type *im );



image_type *
image1_from( image_type *sim, image_type *im )
{
	if( sim->depth == 1 ){
		if( im == NULL )
			return( sim );

		im = image_make_copy( sim, im );
		return( im );
	}



	if( sim->format == IMAGE_FORMAT_US ){
		im = image_from_imageUS( sim, im );
		return( im );
	}



	if( sim->depth == 2 )
		return image1_from_image2( sim, im );


	if( sim->depth == 3 )
		return image3_to_y( sim, im );


	if( sim->depth == 4 )
		return image4_to_y( sim, im );


	return( NULL );
}







static image_type *
image1_from_image2( image_type *sim, image_type *im )
{
	short	*sp;
	u_char	*tp;
	int	i,	j;
	int	tmp;

	im = image_recreate( im, sim->row, sim->column, 1 , 1 );

	sp = (u_short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

	return( im );
}



static image_type *
image_from_imageUS( image_type *sim, image_type *im )
{
	u_short	*sp;
	u_char	*tp;
	int	i,	j;
	int	tmp;

	im = image_recreate( im, sim->row, sim->column, 1 , 1 );

	sp = (u_short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = ( *sp  )>> 8;

			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

	return( im );
}