/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#ifndef __IMAGE_BMP_H__
#define __IMAGE_BMP_H__

#ifdef __cplusplus
extern "C" {
#endif


struct image_type ;
struct gio_type;

	/* ImageWriteBmp.o */
int		image_write_bmp( struct image_type *im, char *dir, char *name );

int		image_writeF_bmp( image_type *im, char *file );

int		image_writeFN_bmp( image_type *im, char *fname, int i );



	/* ImageReadBmp.o */
//image_type *	image_read_bmp( char *dir, char *name, char *extension );

struct image_type *	image_read_bmp_file( char *file );

struct image_type * image1_read_bmp_file( char *file );

struct image_type *	image3_read_bmp_file( char *file );




struct image_type *	image_read_bmp_buffer( char *data, int bytes, int Fdata );


struct image_type *	image_read_bmp( struct gio_type *gio );

#ifdef __cplusplus
}
#endif

#endif //__IMAGE_BMP_H__
