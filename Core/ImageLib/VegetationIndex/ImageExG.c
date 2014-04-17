/***********************
 ***   Image4Ndi.c   ***
 ***********************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"VegetationIndex.h"


// ExG  -  Exetra Green  indices  

static image_type *	image4_exG( image_type *sim, image_type *im );
static image_type *	image3_to_exG( image_type *sim, image_type *im );



image_type *
image_exG( image_type *sim, image_type *im )
{
	if( sim->depth == 4 ){
		im = image4_exG( sim, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_to_exG( sim, im );
		return( im );
	}

	return( NULL );
}


static image_type *
image4_exG( image_type *sim, image_type *im )
{
u_int	*sp;
u_char	*tp;
int	i,	j;
int	R,	G,	B;
int	tmp;	

	
	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data_ui;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++ ){

			R = IMAGE4_RED( *sp );
			G = IMAGE4_GREEN( *sp );
			B = IMAGE4_BLUE( *sp );

			tmp = ( 2*G - R - B )*0.25 + 128;

			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );


		}
	}

	return( im );
}


static image_type *
image3_to_exG( image_type *sim, image_type *im )
{
u_char	*sp;
u_char	*tp;
int	i,	j;
int	R,	G,	B;
int	tmp;	

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = (u_char *)sim->data;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;


			tmp = ( 2*G - R - B )*0.25 + 128;

			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );


		}
	}

	return( im );
}