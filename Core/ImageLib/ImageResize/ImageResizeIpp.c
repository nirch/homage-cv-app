/**************************
 ***	Image1Resize.c	***
 *************************/
#define _DUMP
#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

//#define USE_IPP
#ifdef USE_IPP
//#pragma comment( lib, "ippi.lib" )
#pragma comment( lib, "ippcorel.lib" )
#pragma comment( lib, "ippsemerged.lib" )
#pragma comment( lib, "ippsmerged.lib" )
#pragma comment( lib, "ippiemerged.lib" )
#pragma comment( lib, "ippimerged.lib" )
#include "ippi.h"
#endif


/* mode
IPPI_INTER_NN		nearest neighbor interpolation
IPPI_INTER_LINEAR	linear interpolation
IPPI_INTER_CUBIC	cubic interpolation
IPPI_INTER_SUPER	supersampling interpolation, cannot be applied for image enlarging
IPPI_INTER_LANCZOS interpolation with Lanczos window.
*/

#ifdef USE_IPP
image_type *
image_resize_ipp( image_type *sim, int width, int height, int mode, image_type *im ) 
{
IppiSize srcSize,	dstSize;
IppStatus stat;
IppiRect srcRoi;



	if( mode < 0 )
		mode = IPPI_INTER_SUPER;


	srcSize.width = sim->width;
	srcSize.height = sim->height;


	srcRoi.x = srcRoi.y = 0;
//	srcRoi.width = width;
//	srcRoi.height = height;
	srcRoi.width = sim->width;
	srcRoi.height = sim->height;



	dstSize.width = width;
	dstSize.height = height;



	if( sim->format == IMAGE_FORMAT( IMAGE_TYPE_U16, 1 ) ){
		im = image_realloc( im, width, height, 1, IMAGE_TYPE_U16, 1 );

		stat = ippiResize_16u_C1R (sim->data_us, srcSize, (2*sim->column + sim->pad), srcRoi, 
			im->data_us, 2*im->width, dstSize, 
			(float)width/sim->width, (float)height/sim->height, mode ); 

			IMAGE_DUMP( sim, "resize.bmp", 1, NULL );
			IMAGE_DUMP( im, "resize_mask.bmp", 1, NULL );

			return( im );
	}



	if( sim->depth != 1 && sim->depth != 3 )
		return( NULL );



	im = image_recreate( im, height, width, sim->depth, 1 );

	if( im->depth == 1 )
		stat = ippiResize_8u_C1R (sim->data, srcSize, (sim->column + sim->pad), srcRoi, 
									im->data, im->width, dstSize, 
									(float)width/sim->width, (float)height/sim->height, mode ); 

	else
		stat = ippiResize_8u_C3R (sim->data, srcSize, (3*sim->column + sim->pad), srcRoi, 
									im->data, 3*im->width, dstSize, 
									(double)width/sim->width, (double)height/sim->height, mode ); 


#ifdef _DUMP_ 
		image_dump( sim, "resize_mask.bmp", m_frame_idx, NULL );
#endif
	
		return( im );
}


#endif