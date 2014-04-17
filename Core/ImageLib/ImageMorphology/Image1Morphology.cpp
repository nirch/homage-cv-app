#include <math.h>
#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "ImageMorphology.h"
#include "BitMatrix.h"





void
image1_closeBit( image_type *im, int n )
{
	BitMatrix	bm;

	bm.init( im->width, im->height );

	//	image_dump( im, "1-im", 1, NULL );

	bm.Set( im );

	//	bm.Dump( "1", 1 );

	bm.open( 3 );
	//	bm.Dump( "1-close3", 1 );

	bm.Mask( im );

	//	image_dump( im, "1-close3-im", 1, NULL );

	bm.Release();
}

void
image1_open( image_type *im, int n, int val )
{
	BitMatrix	bm;

	bm.init( im->width, im->height );

	//	image_dump( im, "1-im", 1, NULL );

	bm.Set( im );

	//	bm.Dump( "1", 1 );

	bm.open( n );
	//	bm.Dump( "1-close3", 1 );

	bm.Mask( im, val );

	//	image_dump( im, "1-close3-im", 1, NULL );

	bm.Release();
}



void
image1_close( image_type *im, int n, int val )
{
	BitMatrix	bm;

	bm.init( im->width, im->height );

	//	image_dump( im, "1-im", 1, NULL );

	bm.Set( im );

	//	bm.Dump( "1", 1 );

	bm.close( n );
	//	bm.Dump( "1-close3", 1 );

	bm.Mask( im, val );

	//	image_dump( im, "1-close3-im", 1, NULL );

	bm.Release();
}

void
image1_erode( image_type *im, int n )
{
	BitMatrix	bm;

	bm.init( im->width, im->height );

	//	image_dump( im, "1-im", 1, NULL );

	bm.Set( im );

	//	bm.Dump( "1", 1 );

	bm.erode( n );
	//	bm.Dump( "1-close3", 1 );

	bm.MaskN( im );

	//	image_dump( im, "1-close3-im", 1, NULL );

	bm.Release();
}


void
image1_dilate( image_type *im, int n )
{
	BitMatrix	bm;

	bm.init( im->width, im->height );


	bm.Set( im );


	bm.dilate( n );
	//	bm.Dump( "1-close3", 1 );

	bm.UNMask( im );

	bm.Release();
}



void
image1_mprphology_close( image_type *im, int val, int unset_val, int n )
{
	BitMatrix	bm;

	bm.init( im->width, im->height );

#ifdef _DUMP
	image_dump( im, "1-im", 1, NULL );
#endif


	bm.Set( im, val );
#ifdef _DUMP
	bm.Dump( "1", 1 );
#endif

	bm.erode( 1 );
#ifdef _DUMP
	bm.Dump( "1-erode", 1 );
#endif

	bm.dilate( 1 );
#ifdef _DUMP
	bm.Dump( "1-dilate", 1 );
#endif

//	bm.close( n );
//	bm.Dump( "1-close3", 1 );

	bm.Get( im, val, unset_val );
#ifdef _DUMP
	image_dump( im, "1-close3-im", 1, NULL );
#endif
	bm.Release();
}



void
BitMatrix_dump( BitMatrix *bm, char *name, int index, char *ext )
{
	image_type *im;

	im = bm->GetImage();

	image_dump( im, name, index, ext );

	image_destroy( im, 1 );
}
