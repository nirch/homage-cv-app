//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "HrSourceI.h"




CHrSourceI::CHrSourceI()
{


}

CHrSourceI::~CHrSourceI()
{
	DeleteContents();
}





void CHrSourceI::DeleteContents()
{
}



image_type *
image3_green_alpha( image_type *sim, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	u_char *sp = sim->data;
	u_int *tp = im->data_ui;

	for(i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, tp++, sp += 3 ){
			int t0 = sp[0] - 1;
			int t1 = sp[1] - 254;
			int t2 = sp[2] - 6;
			if( ABS(t0) < 16 && ABS(t1) < 16 && ABS(t2) <16 ){
		//	if( sp[0] == 1 && sp[1] == 254 && sp[2] == 6  ){
				*tp = 0;
				continue;
			}

			*tp = 0xFF000000 | IMAGE4_RGB( sp[0], sp[1], sp[2] );
		}
	}

	return( im );
}
