/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE3_TOOL_
#define         _IMAGE3_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"



	// Image3Tool.c
image_type *	image3_to_image4( image_type *sim, image_type *im );


void		image3_const( image_type *im, int R, int G, int B );

image_type *	image3_subtruct( image_type *im1, image_type *im0, image_type *im );


//image_type *	image3_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im );


image_type *	image3_dup( image_type *sim, int drow, int dcol, image_type *im );

//image_type *	image3_sample( image_type *sim, int width, int height );
image_type *	image3_sample( image_type *sim, int d, image_type *im );

 
image_type *	image3_to_y( image_type *sim, image_type *im );

image_type *	image3_to_cr( image_type *sim, image_type *yim, image_type *im );


//image_type *	image3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im );
image_type *	image3_linear_combinationF( image_type *sim, float a, float fR, float fG, float fB, image_type *im );


image_type *	image3_mult( image_type *sim, float fr, float fg, float fb, image_type *im );


float			image3_information_estimation( image_type *sim );

void			image3_VHflip( image_type *im );


image_type *	image3_sample2( image_type *sim, image_type *im );


image_type *	image3_rotate90( image_type *sim, image_type *im );

image_type *	image3_rotate180( image_type *sim, image_type *im );

void		image3_rotate180_I( image_type *sim );



image_type *	image3_maskT( image_type *sim, image_type *mim, int T0, int T1, image_type *im );


void			image3_swap_rb( image_type *sim );


image_type *image3_abs_diff_image1( image_type *im0, image_type *im1, image_type *im );

image_type *	image3_binary( image_type *sim, int Tr, int Tg, int Tb, image_type *im );

image_type *	image3_binaryM( image_type *sim, int r, int g, int b, image_type *im );



	// Image3From.c
image_type *	image3_from( image_type *sim, image_type *im );

image_type *	image3_from_image4( image_type *sim, image_type *im );

image_type *	image3_from_image1( image_type *sim, image_type *im );

image_type *	image3_to_android( image_type *sim, image_type *im );


	// Image3Convolution.c 
image_type *	image3_convolution_separably( image_type *im,
							float Cy[], float Cx[], int rC,
							image_type *cim );

	// Image3DynamicRange.c
void	image3_dynamic_range( image_type *im, float h0, float h1 );

image_type *	image3_dynamic_rangeN( image_type *sim, box2i *box, float h0, float h1, image_type *im );


void	image3_dynamic_rangeH_R( image_type *im, box2i *box, float h0, float h1, float *a0, float *a1 );

image_type *	image3_stretching( image_type *sim, float a, float b, image_type *im );




	// ImageDeblur.c
void	image3_deblur( image_type *sim, int rC, float sigma, float a, float b, float c, int step );

void	image3_deblur_bitral( image_type *sim, int rC, float sigma,
					 float alpha, float beta, float lamda, int step );


	// WhiteBalance.c
image_type *	image3_white_balance( image_type *sim, float alpha );




	// Image3DewarpLt2.c
//image_type *	image3_dewarp_lt2( image_type *sim, struct lt2_type *lt, float scale, image_type *tim );
image_type *	image3_dewarp_flow( image_type *sim, image_type *fim, image_type *im );

image_type *	image3_dewarp_flow_forword( image_type *sim, image_type *fim, image_type *im );


	// Image3Distance.c
float	image3_distance( image_type *im0, image_type *im1 );




	// IMage3Color.c
//image_type *	image3_to_BGR( image_type *sim, image_type *im );
//
//void	image3_bgr2rgb( image_type *im );


void			image3_VH_RGB_flip( image_type *im );


image_type*		image3_GetBlueColorGray(image_type *im_YCbCr);

image_type *	image3_bgr2ycbcr(image_type *im_BGR);

image_type *	image3_rgb2ycbcr(image_type *im_RGB);

image_type *	image3_findLiklihoodYCbCr2(image_type *im_YCbCr,int cb,int cr,int Y);

image_type *	image3_findLiklihoodYCbCr(image_type *im_YCbCr,int cb,int cr,int Y);



	// Image3Bluring.c
image_type *    image3_bluring_x( image_type *sim );

image_type *    image3_bluring_y( image_type *sim );


	// Image3Extend.c
image_type *	image3_extend_pad( image_type *sim, int d );


	// Image3YCrCb.c
void	image3_to_YCrCb( image_type *im, image_type **imY, image_type **imCr, image_type **imCb );

image_type *	image3_YCrCb_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );

image_type *	image3_YCrCb422_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );

image_type *	image3_YCrCb421_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );


	// Image3Sample2_1331.c
image_type *	image3_sample2_1331( image_type *sim, image_type *im );


	// Image3Bilnear.c
int	image3_bilnear_pixel( image_type *sim, float x, float y,  u_char tp[] );

	// Image3MapTransform.c
image_type *	image3_map_transform( image_type *sim, image_type *map, image_type *im );



	// Image3Resize43.cpp
image_type *	image3_1280X720_to_640X480( image_type *sim, image_type *im );

	// Image3Histogram.c
image_type *	image3_histogram_CrCb( image_type *sim,  image_type *mim, image_type *him );


	// Image3DewarpLt2.c
image_type *	image3_dewarp_lt2S( image_type *sim, lt2_type *lt, float scale, image_type *tim );



	// Image3FilterBoox.c
image_type *image3_filter_box( image_type *sim, int n, image_type *im );

	// Image3Streo.cc
image_type *image3_stereo( image_type *sim, image_type *mim, float _shiftSilh, float _shiftBgr, image_type *im );


	// ImagerLayer.cpp
void	image3_add_layer( image_type *im, image_type *sim, image_type *mim, int x0, int y0 );
void	image3_add_layerA( image_type *im, image_type *sim, int x0, int y0 );


#ifdef __cplusplus
}
#endif

#endif
