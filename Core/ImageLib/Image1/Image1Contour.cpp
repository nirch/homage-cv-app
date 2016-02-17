
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"
#include "Ubox/Box2d.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"


#include "ImageLabel/ImageLabel.h"




int	image1_contour( image_type *im, plnA_type **apl )
{


	imageLabel_type *abw = imageLabelUS_N( im, 128, 0, 1, NULL );



	int n = imageLabel_bigest( abw, 1 );
	

	cln_type *cln = imageLabelUS_contour( abw->im, n, 64, 0 );





	cln_translate( cln, -abw->margin, -abw->margin );


	*apl = cln_to_plnA( cln, 1 );


	plnA_smooth( *apl );


	return( 1 );
}



int 
image1_contour_all( image_type *im, int blobT, int external, plnA_type **apl )
{

	imageLabel_type *abw = imageLabelUS_N( im, 128, 0, 1, NULL );

	
	int iGroup = imageLabelUI_pixel_id( abw, 358, 495 );

	*apl = plnA_alloc( 100 );

	int	i;
	for( i = 1 ; i < abw->nA  ; i++ ){

		if( abw->a[i].no < blobT )
			continue;



		cln_type *cln = imageLabelUS_contour( abw->im, i, 32, external );

		if( cln == NULL )	continue;

		pln_type *pl = pln_copy( cln->a[0] );
		pl->size = abw->a[i].no;
		plnA_add( *apl, pl );

		cln_destroy( cln );

	}

	plnA_translate( *apl, -abw->margin, -abw->margin );

	//PLNA_DUMP( apl, "AA", iFrame, NULL );

	imageLabel_destroy( abw );

	return( 1 );
}