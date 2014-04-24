/**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"


static void	image1_flipV( image_type *im );

static void	image3_flipV( image_type *im );

static void	image4_flipV( image_type *im );



void
image_flipV( image_type *sim )
{

    switch( sim->depth ){
    case 1:
			image1_flipV( sim );
        break;

	case 3:
			image3_flipV( sim );
		break;

    case 4:
			image4_flipV( sim );
        break;
    }
} 

static void
image1_flipV( image_type *im )
{
u_char	*p0,	*p1,	tmp;
int	i,	j,	hrow;

	hrow = im->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE_PIXEL( im, i, 0 );
		p1 = IMAGE_PIXEL( im, im->row-1-i, 0 );
		for( j = 0 ; j < im->column ; j++, p0++, p1++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}


static void
image3_flipV( image_type *im )
{
	u_char	*p0,	*p1,	tmp;
	int	i,	j,	hrow;

	hrow = im->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE_PIXEL( im, i, 0 );
		p1 = IMAGE_PIXEL( im, im->row-1-i, 0 );
		for( j = 0 ; j < 3*im->column ; j++, p0++, p1++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}


static void
image4_flipV( image_type *im )
{
u_int	*p0,	*p1,	tmp;
int	i,	j,	hrow;

	hrow = im->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE4_PIXEL( im, i, 0 );
		p1 = IMAGE4_PIXEL( im, im->row-1-i, 0 );
		for( j = 0 ; j < im->column ; j++, p0++, p1++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}

static image_type *	image1_flip90( image_type *sim, image_type *im );

static image_type *	image3_flip90( image_type *sim, image_type *im );


image_type *
image_flip90( image_type *sim, image_type *im )
{

	switch( sim->depth ){
	case 1:
		im = image1_flip90( sim, im );
		break;

	case 3:
		im = image3_flip90( sim, im );
		break;

	//case 4:
	//	image4_flipV( sim );
	//	break;
	}

	return( im );
} 

static image_type *
image1_flip90( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;

	im = image_recreate( im, sim->width, sim->height, 1, 1 );


	for( i = 0 ; i < sim->height  ; i++ ){

		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, 0, i );
		for( j = 0 ; j < sim->width ; j++, tp += im->width ){
			*tp = *sp++;
		}
	}

	return( im );
}

static image_type *
image3_flip90( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;

	im = image_recreate( im, sim->width, sim->height, 3, 1 );

	for( i = 0 ; i < sim->height  ; i++ ){

		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, 0, i );
		for( j = 0 ; j < sim->width ; j++, tp += 3*im->width ){
			*tp = *sp++;
			*(tp+1) = *sp++;
			*(tp+2) = *sp++;
		}
	}

	return( im );
}