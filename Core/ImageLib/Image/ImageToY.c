/***********************
 ***   ImageToY.c   ***
 ***********************/
#include	<string.h>

#include	"ImageType/ImageType.h"




image_type *	
image_to_y( image_type *sim, image_type *im )
{
	if( sim->channel ==  1 ){
		if( im != NULL && im != sim )
			image_destroy( im, 1 );
		return( sim );
	}

	if( sim->format == IMAGE_FORMAT_US3 ){
		im = imageUS3_to_y( sim, im );
		return( im  );
	}


	if( sim->format == IMAGE_FORMAT_UC3 ||  sim->depth == 3){
		im = image3_to_y( sim, im );
		return( im  );
	}

	if( sim->format == IMAGE_FORMAT_UC4 ||  sim->depth == 4 ){
		im = image4_to_y( sim, im );
		return( im  );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}

