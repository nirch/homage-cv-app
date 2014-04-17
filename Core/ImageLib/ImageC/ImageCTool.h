/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#ifndef         _IMAGEC_TOOL_
#define         _IMAGEC_TOOL_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"


	// ImageCTool.h"
image_type *	imageC_from_image6( image_type *im, int d, image_type *imc );

image_type *	imageC_from_image1( image_type *im, int d, image_type *imc );


image_type *	imageC_mult( image_type *im0, image_type *im1, image_type *im );

image_type *	imageC_mult_F( image_type *im0, image_type *im1, image_type *im );


image_type *	imageC_mult_6( image_type *im0, image_type *im1, image_type *im );


image_type *	imageC_abs( image_type *im0, image_type *im );

image_type *	imageC_r( image_type *sim, image_type *im );

image_type *	imageC_i( image_type *sim, image_type *im );



#ifdef __cplusplus
}
#endif

#endif
