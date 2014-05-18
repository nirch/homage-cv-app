/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef         _IMAGE_
#define         _IMAGE_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"


#ifdef WIN32
#pragma warning(disable :4244)
#endif



//typedef	unsigned char   u_char;


#include	"PaletteType.h"


#define	IMAGE_TYPE_U8		1
#define	IMAGE_TYPE_S12		2

#define	IMAGE_TYPE_U16		3
#define	IMAGE_TYPE_S16		4

#define	IMAGE_TYPE_U32		5
#define	IMAGE_TYPE_S32		6

#define	IMAGE_TYPE_F		7
#define	IMAGE_TYPE_C		8


#define	IMAGE_FORMAT( type, channel )	( type | ( (channel<<8)))

#define	IMAGE_FORMAT_Y			IMAGE_FORMAT( IMAGE_TYPE_U8, 1 )
#define	IMAGE_FORMAT_UC			IMAGE_FORMAT( IMAGE_TYPE_U8, 1 )
#define	IMAGE_FORMAT_RGB		IMAGE_FORMAT( IMAGE_TYPE_U8, 3 )
#define	IMAGE_FORMAT_UC3		IMAGE_FORMAT( IMAGE_TYPE_U8, 3 )

#define	IMAGE_FORMAT_RGB4		IMAGE_FORMAT( IMAGE_TYPE_U8, 4 )
#define	IMAGE_FORMAT_UC4		IMAGE_FORMAT( IMAGE_TYPE_U8, 4 )

#define	IMAGE_FORMAT_YUV420		0x08


#define	IMAGE_FORMAT_US			IMAGE_FORMAT( IMAGE_TYPE_U16, 1 )
#define	IMAGE_FORMAT_US3		IMAGE_FORMAT( IMAGE_TYPE_U16, 3 )

#define	IMAGE_FORMAT_S			IMAGE_FORMAT( IMAGE_TYPE_S12, 1 )
#define	IMAGE_FORMAT_S3			IMAGE_FORMAT( IMAGE_TYPE_S12, 3 )

#define	IMAGE_FORMAT_I			IMAGE_FORMAT( IMAGE_TYPE_S32, 1 )
#define	IMAGE_FORMAT_UI			IMAGE_FORMAT( IMAGE_TYPE_U32, 1 )

#define	IMAGE_FORMAT_UI3		IMAGE_FORMAT( IMAGE_TYPE_S32, 3 )

#define	IMAGE_FORMAT_F			IMAGE_FORMAT( IMAGE_TYPE_F, 1 )
#define	IMAGE_FORMAT_F3			IMAGE_FORMAT( IMAGE_TYPE_F, 3 )

#define	IMAGE_FORMAT_C			IMAGE_FORMAT( IMAGE_TYPE_C, 1 )



#define IMAGE_TYPE( im )		( (im)->format &0x0F )
#define IMAGE_CHANNEL( im)		( (im)->channel )

#define IMAGE_TYPEB( format )	( (format) &0x0F )
#define IMAGE_CHANNELB( format)	( (format) >> 8 )


#define IMAGE_DATA_BYTE( im )	(   ( IMAGE_TYPE(im) < IMAGE_TYPE_U8  )? 0 : \
									( IMAGE_TYPE(im) <= IMAGE_TYPE_U8  )? 1 : \
									( IMAGE_TYPE(im) <= IMAGE_TYPE_U16 )? 2 : \
									( IMAGE_TYPE(im) <= IMAGE_TYPE_F   )? 4 : 8 )



#define MAX_RC	100


typedef struct	image_type {

	union {
		int	column;
		int	width;
	};

	union {
		int	row;
		int height;
	};

	int	depth;	// byte per pixel
	int	pad;	// num of additional bytes at the end of the line (complete to 4*n bytes, so we read a whole int)

	int	column_byte;	// line length : depth*column + pad

	int	format;
	int	channel;
	int align;

	union {
		u_char	*data;
		short	*data_s;
		u_short	*data_us;
		u_int	*data_ui;
		float	*data_f;
	};

	palette_type	*palette;

	char	*file;

	vTime_type	timeStamp;
} image_type;


typedef struct imageA_type {

	int nA;
	int	NA;

	image_type	**a;

} imageA_type;


#define		IMAGE_PIXEL( im, row, column )\
					( (im)->data + (row)*(im)->column_byte +\
					(column)*(im)->depth )

