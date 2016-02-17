 /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>
#include    <math.h>

#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"
#include	"Image1/Image1Tool.h"

//static void	image1_const( image_type *im, int color );
static void	image3_const( image_type *im, int color );
///static void	image4_const( image_type *im, int color );


int
image_const( image_type *sim, int color )
{
	if( sim->depth == 1 ){
		image1_const( sim, color );
		return( 1 );
	}

	if( sim->depth == 3 ){
		image3_const( sim, color );
		return( 1 );
	}

	if( sim->depth == 4 ){
		image4_const( sim, color );
		return( 1 );
	}


	return( -1 );
}



void
image1_const( image_type *im, int color )
{
int	i,	j;


	color = 0xFF & color;


	u_char	*sp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*sp++ = color;

		}
}


static void
image3_const( image_type *im, int color )
{
int	R,	G,	B;
	int	i,	j;

	R = IMAGE4_RED( color );
	G = IMAGE4_GREEN( color );
	B = IMAGE4_BLUE( color );




	u_char	*sp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*sp++ = R;
			*sp++ = G;
			*sp++ = B;

		}
}


void
image4_const( image_type *im, int color )
{

	int	i,	j;

	u_int *sp = im->data_ui;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*sp++ = color;
		}
}