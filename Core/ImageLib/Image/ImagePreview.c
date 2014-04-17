/****************************
 ***    ImagePreview.c    ***
 ****************************/
 
#include	"ImageType/ImageType.h"


image_type *
image_preview( image_type *im, int size, int color )
{
image_type	*sim,	*sim1;
int	sample,	max;
 
 
	max = MAX( im->row, im->column );
	if( max < size ){
		sim1 = image_create( size, size, 4, 1, NULL );
		image4_const( sim1, color );
//		image4_copy_in( im, sim1, (size - im->row)/2, ( size - im->column)/2 );

		image_copy_in( im, ( size - im->column)/2, (size - im->row)/2, sim1 );


		return( sim1 );
	}

 
	sample = ( max + size-1) / size;
	sim = image4_sample( im, sample, NULL );
 
 
	if( sim->row == size && sim->column == size )   return( sim );
 
	sim1 = image_create( size, size, 4, 1, NULL );
	image4_const( sim1, color );
//	image4_copy_in( sim, sim1, (size - sim->row)/2, ( size - sim->column)/2 );

 	image_copy_in( im, ( size - im->column)/2, (size - im->row)/2, sim1 );
	image_destroy( sim, 1 );
 
	return( sim1 );
}
