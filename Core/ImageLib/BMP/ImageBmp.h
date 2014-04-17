#ifndef __IMAGE_BMP_H__
#define __IMAGE_BMP_H__

#ifdef __cplusplus
extern "C" {
#endif


struct image_type ;


	// ImageWriteBmp.o
int		image_write_bmp( image_type *im, char *file );

int		image_writeN_bmp( image_type *im, char *fname, int i );

//#define image_writeF_bmp   image_write_bmp


	// ImageReadBmp.c
struct image_type *	image_read_bmp_file( char *file );

struct image_type * image1_read_bmp_file( char *file );

struct image_type *	image3_read_bmp_file( char *file );




struct image_type *	image_read_bmp_buffer( char *data, int bytes, int Fdata );

int	image_read_bmp_is_valid_SINGATURE( char *data, int bytes );



struct image_type *	image_read_bmp( struct gio_type *gio );


#ifdef __cplusplus
}
#endif

#endif //__IMAGE_BMP_H__
