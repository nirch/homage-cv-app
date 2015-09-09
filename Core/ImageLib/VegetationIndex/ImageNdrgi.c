/*************************
 ***   Image4Ndrgi.c   ***
 *************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"VegetationIndex.h"

// ndi    -  normalized difference indices
// ndrgi  -  normalized difference indices

static image_type *	image4_ndrgi( image_type *sim, image_type *im );
static image_type *	image3_to_ndrgi( image_type *sim, image_type *im );

static image_type *	imageUS3_to_ndrgi( image_type *sim, image_type *im );


image_type *
image_ndrgi( image_type *sim, image_type *im )
{
	if( sim->depth == 4 ){
		im = image4_ndrgi( sim, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_to_ndrgi( sim, im );
		return( im );
	}

	if( sim->depth == 6 ){
		im = imageUS3_to_ndrgi( sim, im );
		return( im );
	}

	return( NULL );
}


static image_type *
image4_ndrgi( image_type *sim, image_type *im )
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
image3_to_ndrgi( image_type *sim, image_type *im )
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
imageUS3_to_ndrgi( image_type *sim, image_type *im )
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


image_type *
image_ndrgi_S( image_type *sim, image_type *im )
{
image_type	*tim;

	tim = image_ndrgi( sim, NULL );

	im = image1_median3( tim, im );
	image_destroy( tim, 1 );


	return( im );
}



image_type *
image_ndrgi_mask( image_type *sim, int T0, int T1, image_type *im )
{
	image_type	*tim;

//u_char	*tp;
//int	i,	j;


	tim = image_ndrgi( sim, NULL );

	im = image1_median3( tim, im );
	image_destroy( tim, 1 );

	image1_threshold2( im, T0, T1, im );

//	for( i = 0 ; i < im->height ; i++ ){
//
//		tp = IMAGE_PIXEL( im, i, 0 );
//
//	
//		for( j = 0 ; j < im->width ; j++, tp++ ){
//		
//			if( *tp <= a0 || *tp >= a2){
//				*tp = 0;
//				continue;
//			}
//
////			*tp = 255;
//		}
//	}

	return( im );

}

