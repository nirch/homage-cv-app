/******************
 *   PsdWrite.c   *
 ******************/
#include	<string.h>

#include	"Uigp/igp.h"
#include	"Ugio/GioType.h"

#include	"ImageType/ImageType.h"

#include	"ImageReadPSD.h"



#define BitmapMode		0
#define GrayscaleMode	1
#define IndexedMode		2
#define CMYKMode		4

#define CMYKColorspace	11

#define PseudoClass		2

#define MAX_LAYERS		256

#define MaxRGB			255





static int	psd_write_gio( gio_type *gio, psd_type *psd );

static void	psd_write_layer_info_image_data( gio_type *gio, psd_type *psd );

static void	psd_write_layer_info( gio_type *gio, psdLayer_type *psdL );

static void	psd_write_image_data_channel( gio_type *gio, image_type *im, int shift, int inverse );




static void	psd_write_image_data( gio_type *gio, psd_type *psd );


int	WriteImageResource_ESOLUTION( gio_type *gio, int resultion );


int	
psd_write_file( psd_type *psd, char *file )
{
gio_type	*gio;


	gio = gio_open_file_to_write( file );
	if( gio == NULL )	return( -1 );

	psd_write_gio( gio, psd );

	gio_close( gio );

	return( 1 );
}




static int
psd_write_gio( gio_type *gio, psd_type *psd )
{
int pos,	sizePos,	size;




//--- Header
	gio_put_buffer( gio, "8BPS", 4 );
	gio_put_noM( gio, 1, 2 );


//	gio_put_buffer( gio, "       ", 6 );	// 6 byte reserved

	gio_put_noM( gio, 0, 2 );				//6 byte reserved
	gio_put_noM( gio, 0, 2 );
	gio_put_noM( gio, 0, 2 );

	gio_put_noM( gio, 4, 2 );				// 4 channels	
	gio_put_noM( gio, psd->height, 4 );	
	gio_put_noM( gio, psd->width, 4 );	
	gio_put_noM( gio, 8, 2 );					// 8 bits depth

	gio_put_noM( gio, psd->mode, 2 );			// mode
//	gio_put_noM( gio, 3, 2 );					// mode
	//0-Bitmap, 1-Grayscale, 2-Indexed, 3-RGB, 4-CMYK, 7-Multichannel,
	//8-Duetone, 9-Lab




//--- Color mode data block
	gio_put_noM( gio, 0, 4 );	// empty block



//--- Image resources block
	gio_put_noM( gio, 28, 4 );	// empty block

	WriteImageResource_ESOLUTION( gio, psd->resolution );




//--- Layer and mask information block
	sizePos = gio_ftell( gio );
	gio_put_noM( gio, 0000, 4 );

	psd_write_layer_info_image_data( gio, psd );


	pos = gio_ftell( gio );
	size = pos - (sizePos + 4 );
	gio_seek( gio, sizePos );
	gio_put_noM( gio, size, 4 );
	gio_seek( gio, pos );


//--- Image data block
	psd_write_image_data( gio, psd );

	return 1;
}





static void
psd_write_layer_info_image_data( gio_type *gio, psd_type *psd )
{
int size, sizePos,	pos,	i;



    // Read layer and mask block.
	sizePos = gio_ftell( gio );
	gio_put_noM( gio, 0000, 4 );

    
	gio_put_noM( gio, psd->nLayer, 2 );


    for( i = 0 ; i < psd->nLayer ; i++)
		psd_write_layer_info( gio, psd->Layer[i] );


    // Read pixel data for each layer.
    for( i = 0 ; i < psd->nLayer ; i++){
		gio_put_noM( gio, 0, 2 );
		psd_write_image_data_channel( gio, psd->Layer[i]->image, 24, 1 );	// alpha channel

		gio_put_noM( gio, 0, 2 );
		psd_write_image_data_channel( gio, psd->Layer[i]->image, 16, 0 );	// red

		gio_put_noM( gio, 0, 2 );
		psd_write_image_data_channel( gio, psd->Layer[i]->image, 8, 0 );	// green

		gio_put_noM( gio, 0, 2 );
		psd_write_image_data_channel( gio, psd->Layer[i]->image, 0, 0 );	// blue
	}


    // without global layer mask info
	pos = gio_ftell( gio );
	size = pos - (sizePos + 4 );
	gio_seek( gio, sizePos );
	gio_put_noM( gio, size, 4 );
	gio_seek( gio, pos );
}



