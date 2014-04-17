/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE2_TOOL_
#define         _IMAGE2_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Umath/LT2Type.h"



	// Image2Tool.c
image_type *	image2_to_image1( image_type *sim, image_type *im );

image_type *	image2_from_image1( image_type *sim, image_type *im );

image_type *	image2_to_image1_P( image_type *sim, int val, image_type *im );


image_type *	image2_from_imageF( image_type *sim );


void	image2_const( image_type *im, int r );

image_type *	image2_subtruct( image_type *im1, image_type *im0, image_type *im );

image_type *	image2_sum_N11( image_type *im1, image_type *im0, image_type *im );

void 			image2_add_N1( image_type *aim, image_type *im );


image_type *	image2_copy( image_type *sim, image_type *im );


image_type *	image2_add_21( image_type *sim, image_type *im );

image_type *	image2_average_21( image_type *sim, int no, image_type *im );


image_type *	image2_scalar_moltiplay( image_type *sim, float a, image_type *im );

image_type *	image2_add( image_type *sim, image_type *im );

image_type *    image2_interpolate( image_type *im0, image_type *im1, image_type *wim, image_type *im);

image_type *	image2_abs_diff( image_type *im0, image_type *im1, image_type *im );


void			image2_binary_down( image_type *sim, int T );

void			image2_minus( image_type *sim );

image_type *	image2_sample2( image_type *sim, image_type *im );



	// Image2DynamicRange.c
void	image2_dynamic_range( image_type *im, box2i *box, float h0, float h1, float maxD );

int		image2_histogram( image_type *im, box2i *box, int h[4096] );

void	image2_range( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 );



	// Image2Convolution.c 
image_type *	image2_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim );


image_type *	image2_convolution_guasian( image_type *sim,
						   float sigma,
						   image_type *cim );


	// Image2Bilateral.c
image_type *	imageS_bilateral( image_type *sim, float sigma_x, float sigma_i, image_type *im );
image_type *	imageS_bilateral_asm( image_type *sim, float sigma_x, float sigma_i, image_type *im );



	// image2Derivative.c
image_type *	image2_derivative_x( image_type *sim, image_type *im );

image_type *	image2_derivative_y( image_type *sim, image_type *im );



	// ImageAccumulate.c
float	image2_accumulate( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height );

float	image2_accumulate_21(	image_type *im0, int i0, int j0,
						image_type *im1, int i1, int j1,
						int width, int height );

float	image2_accumulateX( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height );

float	image2_accumulateY( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height );


void	image2_accumulate_0_X_Y_XX_XY_YY( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height,
					double a[6] );

void	image2_accumulate_0_X_Y_21( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height,
					double a[3] );


void	image2M_accumulate_0_X_Y_XX_XY_YY( image_type *im0, int i0, int j0,
										 image_type *im1, int i1, int j1,
										 int width, int height, image_type *mim,
										 double a[6] );

void	image2M_accumulate_0_X_Y_21( image_type *im0, int i0, int j0,
							image_type *im1, int i1, int j1,
							int width, int height, image_type *mim,
							double a[3] );




void	image2_accumulateW_0_X_Y_XX_XY_YY( image_type *im0, int i0, int j0,
								   image_type *im1, int i1, int j1,
								   image_type *wim,
								   int width, int height,
								   double a[6] );


void	image2_accumulateW_0_X_Y_21( image_type *im0, int i0, int j0,
							 image_type *im1, int i1, int j1,
							 image_type *wim,
							 int width, int height,
							 double a[3] );




void	image2_accumulate_0_X_Y_XX_XY_YY_w( image_type *im0, int i0, int j0,
								   image_type *im1, int i1, int j1,
								   image_type *wim,
								   int width, int height,
								   double a[6] );

void	image2_accumulate_0_X_Y_21_w( image_type *im0, int i0, int j0,
							 image_type *im1, int i1, int j1,
							 image_type *wim,
							 int width, int height,
							 double a[3] );



float	image2_accumulateA( image_type *im0, image_type *im1, box2i *b );


float	image2_accumulate_wight( image_type *im1, image_type *im2, int i0, int j0, image_type *imw );

float	image2_accumulate_wight2( image_type *im1, image_type *im2, int i0, int j0, image_type *imw );

float	image2_accumulate_wight_21( image_type *im1, image_type *im2, int i0, int j0, image_type *imw );

float	image2_accumulate_wight_21_measure( image_type *imDx, image_type *imDy,
								   image_type *im, image_type *imt,
								   float u, float v,
								   int i0, int j0, image_type *imw );


	// Image2Color.c
void			image2_average( image_type *im, int x0, int y0, int width, int height, float *av );

image_type *	image2_negative( image_type *sim, image_type *im );


image_type *	image2_add_scalar( image_type *sim, int a, image_type *im );


image_type *	image2_contrast( image_type *sim, float a, float b, image_type *im );

image_type *	image2_enhance( image_type *sim, float a, float b, float m, image_type *im );


void			image2_var0( image_type *im, int x0, int y0, int width, int height, float av, float *var );



	// Image2Eedge.c
image_type *	image2_gridient( image_type *sim, image_type *im );



	//	Image2Interpolation.c
image_type *	imageS_interpolation_lt2( image_type *sim, lt2_type *lt,
						 int x0, int y0,
						 int width, int height,
						 image_type *im );



	// imageNoiseEstimation.c
float	imageS_noise_estimation( image_type *sim );

float	imageS_noise_estimation_R( image_type *sim, box2i *b );



	// Image2Ssd.c
float	image2_ssd(	image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height );

float	image2_ssd0( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height );



	// Image2LOg.cpp
image_type *	image2_log( image_type *sim, image_type *im );

image_type *	image2_unlog( image_type *sim, image_type *im );




	// Image2EdgeSobol.c
image_type *	image2_gridient_sobol( image_type *sim, image_type *tim );

image_type *	image2_gridient_sobol_value( image_type *sim, image_type *tim );

image_type *	image2_gridient_sobol3( image_type *sim, image_type *tim );



	// ImageSBlure.cpp
image_type *imageS_bluring( image_type *sim, int nx, int ny, image_type *im );

image_type *imageS_bluring_x( image_type *sim, int N );

image_type *imageS_bluring_y( image_type *sim, int N, image_type *im );



#ifdef __cplusplus
}
#endif

#endif
