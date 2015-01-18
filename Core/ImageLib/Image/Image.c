#include	<stdio.h>
#include	<string.h>
#include	"ImageType/ImageType.h"


#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif

//#define _IMAGE_LEAK
//#include "ImageLeak.h"
#include "Uigp/GpMemoryLeak.h"


static	int	Im_bytes[] = {  0,
							1, //	IMAGE_TYPE_8U		1
							2, //	IMAGE_TYPE_12S		2
							2, //	IMAGE_TYPE_16U		3
							2, //	IMAGE_TYPE_16S		4

							4, //	IMAGE_TYPE_32U		5
							4, //	IMAGE_TYPE_32S		6

							4, //	IMAGE_TYPE_F		7
							8  //	IMAGE_TYPE_C		8
							};


image_type *
image_alloc( int width, int height, int channel, int type, int align )
{
image_type	*im;


	im = ( image_type *)malloc( sizeof( image_type ) );

	im->height = height;
	im->width = width;

	im->format = IMAGE_FORMAT( type, channel );

	im->channel = channel;
	im->depth = im->channel*Im_bytes[ type ];

	im->align = align;



	im->column_byte = (( im->column*im->depth + align -1 )/align ) *align;
	im->pad = im->column_byte - im->column*im->depth;


	im->data = (u_char *)malloc( im->height* im->column_byte + 16 );



	im->palette = NULL;

	im->file = NULL;

//	IMAGE_LEAK_ALLOC( im );
	GPMEMORY_LEAK_ALLOC( im );

	return( im );
}


image_type *
image_realloc( image_type *im, int width, int height, int channel, int type, int align )
{
	if( im == NULL ){
		im = image_alloc( width, height, channel, type, align );
		return( im );
	}


	if( im->width == width && im->height == height && im->depth == channel*Im_bytes[ type ] && im->align == align ){
		im->format = IMAGE_FORMAT( type, channel );
		return( im );
	}



	im->height = height;
	im->width = width;

	im->format = IMAGE_FORMAT( type, channel );

	im->channel = channel;
	im->depth = im->channel*Im_bytes[ type ];

	im->align = align;



	im->column_byte = (( im->column*im->depth + align -1 )/align ) *align;
	im->pad = im->column_byte - im->column*im->depth;


	free( im->data );
//	im->data = (u_char *)malloc( im->height* im->width+16 + im->column_byte);
	im->data = (u_char *)malloc( im->height* im->column_byte + 16);



	if( im->palette != NULL ){
		palette_destroy( im->palette );
		im->palette = NULL;
	}

	im->file = NULL;



	return( im );
}


image_type *
image_reallocL( image_type *sim, image_type *im )
{
	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE(sim), sim->align );

	return( im );
}




void
image_destroy( image_type *im, int Fdata )
{

	if( im == NULL )	return;


//	IMAGE_LEAK_DESTROY( im );
	GPMEMORY_LEAK_DESTROY( im );


	if( Fdata == 1 )
		free( im->data );

	if( im->file != NULL )
		free( im->file );

	if( im->palette != NULL )
		palette_destroy( im->palette );

	free( im );
}




image_type * 
image_copyN( image_type *sim, image_type *im )
{
	im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE(sim), 1 );

	memcpy( im->data, sim->data, im->column_byte * im->row );

	im->palette = palette_copy( sim->palette, im->palette );

	im->timeStamp = sim->timeStamp;

	return ( im );
}




int
image_copy_in( image_type *sim, int x0, int y0, image_type *im )
{
	u_char	*tp,	*sp;
	int	i,	bytes,	height;


	bytes = sim->column_byte;
	if( x0 + sim->width > im->width ){
		bytes = (im->width - x0)*im->depth;
	}

	height = sim->height;
	if( y0+ sim->height > im->height )
		height = im->height - y0;


	sp = sim->data;
	tp = IMAGE_PIXEL( im, y0, x0 );

	for( i = 0 ; i < height ; i++, tp += im->column_byte, sp += sim->column_byte ){
		memcpy( tp, sp, bytes );
	}

	return( 1 );
}

int
image_copy_in_VFLIP( image_type *sim, int x0, int y0, image_type *im )
{
	u_char	*tp,	*sp;
	int	i,	bytes,	height;


	bytes = sim->column_byte;
	if( x0 + sim->width > im->width ){
		bytes = (im->width - x0)*im->depth;
	}

	height = sim->height;
	if( y0+ sim->height > im->height )
		height = im->height - y0;


	sp = sim->data;
	tp = IMAGE_PIXEL( im, y0, x0 );
	sp = IMAGE_PIXEL( sim, height-1, 0 );

	for( i = 0 ; i < height ; i++, tp += im->column_byte, sp -= sim->column_byte ){
		memcpy( tp, sp, bytes );
	}

	return( 1 );
}




imageA_type *
imageA_alloc( int n )
{
	imageA_type	*aim;


	aim = (imageA_type *)malloc( sizeof( imageA_type) );

	aim->NA = n;
	aim->a = ( image_type **)calloc( aim->NA, sizeof(image_type*) );

	aim->nA = 0;



	return( aim );
}


imageA_type *
imageA_realloc( imageA_type *aim, int n )
{
	if( aim == NULL ){
		aim = imageA_alloc(n );
		return( aim );
	}


	if( aim->NA < n ){
		aim->a = ( image_type **)realloc( aim->a, n*sizeof(image_type *) );
		aim->NA = n;
	}

	return( aim );
}



void
imageA_destroy( imageA_type *aim )
{
int	i;

	for( i = 0 ; i  < aim->nA ; i++ )
		image_destroy( aim->a[i], 1 );


	free( aim->a );

	free( aim );
}

void
imageA_clear( imageA_type *aim )
{
int	i;

	for( i = 0 ; i  < aim->nA ; i++ ){
		if( aim->a[i] != NULL ){
			image_destroy( aim->a[i], 1 );
			aim->a[i] = 0;
		}
	}

	aim->nA = 0;
}