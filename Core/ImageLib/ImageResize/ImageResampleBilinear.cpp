/**************************
 ***	Image4Sample.c	***
 *************************/

#include "ImageType/ImageType.h"

#include "ImageDump/ImageDump.h"





image_type *	
image_resample_bilinear( image_type *sim, int width, int height, image_type *im )
{

	lt2_type lt;
	lt2_unity( &lt );

	lt.a0 = sim->width / (float )width;
	lt.b1 = sim->height / (float )height;

	im = image_realloc( im, width, height, sim->channel, IMAGE_TYPE(sim), 1 );

	im = image_dewarp_lt2( sim, &lt, im );
	

	IMAGE_DUMP( im, "im", 1 , "SCALE" );

	return( im );
}


