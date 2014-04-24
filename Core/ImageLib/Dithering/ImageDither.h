#ifndef         _IMAGE_DITHER_
#define         _IMAGE_DITHER_


#ifdef __cplusplus
extern "C" {
#endif


#include	../ImageType/ImageType.h"




	/* ImageDitherPat.c */
image_type *	image_dithering_random( image_type *im, image_type *new_im  );

image_type *	image_dithering_pattern1( image_type *im, image_type *new_im );

image_type *	image_dithering_pattern2( image_type *im, image_type *new_im );

	/* ImageDitherOrd.c */
image_type *	image_dithering_ordered_2x2( image_type *im, image_type *new_im );

image_type *	image_dithering_ordered_3x3_1( image_type *im, image_type *new_im );

image_type *	image_dithering_ordered_3x3_2( image_type *im, image_type *new_im );

image_type *	image_dithering_ordered_4x4( image_type *im, image_type *new_im );

	/* ImageDitherErr.c */
image_type *	image_dithering_error_simpler( image_type *im, image_type *new_im );

image_type *	image_dithering_error_floyd( image_type *im, image_type *new_im );

image_type *	image_dithering_error_burke( image_type *im, image_type *new_im );

image_type *	image_dithering_error_jarvis( image_type *im, image_type *new_im );

image_type *	image_dithering_error_stucki( image_type *im, image_type *new_im );


image_type *	image_dithering_error_gray16( image_type *im, image_type *new_im );

image_type *	image_dithering_error_floyd_gray16( image_type *im, image_type *new_im );

image_type *    image_dithering_error_floyd_grey2b( image_type *im, image_type *new_im );


	/* ImageDitherAll.c */
image_type *	image_dithering_all( image_type *im, image_type *new_im );


 
#ifdef __cplusplus
}
#endif

#endif