#define		IMAGES_PIXEL( im, row, column )\
					(short *)( (im)->data + (row)*(im)->column_byte +\
					(column)*(im)->depth )

#define		IMAGEUS_PIXEL( im, row, column )\
					(u_short *)( (im)->data + (row)*(im)->column_byte +\
					(column)*(im)->depth )

#define		IMAGEF_PIXEL( im, row, column )\
					(float *)( (im)->data + (row)*(im)->column_byte +\
					(column)*(im)->depth )


#define		IMAGE4_PIXEL( im, row, col ) \
					( ((u_int *)(im)->data) + (row)*(im)->column + (col) )



#define		IMAGE_ROW( im )	((im)->row)
#define		IMAGE_COLUMN( im )	((im)->column)
#define		IMAGE_DATA_ADRESS( im )	((im)->data )

#define		IMAGE_DATA( im )	((im)->data )
#define		IMAGE4_DATA( im )	((u_int *)((im)->data) )


#define		IMAGE_DATA_SIZE( im )   ( IMAGE_ROW( im )* IMAGE_COLUMN( im ) )



/* This defination is only for im->depth == 4 */

#define         SP_0    SP_R
#define         SP_1    SP_G
#define         SP_2    SP_B




#ifdef WIN32
#define		IMAGE4_RED( pixel )		( ((pixel) >> 16) &0xff )
#define		IMAGE4_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		IMAGE4_BLUE( pixel )	(  (pixel)&0xff )

#define		IMAGE4_RGB( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#endif


#ifdef __linux
#define		IMAGE4_RED( pixel )		( ((pixel) >> 16) &0xff )
#define		IMAGE4_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		IMAGE4_BLUE( pixel )	(  (pixel)&0xff )

#define		IMAGE4_RGB( r, g, b ) 	(((r)<<16) | ((g)<<8) | (b) )
#endif



#ifdef PALMOS

#define		IMAGE4_BLUE( pixel )	(( (pixel) >> 16 ) &0xff )
#define		IMAGE4_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		IMAGE4_RED( pixel )		(  (pixel)&0xff )

#define		IMAGE4_RGB( r, g, b ) 	(((b)<<16) | ((g)<<8) | (r) )
#endif


#ifndef IMAGE4_RGB

#define		IMAGE4_BLUE( pixel )	( ((pixel) >> 16 ) &0xff )
#define		IMAGE4_GREEN( pixel )	( ((pixel) >> 8 ) &0xff )
#define		IMAGE4_RED( pixel )		(  (pixel)&0xff )

#define		IMAGE4_RGB( r, g, b ) 	(((b)<<16) | ((g)<<8) | (r) )
#endif


#define IMAGE_RGB2Y_F( R, G, B )	(R*0.29900 + G *0.58700 + B *0.11400)
#define IMAGE_RGB2Y( R, G, B )		((R*19595 + G *38469 + B *7471)>>16)


#define IMAGE_RGB2CMYK( r, g, b )	( ((255-b)<<16) | ((255-g)<<8) | (255-r) )

	// Image.c
image_type * image_alloc( int width, int height, int channel, int type, int align );

image_type * image_realloc( image_type *im, int width, int height, int channel, int type, int align );

image_type * image_reallocL( image_type *sim, image_type *im );


image_type * image_copyN( image_type *sim, image_type *im );

int	image_copy_in( image_type *sim, int x0, int y0, image_type *im );

int	image_copy_in_VFLIP( image_type *sim, int x0, int y0, image_type *im );


void		 image_destroy( image_type *im, int Fdata );





imageA_type *	imageA_alloc( int n );

imageA_type *	imageA_realloc( imageA_type *aim, int n );

void	imageA_destroy( imageA_type *aim );




	// ImageCreate.c
image_type *	image_create( int row, int column,
						int depth, int pad, u_char *data );

image_type *	image_createYUV420( int height, int width, u_char *data );


image_type *	image_recreate( image_type *im, int row, int column,
						int depth, int pad );


image_type *	image_make_copy( image_type *sim, image_type *im );

image_type *	image_make_copy_VFLIP( image_type *sim, image_type *im );

void IMAGE_LEAK_print();



#include	"ImageTool.h"


#ifdef __cplusplus
}
#endif

#endif
