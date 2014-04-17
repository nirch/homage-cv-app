/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_RESIZE_
#define         _IMAGE_RESIZE_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Umat/Mat2fType.h"
#include	"Uvec/Vec2d.h"
#include	"Ubox/Box2d.h"


	// ImageSample.c
image_type *	image_sample( image_type *sim, int d, image_type *im );

image_type *	image4_sample( image_type *sim, int d, image_type *im );

image_type *	image4_dup( image_type *sim, int d, image_type *im );

//image_type *	image4_sample2( image_type *sim, image_type *im );

image_type *	image_sample2( image_type *sim, image_type *im );

image_type *	image1_sample2( image_type *sim, image_type *im );


image_type *	image3_sample( image_type *sim, int d, image_type *im );

image_type *	image3_sample2( image_type *sim, image_type *im );

image_type *	image3_sample3( image_type *sim, image_type *im );



image_type *	image4_sample2( image_type *sim, image_type *im );


// ImageSampleColumn.c
image_type *		image_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 );

	// ImageSampleDown.c
image_type * image_sample_down( image_type *sim, int drow, int dcol );


	// ImageSampleNew.c
image_type *	image_sampleDown( image_type *sim, int drow, int dcol );



	// ImageSampleDownTransparent.c
image_type * image_sample_down_transparent( image_type *sim, int drow, int dcol );
void image_sample_down_fix_transparent( image_type *im );


	// ImageSamplingF.c - two pass float */
image_type * image_samplefc( image_type *sim, int drow, int dcol );



	// ImageSampleG.c - float version grisha*/ 
image_type *	image_sampleG( image_type *sim, float srow, float scol );

image_type *	image_sampleG_transparent( image_type *sim, float srow, float scol );



	// ImageResample.c
image_type *	image_resample( image_type *sim, int row, int col );


	// ImageResize.c
image_type *	image_resize( image_type *imd, image_type *ims, int row, int col );


	// ImageResizeIpp.c
image_type *	image_resize_ipp( image_type *sim, int width, int height, int mode, image_type *im ) ;



	// Image1ResizeBilinear.c
image_type *	image_resize_bilinear( image_type *sim, float scale, image_type *im );
image_type *	image1_resize_bilinear( image_type *sim, float scale, image_type *im );
image_type *	image3_resize_bilinear( image_type *sim, float scale, image_type *im );

image_type *	image_resize_bilinearB( image_type *sim, box2i_type *b, int width, int height, image_type *im );

#ifdef __cplusplus
}
#endif

#endif
