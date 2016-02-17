#include	<stdio.h>
#include	<string.h>
#include	"ImageType/ImageType.h"

#define _GPLOG
#include "Ulog/Log.h"


#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"

//#define _IMAGE_LEAK
//#include "ImageLeak.h"


static	int	Im_bytes[] = {  0,
							1, //	IMAGE_TYPE_8U		1
							2, //	IMAGE_TYPE_12S		2
							2, //	IMAGE_TYPE_16U		3
							2, //	IMAGE_TYPE_16S		4

							4, //	IMAGE_TYPE_32U		5
							4, //	IMAGE_TYPE_32S		6

							4 //	IMAGE_TYPE_F		7
							};




image_type *
image_create( int row, int column, int format, int pad, u_char *data )
{
image_type	*im;
int	depth,	channle;

	if( IMAGE_CHANNELB( format ) == 0 ){
		switch( format ){
		case 1:
			format = IMAGE_FORMAT_UC;
			break;
		case 3:
			format = IMAGE_FORMAT_UC3;
			break;
		case 4:
			format = IMAGE_FORMAT_UC4;
			break;
		}
	}


	channle = IMAGE_CHANNELB( format );
	if( channle == 0 ){
		depth = format;
		format = 0;
	}
	else
		depth = channle*Im_bytes[ IMAGE_TYPEB(format) ];


	im = ( image_type *)malloc( sizeof( image_type ) );

	im->row = row;
	im->column = column;
	im->depth = depth;

	im->format = (channle == 0 ) ? 0 : format;
	im->channel = (channle == 0 ) ? depth : channle;
	im->align = 1;



	im->column_byte = (( im->column*im->depth + pad -1 )/pad ) *pad;
	im->pad = im->column_byte - im->column*im->depth;

	if( data == NULL )
		im->data = (u_char *)malloc( im->row* im->column_byte+16 + im->column_byte);
	else	im->data = data;


	if( im->data == NULL )
		GPLOGF(("image->data=NULL"));

	im->file = NULL;

	im->palette = NULL;


//	IMAGE_LEAK_ALLOC( im );
	GPMEMORY_LEAK_ALLOC( im );


	return( im );
}



image_type *
image_createYUV420( int height, int width, u_char *data )
{
image_type	*im;

	// width and height must be even
	if( (height & 0x01) != 0 || (width&0x01) != 0 )	return( NULL );

	im = ( image_type *)malloc( sizeof( image_type ) );

	im->row = height;
	im->column = width;
	im->depth = 2;

	im->column_byte = im->column;
	im->pad = 0;

	if( data == NULL )
		im->data = (u_char *)malloc( (int)(im->row* im->column*1.5) );
	else	im->data = data;



	im->format = IMAGE_FORMAT_YUV420;


	im->file = NULL;

	im->palette = NULL;

	return( im );
}



image_type *
image_recreate( image_type *im, int row, int column, int format, int pad )
{
int	depth,	channle;


	if( im == NULL ){
		im = image_create( row, column, format, pad, NULL );
		return( im );
	}




	channle = IMAGE_CHANNELB( format );
	if( channle == 0 ){
		depth = format;
		format = 0;
	}
	else
		depth = channle*Im_bytes[ IMAGE_TYPEB(format) ];


	if( im->row == row && im->column == column && im->depth == depth ){
		if( format != 0 )
			im->format = format;
		return( im );
	}

	free( im->data );

	im->row = row;
	im->column = column;
	im->depth = depth;
	im->format = format;
	im->column_byte = (( im->column*im->depth + pad -1 )/pad ) *pad;
	im->pad = im->column_byte - im->column*im->depth;

	im->data = (u_char *)malloc( im->row* im->column_byte );

	if( im->data == NULL )
		GPLOGF(("image->data=NULL"));


	if( im->palette != NULL ){
		palette_destroy( im->palette );
		im->palette = NULL;
	}

	return( im );
}




image_type * 
image_make_copy( image_type *sim, image_type *im )
{

	if( im != NULL && ( im->row != sim->row || im->column != sim->column || im->depth != sim->depth ) ){
		image_destroy( im, 1 );
		im = NULL;
	}



	if( sim->format == IMAGE_FORMAT_YUV420 ){
		if( im == NULL )
			im = image_createYUV420( sim->row, sim->column, NULL );

		memcpy( im->data, sim->data, (int)(im->row* im->column*1.5) );

		im->timeStamp = sim->timeStamp;
		return( im );
	}


	im = image_recreate( im, sim->row, sim->column, sim->format, 1 );

	memcpy( im->data, sim->data, im->column_byte * im->row );


	im->palette = palette_copy( sim->palette, im->palette );

	im->timeStamp = sim->timeStamp;

	return ( im );
}



image_type * 
image_make_copy_VFLIP( image_type *sim, image_type *im )
{
	int	i;
	u_char	*sp,	*tp;

	im = image_recreate( im, sim->height, sim->width, sim->depth, 1 );


	sp = sim->data;
	tp = IMAGE_PIXEL( im, im->height-1, 0 );
	for( i = 0 ; i < sim->height ; i++, sp += sim->column_byte, tp -= sim->column_byte )
		memcpy( tp, sp, im->column_byte );



	return ( im );
}
