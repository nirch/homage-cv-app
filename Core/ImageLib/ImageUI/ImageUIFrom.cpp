
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"imageUITool.h"


image_type *
	imageUI_from_imageF( image_type *sim, image_type *im )
{

	int	i,	j;


	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_U32, 1 );


	float *sp = (float *)sim->data_f;

	u_int *tp = im->data_ui;


	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column*im->channel ; j++, sp++, tp++ ){
			float tmp = *sp;
//			*tp++ = PUSH_TO_RANGE( tmp, 0, (1<<32)-1 );
			*tp = tmp;

		}

		return( im );
}








#ifdef _AA_
image_type *
imageUI_to_U8( image_type *sim, int min, int max, image_type *im )
{
	u_int	*sp;
	u_char	*tp;
	int	i,	j;
	float	t;
	int	tmp;


	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_U8, 1 );


	sp = sim->data_ui;
	tp = im->data;

	t = 255.0 / ( max-min);

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < sim->channel*im->column ; j++ ){



			tmp = ((*sp++) - min )*t + 0.5;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}


image_type *
imageUI_to_imageU8( image_type *sim, image_type *im )
{
	int	min,	max;


	imageUI_minmax( sim, &min, &max );


	im = imageUI_to_U8( sim, min, max, im );
	return( im );
}




image_type *
imageUI_linearAB( image_type *sim, float a, float b, image_type *im )
{
	u_short	*sp,	*tp;
	int	i,	j,	tmp;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );


	sp = sim->data_s;
	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a* *sp + b;
			*tp = PUSH_TO_RANGE( tmp, 0, 65535 );
		}
	}

	return( im );
}
#endif