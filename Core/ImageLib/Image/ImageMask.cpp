/***************************
 ***   ImageMaks.c   ***
 **************************/
#include	"ImageType/ImageType.h"


static image_type *imageUS3_mask( image_type *sim, image_type *mim, image_type *im );

static image_type *image4_mask( image_type *sim, image_type *mim, int color, image_type *im );

//static image_type *image1_mask( image_type *sim, image_type *mim, image_type *im );

static image_type *image3_mask( image_type *sim, image_type *mim, int color, image_type *im );




image_type *
image_mask( image_type *sim, image_type *mim, int color, image_type *im )
{
	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U16 ){
		im = imageUS3_mask( sim, mim, im  );
		return( im );
	}


	if( sim->depth == 1 ){
		im = image1_mask( sim, mim, color, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_mask( sim, mim, color, im );
		return( im );
	}


	if( sim->depth == 4 ){
		im = image4_mask( sim, mim, color, im );
		return( im );
	}




	return( NULL );
} 



static image_type *
image4_mask( image_type *sim, image_type *mim, int color, image_type *im )
{
u_int	*sp,	*tp;
u_char	*mp;
int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 4, 1 );


	sp = sim->data_ui;
	mp = mim->data;
	tp = im->data_ui;


	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, mp++, tp++ ){

			*tp = ( *mp == 0  )? color : *sp;
		}
	}

	return( im );
}



image_type *
image1_mask( image_type *sim, image_type *mim, int color, image_type *im )
{
u_char	*sp,	*mp,	*tp;
int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data;
	mp = mim->data;
	tp = im->data;

	color = color & 0xFF;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, mp++, tp++ ){

			*tp = ( *mp == 0  )? color : *sp;
		}
	}

	return( im );
}



image_type *
image3_mask( image_type *sim, image_type *mim, int color, image_type *im )
{
	u_char	*sp,	*mp,	*tp;
	int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 3, 1 );

	sp = sim->data;
	mp = mim->data;
	tp = im->data;

	int R = COLOR_RED( color );
	int G = COLOR_GREEN( color );
	int B = COLOR_BLUE( color );

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++ ){
			if( *mp == 0 ){
				*tp++ = R;
				*tp++ = G;
				*tp++ = B;
				sp += 3;
				continue;
			}
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}

	return( im );
}




static image_type *
imageUS3_mask( image_type *sim, image_type *mim, image_type *im )
{
	u_short	*sp,	*tp;
	u_char *mp;
	int	i,	j;

	im = image_reallocL( sim, im );

	sp = sim->data_us;
	mp = mim->data;
	tp = im->data_us;

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, mp++ ){
			if( *mp == 0 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				sp += 3;
				continue;
			}
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}

	return( im );
}