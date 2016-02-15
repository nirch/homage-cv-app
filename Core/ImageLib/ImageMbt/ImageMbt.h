
#ifndef _IMAGE_MBT_H_
#define _IMAGE_MBT_H_

#include	<string.h>
#include	<math.h>


#include	"ImageType/ImageType.h"


#include "Uvec/Vec2d.h"


typedef struct mbt_type {
	vec2f_type	p;

	vec2f_type	tp;

	int	transparency;
} mbt_type;



image_type *	image_mbt_create( int width, int height, int dxy );


image_type *image_mbt( image_type *sim, image_type *mbt, int d, vec2f_type *dp, image_type *im );






void	image_mbt_set( image_type *mbt, pt2dA_type *capt, int transparency );


void	image_mbt_transparency( image_type *mbt, int d, int dx, int dy, image_type *sim );


#endif