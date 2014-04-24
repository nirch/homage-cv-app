/**********************
 ***   ImageAdd.c   ***
 **********************/

#include	"ImageType/ImageType.h"



image_type *
image_append_horizontal( image_type *sim0, image_type *sim1, image_type *im )
{
	im = image_realloc( im, sim0->width+sim1->width, sim0->height, IMAGE_CHANNEL(sim0), IMAGE_TYPE(sim0), 1 );
	image_copy_in( sim0, 0, 0, im );
	image_copy_in( sim1, sim0->width, 0, im );

	image_destroy( sim0, 1 );
	image_destroy( sim1, 1 );

	return( im );
}


image_type *
image_append_vertical( image_type *sim0, image_type *sim1, image_type *im )
{
	im = image_realloc( im, sim0->width, sim0->height+sim1->height, IMAGE_CHANNEL(sim0), IMAGE_TYPE(sim0), 1 );
	image_copy_in( sim0, 0, 0, im );
	image_copy_in( sim1, 0,sim0->height, im );

	image_destroy( sim0, 1 );
	image_destroy( sim1, 1 );

	return( im );
}