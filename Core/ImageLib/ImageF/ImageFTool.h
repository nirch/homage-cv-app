/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGEF_TOOL_
#define         _IMAGEF_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"

struct matrix_type;


	// ImageFTool.h"
image_type *	imageF_from( image_type *sim, image_type *im );

image_type *	imageF_from_image1( image_type *im, image_type *imc );

image_type *	imageF_to_image1( image_type *sim, image_type *im );

image_type *	imageF_to_image1_S( image_type *sim, image_type *im );



image_type *	imageF_from_image2( image_type *im, image_type *imc );

image_type *	imageF_to_image2( image_type *im, image_type *imc );


image_type *	imageF_negative( image_type *sim, image_type *im );

image_type *	imagef_sub_c( image_type *im1, image_type *im0, image_type *im );

image_type *	imageF_divide( image_type *im1, image_type *im0, image_type *im );

void	imageF_minmax( image_type *im, float *min,	float *max );
void	imageFM_minmax( image_type *im, image_type *mim, float *min, float *max );


void	imageF_minmaxP( image_type *im, float percent, float *min, float *max );

void	imageFM_minmaxP( image_type *im, image_type *mim, float percent, float *min, float *max );


void	imageF_max( image_type *im, float *max );


void	imageFB_variance( image_type *im, box2i *b, float *av, float *var );

void	imageFM_variance( image_type *sim, image_type *mim, float *av, float *var );


image_type *	imageF_contrast( image_type *sim, float a, float b, image_type *im );

image_type *	imageF_linearAB( image_type *sim, float a, float b, image_type *im );

image_type *imageF_stretch( image_type *sim, image_type *im );



void	imageF_const( image_type *im, float val );

image_type *imageF_add_const( image_type *sim, float val, image_type *im );

float	imageF_compare( image_type *im1, image_type *im0 );


image_type *	imageF_to_y( image_type *sim, image_type *im );

image_type *	imageF_limited( image_type *sim, float min, float max, image_type *im );


image_type *	imageFM_limited( image_type *sim, image_type *mim, float min, float max, image_type *im );

image_type *	imageF_transform_matrix( image_type *sim, struct matrix3_type *m, image_type *im );

float			imageF_mean( image_type *im );

void			imageF_normal( image_type *im );

image_type *	imageF_binary( image_type *sim, int T, image_type *im );




	// ImageFArithmetic.c
image_type *imageF_add( image_type *sim, image_type *aim, image_type *im );

image_type *	imageF_subtrct( image_type *im1, image_type *im0, image_type *im );

image_type *	imageF_absdiff( image_type *im1, image_type *im0, image_type *im );

image_type *	imageF_abs( image_type *sim, image_type *im );

image_type *	imageF_subtrct_U8( image_type *im1, image_type *im0, image_type *im );


double	imageF_inner( image_type *im0, image_type *im1 );
double	imageF_inner_U( image_type *im0, image_type *im1 );

void	imageF_sum( image_type *sim, float f, image_type *im );



	// ImageFDynamicRAnge.c
void	imageF_dynamic_range( image_type *im, float h0, float h1 );

void	imageF_dynamic_rangeH( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 );

void	imageF_dynamic_rangeHS( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 );




	// ImageFAccumulate.c
float	imagef_accumulate( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );

float	imagef_accumulate_FC( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );


float	imagef_accumulateX( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );

float	imagef_accumulateY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );

float	imagef_accumulateXX( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );

float	imagef_accumulateXY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );

float	imagef_accumulateYY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );


void	imagef_accumulate_0_X_Y( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height,
						float a[3] );

void	imagef_accumulate_0_X_Y_XX_XY_YY( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height,
						float a[6] );



float	imageF_accumulate( image_type *im0, int i0, int j0,
				  image_type *im1, int i1, int j1,
				  int width, int height );


float	imageF_accumulate_1( image_type *im0, int i0, int j0,
					image_type *im1, int i1, int j1,
					int width, int height );

float	imageF_accumulate_US( image_type *im0, int i0, int j0,
					 image_type *im1, int i1, int j1,
					 int width, int height );



float	imageFW_accumulate( image_type *im0, int i0, int j0,
						   image_type *im1, int i1, int j1,
						   image_type *wim,
						   int width, int height );

float	imageFW_accumulate_UC( image_type *im0, int i0, int j0,
							  image_type *im1, int i1, int j1,
							  image_type *wim,
							  int width, int height );

float	imageFW_accumulate_US( image_type *im0, int i0, int j0,
							  image_type *im1, int i1, int j1,
							  image_type *wim,
							  int width, int height );




	// ImageFConvolution.c 
image_type *	imageF_convolution_guasian( image_type *sim,
						   float sigma,
						   image_type *cim );

image_type *	imageF_convolution_separably( image_type *im,
						float Cy[], float Cx[], int rC,
						image_type *cim );


image_type *imageFM_convolution_guasian( image_type *sim,
							image_type *mim,
							float sigma,
							image_type *cim );


	// ImageFPyramid.c
image_type *	imageF_pyramid( image_type *im );

image_type *	imageF_pyramid_up( image_type *im, image_type *up );

image_type *	imageF_pyramid_up_121( image_type *im, image_type *cim );


image_type *	imageF_pyramidL_121( image_type *sim, int level, image_type *im );


	// ImageFLog.c
image_type *	imageF_log( image_type *sim, image_type *im );

image_type *	imageF_unlog( image_type *sim, image_type *im );

image_type *	imageF_log10( image_type *sim, image_type *im );

image_type *	imageF_unlog10( image_type *sim, image_type *im );

//image_type *	imageF3_log( image_type *sim, image_type *im );

//image_type *	imageF3_unlog( image_type *sim, image_type *im );

	// ImageFYCrCb.c
void	imageF3_to_YCrCb( image_type *im, image_type **imY, image_type **imCr, image_type **imCb );

image_type *	imageF3_YCrCb_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );



	// ImageFFilterBox.cpp
image_type *	imageF_filter_box( image_type *sim, int rC, image_type *im );



	// ImageFResizeBilinear.c
image_type *	imageF_resize_bilinear( image_type *sim, float scale, image_type *im );

image_type *	imageF2_resize_bilinear( image_type *sim, float scale, image_type *im );


	// Image1Level.c
//image_type *	imageFM_level( image_type *sim, image_type *mim, float hp, int nRange, float range[], image_type *im );


	// ImageFMedian.c
image_type *	imageF_median( image_type *sim, int rC, image_type *im );

image_type *	imageF_median3( image_type *sim, image_type *im );

image_type *	imageFM_median3( image_type *sim, image_type *mim, image_type *im );


	// ImageFHistograme.c 
void	imageF_histogram( image_type *im, float min, float max, int h[] );

void	imageFM_histogram( image_type *im, image_type *mim, float min, float max, int h[] );



	// ImageFDensity.c
image_type *	imageF_density( int width, int height, struct pt2dA_type *apt, float h, image_type *im );

image_type *	imageFM_density( image_type *mim, struct pt2dA_type *apt, float h, image_type *im );



	// ImageAFEigen.cpp

int				imageAF_eigen( imageA_type *am, imageA_type **amE, struct matrix_type **ev, image_type **aim );

int				imageAF_covariance( imageA_type *am, struct matrix_type **m );


image_type *	imageAF_avarage( imageA_type *am, image_type *im );

imageA_type *	imageAF_subtruct(imageA_type *am, image_type *im );

image_type *	imageAF_linear_combination(imageA_type *am, double f[] );


void			imageAF_projection( imageA_type *amE, image_type *im, float f[] );



#ifdef __cplusplus
}
#endif

#endif
