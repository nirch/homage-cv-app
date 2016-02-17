/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_US_TOOL_
#define         _IMAGE_US_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"



	// ImageUSTool.h"
image_type *	imageUS_from( image_type *sim, image_type *im );

void	imageUS_const( image_type *im, int color );


void	imageUS_minmax( image_type *im, int *min,	int *max );

void	imageUS_max( image_type *im, int *max );


image_type * imageUS3_to_y( image_type *sim, image_type *im );

//image_type *	imageUS3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );



image_type *	imageUS_to_image1( image_type *sim, image_type *im );

image_type *	imageUS_to_image1_F( image_type *sim, float f, image_type *im );



int	imageUS_compare( image_type *im1, image_type *im0 );


image_type *	imageUS_to_imageU8( image_type *sim, image_type *im );

image_type *	imageUS_to_U8( image_type *sim, int min, int max, image_type *im );


image_type *	imageUS_to_U8_smart(  image_type *sim, float p0, float p1, image_type *im );


image_type *	imageUS3_from_imageF( image_type *sim, image_type *im );



image_type *imageUS_linearAB( image_type *sim, float a, float b, image_type *im );


image_type *	imageUS_binary( image_type *sim, int T, int unit, image_type *im );

image_type *	imageUS_band( image_type *sim, int T0, float T1, image_type *im );

	// ImageUsEdgeSobol.cpp
image_type *	imageUS_gridient_sobol_value( image_type *sim, image_type *tim );


	// ImageUSInterpolate.c
image_type *	imageUS_interpolation_lt2( image_type *sim, lt2_type *lt,
						  int x0, int y0,
						  int width, int height,
						  image_type *im );


	// ImageUSInterpolate.c
image_type *	imageAS_interpolation_V( imageA_type *aim, vec2f_type *p, vec2f_type *v, 
						int x0, int width,
						image_type *im );



	// ImageUSDynamicRange.cpp
void	imageUS_dynamic_range( image_type *im, box2i_type *box, float h0, float h1, int *a0, int *a1 );

void	imageUS_dynamic_rangeH( image_type *im, float h0, float h1, int *a0, int *a1 );



#ifdef __cplusplus
}
#endif

#endif
