/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _IMAGE_MORPHOLOGY_
#define _IMAGE_MORPHOLOGY_

#ifdef __cplusplus
extern "C" {
#endif


void	image1_open( image_type *im, int n, int val );

void	image1_close( image_type *im, int n, int val );

void	image1_erode( image_type *im, int n );

void	image1_dilate( image_type *im, int n );



void	image1_closeBit( image_type *im, int n );

void	image1_mprphology_close( image_type *im, int val, int unset_val, int n );







#ifdef __cplusplus
}
#endif

#endif