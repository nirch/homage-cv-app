/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_TRANSFORM_
#define         _IMAGE_TRANSFORM_

#include	"Uigp/igp.h"
#include	"Umath/Matrix3Type.h"
#include	"ImageType/ImageType.h"

#include	"Uvec/Vec2d.h"
#include	"Ubox/Box2d.h"


#ifdef __cplusplus
extern "C" {
#endif

	// Image3KlTransform.c 
int		imageM_KL_matrix( image_type *sim, image_type *mim, matrix3_type *em, double ev[3] );

int	image3_KL_matrix( image_type *sim, matrix3_type *em, double ev[3] );

int	image3M_KL_matrix( image_type *sim, image_type *mim, matrix3_type *em, double ev[3] );

int	imageUS3M_KL_matrix( image_type *sim, image_type *mim, matrix3_type *em, double ev[3] );



image_type *	image3_KL_component( image_type *sim, int i, image_type *im );

image_type *	imageM_KL_component( image_type *sim, image_type *mim, int i, image_type *im );

image_type *	image3M_KL_component( image_type *sim, image_type *mim, int i, image_type *im );
image_type *	image3M_KL_componentF( image_type *sim, image_type *mim, int i, image_type *im );

image_type *	imageUS3M_KL_component( image_type *sim, image_type *mim, image_type *im );



void		image3M_KL_component_dump( image_type *sim, image_type *mim, int iFrame );



	/* ImageDct.c */
image_type * image_dct(image_type *sim);

image_type * image_dct_inverse(image_type *sim);

image_type * image_dct_draw(image_type *sim);

void	image_dct_highpass_filter( image_type *im, int N );

	/* ImageDct88.c */
image_type *	image_dct88( image_type *sim);
image_type *	image_idct88( image_type *sim );



	/* ImageFourier.c */
image_type *	image_fourier_amplitod( image_type *im ); 

image_type *	image_fourier(image_type *im);

image_type *	image_fourier_inverse(image_type *im);


	/* ImageFft.c */
image_type *	image_FFT(image_type *im );

image_type *	image_FFT_invese( image_type *im, int Fdestroy );



#ifdef __cplusplus
}
#endif

#endif
