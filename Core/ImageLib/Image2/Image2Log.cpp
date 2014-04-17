/*********************
 ***   Image2Log.c   ***
 *********************/
#include	<math.h>
#include	"ImageType/ImageType.h"



image_type *
image2_log( image_type *sim, image_type *im )
{
short	*sp;
short	*tp;
int	i,	j;
float	F;


	im = image_recreate( im, sim->row, sim->column, 2, 1 );

	sp = sim->data_s;
	tp = im->data_s;

	F = 255.0/log(255.0);

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = 16*F*log( 1.0 + (*sp++)/16.0 );
		}

		return( im );
}



image_type *
image2_unlog( image_type *sim, image_type *im )
{
short	*sp;
short	*tp;
int	i,	j;
float	tmp;

	im = image_recreate( im, sim->row, sim->column, 2, 1 );

	sp = sim->data_s;
	tp = im->data_s;


	float F = log(255.0)/255.0;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			tmp = *sp++;
			*tp++ = 16*( exp( tmp*F/16.0 ) - 1 );
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
//	im = image_recreate( im, sim->row, sim->column, 12, 1 );
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
//	im = image_recreate( im, sim->row, sim->column, 12, 1 );
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
//
//image_type *
//imageF_log10( image_type *sim, image_type *im )
//{
//float	*sp;
//float	*tp;
//int	i,	j;
//
//
//	im = image_recreate( im, sim->row, sim->column, 4, 1 );
//
//	sp = sim->data_f;
//	tp = im->data_f;
//
//
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			*tp++ = log10( 1.0 + (*sp++) );
//		}
//
//		return( im );
//}
//
//
//
//image_type *
//imageF_unlog10( image_type *sim, image_type *im )
//{
//float	*sp;
//float	*tp;
//int	i,	j;
//float	tmp;
//
//	im = image_recreate( im, sim->row, sim->column, 4, 1 );
//
//	sp = sim->data_f;
//	tp = im->data_f;
//
//
//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
//			tmp = *sp++;
//			*tp++ = pow( 10.0F, tmp );
//		}
//
//	return( im );
//}
