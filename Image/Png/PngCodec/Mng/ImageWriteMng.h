/****************************
 ***	ImageWriteMng.h   ***
 ****************************/
#ifndef	_IMAGE_WRITE_MNG_
#define _IMAGE_WRITE_MNG_

#include "ImageType/ImageType.h"


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PngCodecD.lib" )
#else
#pragma comment( lib, "PngCodec.lib" )
#endif

#ifdef __cplusplus
extern "C" {
#endif


#include	"MngIo.h"

/*****************  ImageWriteMng.c  ****************/

int		image_write_mng( image_type *im, palette_type *palette, char *file );

FILE *  image_write_mng_open( char *file, mngIo_type *mngIo );

void    image_write_mng_rewrite_header( FILE *fp, mngIo_type *mngIo );

int		image_write_mng_add_frame( FILE *fp, int frame_i,
			image_type *im, palette_type * palette, int fGlobalPalette );

int		image_write_mng_add_frame_delta( FILE *fp, int frame_i,
			image_type *im_delta, int x_loc, int y_loc,
			palette_type * palette, int fGlobalPalette );

int		image_write_mng_close( FILE *fp, mngIo_type *mngIo );


/****************  ImageWriteMngPng  ***************/

int		image_write_mng_add_frame__png( image_type *image, FILE *fp );

int		image_write_mng_add_frame__png_T( image_type *image, 
							  palette_type * palette, int fGlobalPalette,
							  int tranparteId, FILE *fp );

#ifdef __cplusplus
}
#endif

#endif


