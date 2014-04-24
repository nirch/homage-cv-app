/***********************
 ***   ImageFLog.c   ***
 ***********************/
#include	<math.h>
#include	"ImageType/ImageType.h"



image_type *
imageF_log( image_type *sim, image_type *im )
{
float	*sp;
float	*tp;
int	i,	j;


//	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_F, sim->align );
	im = image_reallocL( sim, im );

	sp = sim->data_f;
	tp = im->data_f;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column*im->channel ; j++ ){
			*tp++ = log( 1.0 + (*sp++) );
		}

		return( im );
}



image_type *
imageF_unlog( image_type *sim, image_type *im )
{
	float	*sp;
	float	*tp;
	int	i,	j;
	float	tmp;
	

//	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_F, sim->align );
	im = image_reallocL( sim, im );




	sp = sim->data_f;
	tp = im->data_f;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column*im->channel ; j++, sp++ ){
			tmp = *sp;
			*tp++ = exp( tmp ) - 1;
		}

		return( im );
}


//image_type *
//imageF3_log( image_type *sim, image_type *im )
//{
//float	*sp;
//float	*tp;
//int	i,	j;
//
//
////	im = image_recreate( im, sim->row, sim->column, 12, 1 );
//	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_F, 1 );
//
//	sp = sim->data_f;
//	tp = im->data_f;
//
//
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			*tp++ = log( 1.0 + (*sp++) );
//			*tp++ = log( 1.0 + (*sp++) );
//			*tp++ = log( 1.0 + (*sp++) );
//		}
//
//		return( im );
//}
//
//
//
//image_type *
//imageF3_unlog( image_type *sim, image_type *im )
//{
//float	*sp;
//float	*tp;
//int	i,	j;
//
//
////	im = image_recreate( im, sim->row, sim->column, 12, 1 );
//	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_F, 1 );
//
//	sp = sim->data_f;
//	tp = im->data_f;
//
//
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			*tp++ = exp( *sp++ ) - 1;
//			*tp++ = exp( *sp++ ) - 1;
//			*tp++ = exp( *sp++ ) - 1;
//		}
//
//		return( im );
//}

image_type *
imageF_log10( image_type *sim, image_type *im )
{
float	*sp;
float	*tp;
int	i,	j;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = sim->data_f;
	tp = im->data_f;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = log10( 1.0 + (*sp++) );
		}

		return( im );
}



image_type *
imageF_unlog10( image_type *sim, image_type *im )
{
float	*sp;
float	*tp;
int	i,	j;
float	tmp;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = sim->data_f;
	tp = im->data_f;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			tmp = *sp++;
			*tp++ = pow( 10.0F, tmp );
		}

	return( im );
}
