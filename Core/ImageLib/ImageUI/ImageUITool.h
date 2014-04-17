/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_UI_TOOL_
#define         _IMAGE_UI_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"



	// imageUITool.h"

void	imageUI_const( image_type *im, int val );

image_type *	imageUI_from( image_type *sim, image_type *im );


void	imageUI_minmax( image_type *im, u_int *min, u_int *max );

void	imageUI_max( image_type *im, u_int *max );

int		imageUI_maxPixel( image_type *im, int *i0, int *j0 );

int	imageUI_compare( image_type *im1, image_type *im0 );


#ifdef _AA_
image_type *	imageUI_to_imageU8( image_type *sim, image_type *im );

image_type *	imageUI_to_U8( image_type *sim, int min, int max, image_type *im );


image_type *	imageUI3_from_imageF( image_type *sim, image_type *im );



image_type *imageUI_linearAB( image_type *sim, float a, float b, image_type *im );
#endif



	// ImageIBluring
image_type *	imageUI_bluring( image_type *sim, int n );




#ifdef __cplusplus
}
#endif

#endif
