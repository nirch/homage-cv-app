
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include	"Uigp/igp.h"
#include	"BMP/ImageBmp.h"
#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"
#include	"ImageDump/ImageDump.h"

#include "UCst/CstType.h"



#include "Gif/ImageReadGif.h"




#ifdef _AA_
image_type *
image_read_gift( char *file )
{

	cst_type *cst;

	if( cst_read_file( &cst, file ) <  0 )
		return( NULL );

	char signature[4];
	cst_get_buffer( cst, (u_char *)signature, 4 );

	if( strncmp( signature, "pgif", 4) != 0 ){
		cst_destroy( cst, 1 );
		return( NULL );
	}

	int nData = cst_get_int( cst );


	image_type *im = image_read_gif_buffer( (char *)cst->rp, nData, 1 );

	CST_SKIP( cst, nData );

	nData = cst_get_int( cst );

	image_type *tim = image_read_gif_buffer( (char *)cst->rp, nData, 1 );

	image_type *aim = image_band( tim, 1, NULL );
	image_destroy( tim, 1 );



	IMAGE_DUMP( im, "BB", 1, "RGB");
	IMAGE_DUMP( aim, "BB", 1, "ALPHA");

	imageA_set_alpha_band( im, aim );

	image_destroy( aim, 1 );

	cst_destroy( cst, 1 );
	return( im );


}
#endif


u_int color4 = 0;
u_int color5 = 0;
u_int color6 = 0;


image_type *
image_read_gift_from_buffer( u_char *data, int nBytes )
{

	cst_type *cst;

	cst = cst_create( (char *)data, nBytes, 1 );


	char signature[4];
	cst_get_buffer( cst, (u_char *)signature, 4 );

	if( strncmp( signature, "pgif", 4) != 0 ){
		cst_destroy( cst, 1 );
		return( NULL );
	}

	int nData = cst_get_int( cst );


	image_type *im = image_read_gif_buffer( (char *)cst->rp, nData, 1 );

	if( im->width > 100 )
	color4 = *(u_int *)IMAGE_PIXEL( im, 180, 206 );

	CST_SKIP( cst, nData );

	nData = cst_get_int( cst );

	image_type *tim = image_read_gif_buffer( (char *)cst->rp, nData, 1 );

		if( tim->width > 100 )
	color5 = *(u_int *)IMAGE_PIXEL( tim, 180, 206 );

	image_type *aim = image_band( tim, 1, NULL );
	image_destroy( tim, 1 );



	IMAGE_DUMP( im, "BB", 1, "RGB");
	IMAGE_DUMP( aim, "BB", 1, "ALPHA");

	imageA_set_alpha_band( im, aim );

	if( im->width > 100 )
	color6 = *(u_int *)IMAGE_PIXEL( im, 180, 206 );

	image_destroy( aim, 1 );

	cst_destroy( cst, 1 );
	return( im );


}




