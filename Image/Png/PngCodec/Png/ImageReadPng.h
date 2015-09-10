/***************************
 ***	ImageReadPng.h   ***
 ***************************/
#ifndef _IMAGE_READ_PNG_
#define	_IMAGE_READ_PNG_

#include "ImageType/ImageType.h"

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PngCodecD.lib" )
#else
#pragma comment( lib, "PngCodec.lib" )
#endif


#ifdef __cplusplus
extern "C" {
#endif

image_type * image_read_png_file(char *file_name);


image_type * image_read_png_buffer( char *data, int bytes, int Fdata );

int	image_read_png_is_valid_SINGATURE( char *data, int bytes );


#ifdef __cplusplus
}
#endif


#endif

