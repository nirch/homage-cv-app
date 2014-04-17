
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"


image_type *	image3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );

image_type *	imageUS3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );



image_type *	
image_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im )
{

	if( sim->format == IMAGE_FORMAT_US3 ){
		im = imageUS3_linear_combination( sim, a, fR, fG, fB, im );
		return( im  );
	}


	if( sim->format == IMAGE_FORMAT_UC3 ){
		im = image3_linear_combination( sim, a, fR, fG, fB, im );
		return( im  );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}




image_type *
image3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im )
{
	u_char	*sp;
	u_char	*tp;
	int	i,	j;

	int	R,	G,	B;

	int	Y;




	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = (u_char *)sim->data;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;


			Y  =  a + fR * R + fG * G + fB * B + 0.5;
			*tp++ = PUSH_TO_RANGE( Y, 0, 255 );
		}
	}

	return( im );
}


image_type *
imageUS3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im )
{
	u_short	*sp;
	u_short	*tp;
	int	i,	j;

	int	R,	G,	B;

	int	Y;




	im = image_recreate( im, sim->height, sim->width, IMAGE_FORMAT_US, 1 );

	sp = sim->data_us;
	tp = im->data_us;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;


			Y  =  a + fR * R + fG * G + fB * B + 0.5;
			*tp++ = PUSH_TO_RANGE( Y, 0, ((1<<16)-1) );
		}
	}

	return( im );
}


