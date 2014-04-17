
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"Image1/Image1Tool.h"

#include	"Image3Tool.h"



image_type *
image3_sample2_1331( image_type *sim, image_type *im )
{
image_type	*tim,	*aim[3];

	tim = image_band( sim, 0, NULL );
	aim[0] = image1_sample2_1331( tim, NULL );


	tim = image_band( sim, 1, tim );
	aim[1] = image1_sample2_1331( tim, NULL );


	tim = image_band( sim, 2, tim );
	aim[2] = image1_sample2_1331( tim, NULL );


	im = image_interlive( aim, 3, im );


	image_destroy( aim[0], 1 );
	image_destroy( aim[1], 1 );
	image_destroy( aim[2], 1 );

	image_destroy( tim, 1 );



	return( im );
}
