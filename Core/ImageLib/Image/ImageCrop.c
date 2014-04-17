/***********************
 ***   ImageCrop.c   ***
 ***********************/
#include	<string.h>

#include	"ImageType/ImageType.h"

static image_type *	image1_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );
static image_type *	image4_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );
static image_type *	image3_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );
//static image_type *	imageS3_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );

static image_type *	imageUS_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );



image_type *
image_crop_B( image_type *sim, box2i *b, image_type *im )
{
	im = image_crop( sim, b->x0, b->y0, b->x1 - b->x0,  b->y1 - b->y0, im );

	return( im );
}


image_type *
image_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
int	type;
	type = IMAGE_TYPE(sim);
	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U16 ){
		im = imageUS_crop( sim, x0, y0, width, height, im );
		return( im );
	}

	if( sim->depth == 1){
		im = image1_crop( sim, x0, y0, width, height, im );
		return( im );
	}

	if( sim->depth == 3){
		im = image3_crop( sim, x0, y0, width, height, im );
		return( im );
	}

	if( sim->depth == 4){
		im = image4_crop( sim, x0, y0, width, height, im );
		return( im );
	}

	//if( sim->depth == 6){
	//	im = imageS3_crop( sim, x0, y0, width, height, im );
	//	return( im );
	//}

	return( NULL );
} 



static image_type *
image3_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;


//	im = image_recreate( im, height, width, 3, 1 );
	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );

	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( sim, y0+i, x0 );
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}

	return( im );
}


static image_type *
image1_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;


//	im = image_recreate( im, height, width, 1, 1 );
	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );

	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( sim, y0+i, x0 );
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}

	return( im );
}


static image_type *
image4_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
	u_int	*sp,	*tp;
	int	i,	j;


//	im = image_recreate( im, height, width, 4, 1 );
	im = image_realloc( im, width, height, 4, IMAGE_TYPE_U8, 1 );

	tp = im->data_ui;
	for( i = 0 ; i < im->row ; i++ ){
		sp = (u_int *)IMAGE_PIXEL( sim, y0+i, x0 );
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp++;
		}
	}

	return( im );
}




static image_type *
imageUS_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
	u_short	*sp,	*tp;
	int	i,	j;


	im = image_realloc( im, width, height, sim->channel, IMAGE_TYPE_U16, 1 );


	tp = im->data_us;
	for( i = 0 ; i < im->row ; i++ ){
		sp = (u_short *)IMAGE_PIXEL( sim, y0+i, x0 );
		for( j = 0 ; j < im->column*im->channel ; j++ ){
			*tp++ = *sp++;
		}
	}

	return( im );
}


//static image_type *
//imageS3_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
//{
//	short	*sp,	*tp;
//	int	i,	j;
//
//
//	im = image_recreate( im, height, width, 6, 1 );
//
//	tp = im->data_s;
//	for( i = 0 ; i < im->row ; i++ ){
//		sp = (short *)IMAGE_PIXEL( sim, y0+i, x0 );
//		for( j = 0 ; j < im->column ; j++ ){
//			*tp++ = *sp++;
//			*tp++ = *sp++;
//			*tp++ = *sp++;
//		}
//	}
//
//	return( im );
//}