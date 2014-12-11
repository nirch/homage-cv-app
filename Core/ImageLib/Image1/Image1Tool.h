/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE1_TOOL_
#define         _IMAGE1_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Umath/LT2Type.h"

	// Image1From.c
image_type *image1_from( image_type *sim, image_type *im );

	//Image1Tool.c
void	image1_const( image_type *im, int color );


float	image1_accumulate( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height );



void	image1_color( image_type *im, int color,
						int x0, int y0, int width, int height );


image_type *	image1_to_image4( image_type *sim, image_type *im );



image_type *	image1_abs_diff( image_type *im0, image_type *im1, image_type *im );

image_type *	image1_negative( image_type *sim, image_type *im );

image_type *	image1_add_scalar( image_type *sim, int a, image_type *im );


image_type *	image1_stretch( image_type *sim, image_type *im );

image_type *	image1_axb( image_type *sim, float a, float b, image_type *im );

image_type *	image1S_axb( image_type *sim, float a, float b, image_type *im );



// tp = a* (sp -b) + b;
image_type *	image1_contrast( image_type *sim, float a, float b, image_type *im );

int				image1_nPixel_range( image_type *sim, int T0, int T1 );

void			image1_minmax( image_type *im, int *min, int *max );

image_type *	image1_local_max( image_type *sim, image_type *im );

image_type *	image1_color_table_transform( image_type *sim, u_char table[], image_type *im );



	// Image1Sample.c
image_type *	image1_sample2( image_type *sim, image_type *im );

image_type *	image1_sample2L( image_type *sim, int level, image_type *im );

image_type *	image1B_sample2( image_type *sim, box2i_type *b, image_type *im );

image_type *	image1_dup2( image_type *sim, image_type *im );

image_type *	image1_sample( image_type *sim, int d, image_type *im );

image_type *	image1B_sample( image_type *sim, box2i_type *b, int d, image_type *im );


//image_type *	image1_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 );


image_type *	image1_dup( image_type *sim, int d, image_type *im );


image_type *	image1_sample_mathlab( image_type *sim, int d, image_type *im );

	// Image1Sample2.c
image_type *	image1_sample2_1331( image_type *sim, image_type *im );


	/* ImageInterpolation.c */
image_type *	image1_interpolation( image_type *sim,
					 float x0, float y0, int width, int height,
					 image_type *im );

image_type *	image1_interpolation_1( image_type *sim,
					 float x0, float y0, int width, int height,
					 image_type *im );


image_type *	image1_interpolationS( image_type *sim,
					 float x0, float y0, int width, int height,
					 float dx, float dy, float s,
					 image_type *im );

image_type *	image1_interpolation_lt2( image_type *sim, struct lt2_type *lt,
					int x0, int y0,
					int width, int height,
					image_type *im );

image_type *	image1_interpolation_lt2I( image_type *sim, lt2_type *lt,
						  int x0, int y0,
						  int width, int height,
						  image_type *im );


int				image1M_interpolation_lt2( image_type *sim, struct lt2_type *lt,
					int x0, int y0,
					int width, int height,
					image_type **im, image_type **mim );


int				image1_interpolation_lt2_N( image_type *sim, lt2_type *lt,
						   int x0, int y0,
						   int width, int height,
						   image_type **im );


//float			image1_interpolation_lt2_ssd( image_type *sim, struct lt2_type *lt,
//							 int x0, int y0,
//							 image_type *im, int j1, int i1,
//							 int width, int height );

float	image1_interpolation_lt2_ssd( image_type *sim, lt2_type *lt,
							 int x0, int y0,
							 image_type *im, int j1, int i1,
							 int width, int height, float *av, float *ssd );



image_type *	image1_interpolation_Alt2( image_type *sim, struct lt2A_type *alt, float f, image_type *im );





	// Image1Convolution.c 
image_type *	image1_convolution( image_type *im, image_type *gim, image_type *cim );

image_type *	image1_convolution_bilateral( image_type *im, image_type *gim, float sigma, image_type *cim );


	// Image1ConvolutionSeparably.c 
image_type *	image1_convolution_separably( image_type *im,
							float Cy[], float Cx[], int rC,
							image_type *cim );


image_type *	image1_convolution_guasian( image_type *sim,
						   float sigma,
						   image_type *cim );



	//  Image1ConvolutionM.c
image_type *	image_convolutionM( image_type *sim, struct matrix_type *m, image_type *im );




	// Image1NoiseEstimation.c
float	image1_noise_estimation( image_type *sim, float sigma, float minGradient );

float	image1_information_estimation( image_type *sim );

	// Image1Pyramid.c
image_type *	image1_pyramid_N( image_type *im );

image_type *	image1_pyramid_up( image_type *im );




// Image1EdgeSobol.c
image_type *	image1_gridient_sobol( image_type *sim, image_type *tim );

image_type *	image1_gridient_sobol_value( image_type *sim, image_type *tim );

image_type *	image1M_gridient_sobol_value( image_type *sim, image_type *mim, image_type *im );


image_type *	image1_gridient_to_value( image_type *sim, image_type *tim );

image_type *	image1_gridient_sobol3( image_type *sim, image_type *tim );



	// Image1Gravity.c
int		image1_gravity(image_type *im, vec2d *p, int dw, int dh, int nT, vec2d *cg );