static void
psd_write_layer_info( gio_type *gio, psdLayer_type *psdL )
{
int sizeName;
int	imageDataSize;

	gio_put_noM( gio, psdL->y, 4 );
	gio_put_noM( gio, psdL->x, 4 );
	gio_put_noM( gio, psdL->y + psdL->image->row, 4 );
	gio_put_noM( gio, psdL->x + psdL->image->column, 4 );

	gio_put_noM( gio, 4, 2 );	// number of channels
 

	imageDataSize = 2 + psdL->image->row * psdL->image->column;
	gio_put_noM( gio, -1, 2 );		// alpaha channel
	gio_put_noM( gio, imageDataSize, 4 );

	gio_put_noM( gio, 0, 2 );
	gio_put_noM( gio, imageDataSize, 4 );	// red

	gio_put_noM( gio, 1, 2 );		// green
	gio_put_noM( gio, imageDataSize, 4 );	

	gio_put_noM( gio, 2, 2 );		// blue
	gio_put_noM( gio, imageDataSize, 4 );



	// mask
	gio_put_buffer( gio, "8BIM", 4 );
	gio_put_buffer( gio, "norm", 4 );
	gio_put_noM( gio, MaxRGB, 1 );	//opacity
	gio_put_noM( gio, 0, 1 );		// clipping
	gio_put_noM( gio, 26, 1 );		// flags
	gio_put_noM( gio, 0, 1 );		// padding



	// extrea
	sizeName = (int)strlen( psdL->name );
	sizeName = (int)strlen( psdL->name )/4*4 + 3;

    // without mask data and blending data 
	gio_put_noM( gio, 4 + 4 + sizeName+1, 4 );	// extrea size
	gio_put_noM( gio, 0, 4 );			// mask size 
	gio_put_noM( gio, 0, 4 );			// Blending Data size 

  
	gio_put_noM( gio, sizeName, 1 ); 
	gio_put_buffer( gio, psdL->name, sizeName );

		
//	layer_infoEx->matte		= psd_info->matte;
//    if ( psd_info->mode != CMYKMode)
//		layer_infoEx->matte	= layer_infoEx->channels >= 4;
}



static void
psd_write_image_data_channel( gio_type *gio, image_type *im, int shift, int inverse )
{

int	i,	j;
u_int	*tp,	pixel;

//	gio_put_noM( gio, 0, 2 );	//  compression = uncompress

	tp = (u_int *)im->data;

    for( i = 0; i < im->row ; i++ )
		for( j = 0; j < im->column; j++, tp++ ){

			pixel = (( *tp ) >> shift) & 0xFF;
			
			if( inverse )	pixel = 0xFF - pixel;

			gio_put_noM( gio, pixel, 1 );
		}

}



static void
psd_write_image_data( gio_type *gio, psd_type *psd )
{
int	i,	j;

	gio_put_noM( gio, 0, 2 );	//  compression = uncompress


	if( psd->image != NULL ){
		psd_write_image_data_channel( gio, psd->image, 16, 0 );	// red

		psd_write_image_data_channel( gio, psd->image, 8, 0 );	// green

		psd_write_image_data_channel( gio, psd->image, 0, 0 );	// blue

		psd_write_image_data_channel( gio, psd->image, 24, 0 );	// alpha channel

		return;
	}


    for( i = 0; i < psd->height ; i++ )
		for( j = 0; j < psd->width; j++ ){
			gio_put_noM( gio, 255, 1 );
			gio_put_noM( gio, 255, 1 );
			gio_put_noM( gio, 255, 1 );
			gio_put_noM( gio, 0, 1 );
		}

}


int
WriteImageResource_ESOLUTION( gio_type *gio, int resultion )
{
	gio_put_buffer( gio, "8BIM", 4 );

	gio_put_noM( gio, 0x03ed, 2 ); 

	
	// name
	gio_put_noM( gio, 0, 1 ); 
	gio_put_noM( gio, 0, 1 ); 


	// size
	gio_put_noM( gio, 16, 4 ); 

	gio_put_noM( gio, resultion*(1<<16), 4 ); 
	gio_put_noM( gio, 1, 2 ); 
	gio_put_noM( gio, 2, 2 ); 

	gio_put_noM( gio, resultion*(1<<16), 4 ); 
	gio_put_noM( gio, 1, 2 ); 
	gio_put_noM( gio, 2, 2 ); 

	return( 28 );
}
