/************************
 ***   Image6Frome.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"


#define         PIXEL_12_TO_8( x )   \
	      		( ( (x) <= 0 )? 0 : (( (x) > 4096 )? 255 : ((x)>>4) ) )


image_type *
image6_from( image_type *sim, image_type *im )
{

	if( sim->depth == 6 ){
		if( im == NULL )	return( sim );

		if( im != NULL )
			im = image_make_copy( sim, NULL );

		return( im );
	}

	if( sim->depth == 4 ){
		im = image6_from_image4( sim, im );
		return( im );
	}
	
	if( sim->depth == 3 ){
		im = image6_from_image3( sim, im );
		return( im );
	}


	return( NULL );
}



image_type *
image6_from_image3( image_type *sim, image_type *im )
{
u_short	*tp;
u_char	*sp;
int	i,	j;

//	im = image_recreate( im, sim->row, sim->column, 6 , 1 );
	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_S12, 1 );

	sp = sim->data;
	tp = (u_short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *(sp++) << 4;
			*tp++ = *(sp++) << 4;
			*tp++ = *(sp++) << 4;
		}
	}

	return( im );
}


image_type *
image6_from_image4( image_type *sim, image_type *im )
{
u_short	*tp;
u_int	*sp;
int	i,	j;
int	R,	G,	B;

	im = image_recreate( im, sim->row, sim->column, 6 , 1 );

	sp = (u_int *)sim->data;
	tp = (u_short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){
			R = IMAGE4_RED( *sp );
			G = IMAGE4_GREEN( *sp );
			B = IMAGE4_BLUE( *sp );

			*tp++ = R << 4;
			*tp++ = G << 4;
			*tp++ = B << 4;
		}
	}

	return( im );
}