image_type *	image1_binary( image_type *sim, int T, image_type *im );

image_type *	image1_binaryM( image_type *sim, int T, image_type *im );


void	image1_boundary_set( image_type *im, int val );


void	image1_threshold( image_type *sim, int T );

image_type *image1_threshold2( image_type *sim, int T0, int T1, image_type *im );

image_type *	image1_maskT( image_type *sim, image_type *mim, int T0, int T1, image_type *im );

image_type *	imageM_inverse( image_type *sim, image_type *im );

//void	image1_mask( image_type *sim, image_type *mim );

int		image1_mask_no( image_type *sim );

void	image1_set( image_type *sim, box2i *b, int val );

image_type *image1_sub( image_type *im0, image_type *im1, image_type *im );


void	image1_mask_point( image_type *im, float x0, float y0, float r );







	// Image1Pixel.c
int		image1_nPixel_nonzero( image_type *sim );

int		image1_nPixel_range( image_type *sim, int T0, int T1 );

int		image1B_nPixel_range( image_type *sim, box2i *b, int T0, int T1 );


int		image1B_nPixel_rangeN( image_type *sim, box2i *b, int T0, int T1, int nT );



	// ImageSsd.c
float	image1_ssd(	image_type *im0, int i0, int j0,
		   image_type *im1, int i1, int j1,
		   int width, int height );

void	image1_ssd_av(	image_type *im0, int i0, int j0,
			image_type *im1, int i1, int j1,
			int width, int height, float *av, float *ssd );


void	image1_ssd0( image_type *sim1, image_type *sim0,
			int x0, int y0, int width, int height, float *av, float *ssd0 );

void	image1M_ssd0( image_type *sim1, image_type *sim0, image_type *mim,
			 int x0, int y0, int width, int height, float *av, float *ssd0 );



	// Image1FisherMatrix.c
float	image_fisher_info( image_type *sim, box2i *b );

float	image1_fisher_info( image_type *sim, box2i *b );

void	image1_fisher_matrix( image_type *sim, box2i *b, struct matrix2_type *m );



	// Image1Median.c
image_type *	image1_median( image_type *sim, int rC, image_type *im );

image_type *	image1_median3( image_type *sim, image_type *im );



// Image1Histogram.c

int	image1_histogram( image_type *im, box2i *box, int h[], int fclear );

void	image1M_histogram( image_type *im, image_type *mim, int h[] );

void	image1_range( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 );

int		image1_histogram_percent( image_type *im, float pT );

int		image1M_histogram_percent( image_type *im, image_type *mim, float pT );

int		image1M_histogram_range( image_type *im, image_type *mim, int nRange, int range[] );


image_type *	image1_dynamic_range( image_type *sim, box2i *box, float h0, float h1, image_type *im );


image_type *	image1_linear_intensity( image_type *sim, float a, float b, image_type *im );

image_type *	image1_histogram_equlization( image_type *sim, image_type *im );




// Image1Average.c
image_type *	image1_avrage4_diff( image_type *sim, image_type *var, image_type *im );

image_type *	image1_average8( image_type *sim, image_type *im );

image_type *	image1_average8B( image_type *sim, image_type *im );


image_type *	image1_boxN( image_type *sim, int b, int nT, image_type *im );


// Image1Blur.c
image_type *	image1_bluring( image_type *sim, int nx, int ny, image_type *im );

image_type *	image1_bluring_x( image_type *sim, int n );

image_type *	image1_bluring_y( image_type *sim, int n, image_type *im );

image_type *	image1_blure14( image_type *sim, image_type *im );


// Image1FilterBox.c
image_type *	image1_integral( image_type *sim, image_type *im );

image_type *	image1_filter_box( image_type *sim, int n, image_type *im );

// Image1FilterIir.c
image_type *	image1_filter_IIR( image_type *sim, float r, image_type *im );

image_type *	image3_filter_IIR( image_type *sim, float r, image_type *im );



	// Image1Level.c
//image_type *	image1M_level( image_type *sim, image_type *mim, float hp, int nRange, float range[], image_type *im );
//
//image_type *	image_levelN( image_type *sim, float aL[], int nL, image_type *im );



	// Image1ResizeBilinear.c	
//image_type *	image1_resize_bilinear( image_type *sim, float scale, image_type *im );
//image_type *	image1_resize_bilinearI( image_type *sim, float scale, image_type *im );


	// Image1EqualNeighbor.c
int	image1_equal_neighbor_filter( image_type *sim, int nT );


	// Image1CornerFilter.c
int	image1_corener_filter( image_type *sim );


	// Image1Integral.c
image_type *	image1_integral_y( image_type *sim, box2i *b, int n, image_type *im );


	// Image1Majority.c
image_type *	image1_majority( image_type *sim, int N, int mH, image_type *im );

image_type *	image1M_majority( image_type *sim, image_type *mim, int N, int mH, image_type *im );


	// Image1Mask.c
image_type *	image1_mask_area( image_type *sim, int rC, image_type *im );

	// Image1MaskPln.c
image_type *	image1_mask_plnA( struct plnA_type *apl, int width, int height, image_type *im );

image_type *	image1_mask_cln( struct cln_type *cln, int width, int height, int fNormal, image_type *im );

image_type *	image1_mask_pln( struct pln_type *pl, int width, int height, image_type *im );



#ifdef __cplusplus
}
#endif

#endif
