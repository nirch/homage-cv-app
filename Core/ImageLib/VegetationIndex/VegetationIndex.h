#ifndef         _VEGETATION_INDEX_
#define         _VEGETATION_INDEX_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"

	// ImageNdrgi.c
image_type *image_ndrgi( image_type *sim, image_type *im );

image_type *	image_ndrgi_S( image_type *sim, image_type *im );

image_type *	image_ndrgi_mask( image_type *sim, int a0, int a2, image_type *im );

	// ImageNdrgiF.c
image_type *image_ndrgiF( image_type *sim, image_type *im );

	// ImageNdi.c
image_type *image_ndi( image_type *sim, image_type *im );



	// ImageExG.c
image_type *image_exG( image_type *sim, image_type *im );


#ifdef __cplusplus
}
#endif

#endif
