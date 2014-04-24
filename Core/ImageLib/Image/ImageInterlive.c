/****************************
 ***   ImageInterlive.c   ***
 ****************************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Ubox/Box2d.h"


#include "ImageType/ImageType.h"


static image_type *	image3_interlive( image_type *aim[], int type, image_type *im );

static image_type *	imageS3_interlive( image_type *aim[], int type, image_type *im );

static image_type *	imageI3_interlive( image_type *aim[], int type, image_type *im );



image_type *
image_interlive( image_type *aim[], int nBand, image_type *im )
{
	if( IMAGE_DATA_BYTE( aim[0] ) == 1 ){
		im = image3_interlive( aim, IMAGE_TYPE(aim[0]), im );
		return( im );
	}


	if( IMAGE_DATA_BYTE( aim[0] ) == 2 ){
		im = imageS3_interlive( aim, IMAGE_TYPE(aim[0]), im );
		return( im );
	}

	if( IMAGE_DATA_BYTE( aim[0] ) == 4 ){
		im = imageI3_interlive( aim, IMAGE_TYPE(aim[0]), im );
		return( im );
	}




	
	if( aim[0]->depth == 1 ){
		im = image3_interlive( aim, IMAGE_TYPE_U8, im );
		return( im );
	}


	if( aim[0]->depth == 2 ){
		im = imageS3_interlive( aim, IMAGE_TYPE_S12, im );
		return( im );
	}

	if( aim[0]->depth == 4 ){
		im = imageI3_interlive( aim, IMAGE_TYPE_F, im );
		return( im );
	}

	return( NULL );
}


static image_type *
image3_interlive( image_type *aim[], int type, image_type *im )
{
	int	i,	j;
	u_char	*sp[3],	*tp;

	im = image_recreate( im, aim[0]->height, aim[0]->width, IMAGE_FORMAT(type, 3), 1 );


	tp = im->data;

	sp[0] = aim[0]->data;
	sp[1] = aim[1]->data;
	sp[2] = aim[2]->data;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp[0]++;
			*tp++ = *sp[1]++;
			*tp++ = *sp[2]++;
		}
	}

	return( im );
}


static image_type *
imageS3_interlive( image_type *aim[], int type, image_type *im )
{
int	i,	j;
short	*sp[3],	*tp;

	im = image_recreate( im, aim[0]->height, aim[0]->width, IMAGE_FORMAT(type,3), 1 );


	tp = im->data_s;

	sp[0] = aim[0]->data_s;
	sp[1] = aim[1]->data_s;
	sp[2] = aim[2]->data_s;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp[0]++;
			*tp++ = *sp[1]++;
			*tp++ = *sp[2]++;
		}
	}

	return( im );
}

static image_type *
imageI3_interlive( image_type *aim[], int type, image_type *im )
{
int	i,	j;
u_int	*sp[3],	*tp;


	im = image_recreate( im, aim[0]->height, aim[0]->width, IMAGE_FORMAT(type, 3), 1 );



	tp = im->data_ui;

	sp[0] = aim[0]->data_ui;
	sp[1] = aim[1]->data_ui;
	sp[2] = aim[2]->data_ui;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *sp[0]++;
			*tp++ = *sp[1]++;
			*tp++ = *sp[2]++;
		}
	}

	return( im );
}