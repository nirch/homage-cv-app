/*
  Image2YUV.h by KEN
  image_type to YUV convertion utilities declarations
*/
#ifndef         _IMAGE_TO_YUV_
#define         _IMAGE_TO_YUV_

#ifdef __cplusplus
extern "C" {
#endif

extern "C"
{
 #include	"ImageType/ImageType.h"
}


int image_GetYUVFrame( image_type *frame, u_char **ppOutFrame, int *piSize );


void image_RGB24_to_YUV420P(unsigned char* lum,	unsigned char* cb, unsigned char* cr, 
				    	    unsigned char* src, int width, int height);

void image_GetRGB24Buffer(image_type* frame, int width, int height, 
						  unsigned char** ppOutBuffer);


image_type *	image_from_YUV420(int row, int column, u_char *pY, u_char *pU,
				  u_char *pV, int bufCol, image_type *im );



image_type *	image_read_yuv( int row, int column, int frame, char *file );

image_type *	image3_to_YUV420( image_type *sim, image_type *im );

image_type *	image_from_YUV420(int row, int column, u_char *pY, u_char *pU,
								  u_char *pV, int bufCol, image_type *im );

image_type *	imageYUV420_to_RGB( image_type *sim, image_type *im );


image_type *	imageYUV_sampling2( image_type *sim, image_type *im );


#ifdef __cplusplus
}
#endif
#endif