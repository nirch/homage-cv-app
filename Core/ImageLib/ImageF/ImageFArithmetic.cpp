/************************
 ***   PfDeblur.cpp   ***
 ************************/
#include	<stdlib.h>
#include	<string.h>

#include "Utime/GpTime.h"

//#ifdef _DEBUG
#define _DUMP
//#endif

#include	"ImageTYpe/ImageType.h"


//#define USE_IPP
#ifdef USE_IPP
//#pragma comment( lib, "ippi.lib" )
#pragma comment( lib, "ippcorel.lib" )
#pragma comment( lib, "ippsemerged.lib" )
#pragma comment( lib, "ippsmerged.lib" )
#pragma comment( lib, "ippiemerged.lib" )
#pragma comment( lib, "ippimerged.lib" )
#include "ippi.h"


image_type *
imageF_abs( image_type *sim, image_type *im )
{
IppiSize roiSize;
IppStatus ret;

	im = image_recreate( im, sim->height, sim->width, 4, 1 );

	roiSize.width = im->width;
	roiSize.height = im->height;

	ret = ippiAbs_32f_C1R( sim->data_f, 4*sim->width,
					im->data_f, 4*im->width, roiSize );


	return( im );
}


image_type *
imageF_absdiff( image_type *im1, image_type *im0, image_type *im )
{
IppiSize roiSize;
//IppStatus ret;

return( NULL );

	im = image_recreate( im, im1->height, im1->width, 4, 1 );

	roiSize.width = im->width;
	roiSize.height = im->height;


	//ret = ippiAbsDiff_32f_C1R( im1->data_f, 4*im1->width,
	//							im0->data_f, 4*im0->width,
	//							im->data_f, 4*im->width, roiSize );
		


	return( im );
}


image_type *
imageF_subtrct( image_type *im1, image_type *im0, image_type *im )
{
IppiSize roiSize;
IppStatus ret;

	im = image_recreate( im, im1->height, im1->width, 4, 1 );

	roiSize.width = im->width;
	roiSize.height = im->height;


	ret = ippiSub_32f_C1R( im0->data_f, 4*im0->width,
								im1->data_f, 4*im1->width,
								im->data_f, 4*im->width, roiSize );



	return( im );
}


image_type *
imageF_add( image_type *im1, image_type *im0, image_type *im )
{
	IppiSize roiSize;
	IppStatus ret;

	im = image_recreate( im, im1->height, im1->width, 4, 1 );

	roiSize.width = im->width;
	roiSize.height = im->height;


	ret = ippiAdd_32f_C1R( im0->data_f, 4*im0->width,
		im1->data_f, 4*im1->width,
		im->data_f, 4*im->width, roiSize );



	return( im );
}

#else

image_type *
imageF_add( image_type *sim, image_type *aim, image_type *im )
{
	int	i,	j;
	float	*sp,	*ap,	*tp;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = sim->data_f;
	ap = aim->data_f;
	tp = im->data_f;

	for( i = 0 ; i < im->height ; i++ )
		for( j = 0 ; j < im->width ; j++, sp++, ap++,  tp++ )
			*tp = *sp + *ap;

	return( im );
}



image_type *
imageF_subtrct( image_type *im1, image_type *im0, image_type *im )
{
	float	*tp;
	float	*sp0,	*sp1;
	int	i,	j;

	im = image_reallocL( im0, im );

	sp1 = (float *)im1->data;
	sp0 = (float *)im0->data;

	tp = (float*)im->data;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *sp1++ - (float)*sp0++;
		}


		return( im );
}


image_type *
imageF_absdiff( image_type *im1, image_type *im0, image_type *im )
{
	float	*tp;
	float	*sp0,	*sp1;
	int	i,	j;

	im = image_recreate( im, im0->row, im0->column, 4, 1 );

	sp1 = (float *)im1->data;
	sp0 = (float *)im0->data;

	tp = (float*)im->data;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			float tmp = *sp1++ - *sp0++;
			if( tmp < 0 )	tmp = -tmp;
			*tp++ = tmp;
		}

		return( im );
}




image_type *
imageF_abs( image_type *sim, image_type *im )
{
	float	*tp;
	float	*sp;
	int	i,	j;



	im = image_recreate( im, sim->row, sim->column, 4, 1 );

	sp = (float *)sim->data;

	tp = (float*)im->data;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++ ){

			*tp++ = (*sp < 0 )? -*sp : *sp;;
		}

		return( im );
}

#endif


image_type *
imageF_subtrct_B( image_type *im1, image_type *im0, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_F, 1 );

	float *sp0 = im0->data_f;
	float *sp1 = im1->data_f;

	float *tp = im->data_f;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *sp1++ - (float)*sp0++;
		}


		return( im );
}

image_type *
imageF_subtrct_U8( image_type *im1, image_type *im0, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_F, 1 );

	float *sp0 = im0->data_f;
	u_char *sp1 = im1->data;

	float *tp = im->data_f;


	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = *sp1++ - (float)*sp0++;
		}


	return( im );
}



double
imageF_inner( image_type *im0, image_type *im1 )
{
double	sum,	ret;
int	i,	j;

	if( IMAGE_TYPE(im1) == IMAGE_TYPE_U8 ){
		ret = imageF_inner_U( im0,im1 );
		return( ret );
	}


	float *sp0 = im0->data_f;
	float *sp1 = im1->data_f;



	sum = 0;
	for( i = 0 ; i < im0->height ; i++ )
		for( j = 0 ; j < im0->width ; j++ ){

			sum += *sp1++  * *sp0++;
		}


	return( sum/( im0->width * im0->height ) );
}


double
imageF_inner_U( image_type *im0, image_type *im1 )
{
	double	sum;
	int	i,	j;


	float *sp0 = im0->data_f;
	u_char *sp1 = im1->data;



	sum = 0;
	for( i = 0 ; i < im0->height ; i++ )
		for( j = 0 ; j < im0->width ; j++ ){

			sum += *sp1++  * *sp0++;
		}


		return( sum/( im0->width * im0->height ) );
}


double
imageF_innerW( image_type *sim, int i0, int j0, image_type *wim )
{
	double	sum;
	int	i,	j;




	float *sp = (float *)IMAGE_PIXEL( sim, i0, j0 );
	float *wp = wim->data_f;


	int	align = sim->width - wim->width;

	sum = 0;
	for( i = 0 ; i < wim->height ; i++, sp += align )
		for( j = 0 ; j < wim->width ; j++ ){

			sum += *sp++  * *wp++;
		}


		return( sum );
}


void
imageF_sum( image_type *sim, float f, image_type *im )
{
	int	i,	j;
	float	*sp,	*tp;

	
	sp = sim->data_f;
	tp = im->data_f;

	for( i = 0 ; i < im->height ; i++ )
		for( j = 0 ; j < im->width ; j++, sp++,  tp++ )
			*tp += f * *sp;


}
