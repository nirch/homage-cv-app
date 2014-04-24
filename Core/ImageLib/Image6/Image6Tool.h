
/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE6_TOOL_
#define         _IMAGE6_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"




	// Image6Tool.c
image_type *	image6_to_image3( image_type *sim, image_type *im );
image_type *	image6_to_image3_BGR( image_type *sim, image_type *im );

image_type *	image6_to_image3_l( image_type *sim );

void	image6_to_image3_2( image_type *sim, image_type **mim, image_type **lim );


image_type *	image6_to_image4( image_type *sim, image_type *im );



void	image6_force_range( image_type *im );


void			image6_const( image_type *im, int r, int g, int b );

image_type *	image6_subtruct( image_type *im1, image_type *im0, image_type *im );


void		image6_dynamic_range( image_type *im, float h0, float h1, float maxD );
void		image6_dynamic_range_YCrCb( image_type *im, float h0, float h1, float maxD );


image_type *	image6_to_y( image_type *sim );


image_type *	image6_mult( image_type *sim, float fr, float fg, float fb, image_type *im );


void	image6_accumulate( image_type *tim, image_type *sim, float a );

void	image6_sum( image_type *tim, image_type *sim );

void	image6_compare( image_type *tim, image_type *sim, short D[3] );

void	image6_unpack( image_type *im, image_type **imR, image_type **imG, image_type **imB );

image_type *	image6_pack( image_type *imR, image_type *imG, image_type *imB, image_type *im );

int	image6_black( image_type *im, float h0 );

float	image6_averageY( image_type *im, float min, float max );


image_type *	image6_average_21( image_type *sim, int no, image_type *im );

image_type *	image3_add_21( image_type *sim, image_type *im );


	// Image6From.c
image_type *	image6_from( image_type *sim, image_type *im );

image_type *	image6_from_image3( image_type *sim, image_type *im );

image_type *	image6_from_image4( image_type *sim, image_type *im );



	// Image6Convolution.c 
image_type *	image6_convolution_separably( image_type *im,
					float Cy[], float Cx[], int rC,
					image_type *cim );


	// Image6YCrCb.c
void	image6_to_YCrCb( image_type *im, image_type **imY, image_type **imCr, image_type **imCb );

image_type *	image6_YCrCb_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );

image_type *	image6_YCrCb421_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );
image_type *	image6_YCrCb422_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im );


	// Image6WhiteBalance.c
image_type *	image6_white_balance( image_type *sim, float alpha );




#ifdef __cplusplus
}
#endif

#endif
