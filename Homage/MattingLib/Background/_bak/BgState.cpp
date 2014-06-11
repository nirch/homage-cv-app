/*********************************
 ***   DynamicBackground.cpp   ***
 *********************************/

#ifdef _DEBUG
#define _DUMP
#endif


#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#include	"Ulog/log.h"
#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"

#include "BgState.h"



bgStage_type *
bgStage_alloc()
{
bgStage_type *bg;

	bg = ( bgStage_type *)malloc( sizeof(bgStage_type) );

	bg->rgbBim = NULL;
	bg->bim = NULL;
	bg->bim4 = NULL;

	bg->iFrame = -1;

	return( bg );
}



void
bgStage_destroy( bgStage_type *bg )
{

	if( bg->rgbBim != NULL ){
		image_destroy( bg->rgbBim, 1 );
		bg->rgbBim = NULL;
	}


	if( bg->bim != NULL ){
		image_destroy( bg->bim, 1 );
		bg->bim = NULL;
	}

	if( bg->bim4 != NULL ){
		image_destroy( bg->bim4, 1 );
		bg->bim4 = NULL;
	}

	bg->iFrame = -1;
}



bgStage_type *
bgStage_set( bgStage_type *bg, image_type *rgbBim, image_type *bim, image_type *bim4, int iFrame )
{
	if( bg == NULL )
		bg = bgStage_alloc();

	bg->rgbBim = image_make_copy( rgbBim, bg->rgbBim );

	bg->bim = image_make_copy( bim, bg->bim );

	bg->bim4 = image_make_copy( bim4, bg->bim4 );

	bg->iFrame = iFrame;

	return( bg );
}

void
bgStage_restore( bgStage_type *bg, image_type **rgbBim, image_type **bim, image_type **bim4, int *iFrame )
{

	*rgbBim = image_make_copy( bg->rgbBim, *rgbBim );

	*bim = image_make_copy( bg->bim, *bim );

	*bim4 = image_make_copy( bg->bim4, *bim4 );

	*iFrame = bg->iFrame;
}



