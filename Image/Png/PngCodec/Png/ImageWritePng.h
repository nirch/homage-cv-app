/***************************
 ***	ImageWritePng.h   ***
 ***************************/
#ifndef _IMAGE_WRITE_PNG_
#define	_IMAGE_WRITE_PNG_



#include "ImageType/ImageType.h"


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PngCodecD.lib" )
#else
#pragma comment( lib, "PngCodec.lib" )
#endif

#ifdef __cplusplus
extern "C" {
#endif


#ifdef WIN32

#define		IMAGE2PNG_RGB( color ) 	((((color) & 0xff)<<16) | ((((color) >> 8) & 0xff)<<8) | (((color) >> 16) & 0xff) )
#endif


#ifdef __linux

#define		IMAGE2PNG_RGB( color ) 	((((color) & 0xff)<<16) | ((((color) >> 8) & 0xff)<<8) | (((color) >> 16) & 0xff) )
#endif



#ifdef PALMOS

#define		IMAGE2PNG_RGB( color ) 	((((color) & 0xff)<<16) | ((((color) >> 8) & 0xff)<<8) | (((color) >> 16) & 0xff) )
#endif


#ifndef IMAGE2PNG_RGB

#define		IMAGE2PNG_RGB( color ) 	((((color) & 0xff)<<16) | ((((color) >> 8) & 0xff)<<8) | (((color) >> 16) & 0xff) )
#endif



int image_write_png(image_type *image,char *file_name);

int image_write_png_T( image_type *image,
						palette_type *palette, int tranparteId,
						char *file_name);


int image_write_png_TA( image_type *image, char *file_name);


#ifdef __cplusplus
}
#endif

#endif
