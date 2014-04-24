#include "GifEncoder.h"


extern "C"
{
#include "ImageType/ImageType.h"
#include "ImageType/PaletteType.h"
#include "Uigp/igp.h"
#include "Ugio/GioType.h"

#include "ImageReadGif.h"
#include "ImageWriteGif.h"
}



// ======================= ENCODER =============================

CGifEncoder::CGifEncoder():CImageEncoder()
{

}
CGifEncoder::~CGifEncoder()
{

}


void CGifEncoder::SetParams(int *bitsPerPixel, int *colorSpace, int quality )
{

	m_iQuality = quality;

	m_iColorSpace = *colorSpace;
	if( !PALETTE_IS_VALID(m_iColorSpace) ||  m_iColorSpace == PALETTE_RGB )
		m_iColorSpace = PALETTE_ADAPTIVE;


	m_iBitsPerPixel = *bitsPerPixel;

	if( m_iBitsPerPixel >= 8 || m_iBitsPerPixel <= 0 )
		m_iBitsPerPixel =  8;

	if( m_iBitsPerPixel == 8 )
		m_iQuality = 10;


	*bitsPerPixel = m_iBitsPerPixel;
	*colorSpace = m_iColorSpace;
}

void CGifEncoder::SetQuality( int quality )
{
static int bitPerPixel[] = { 0, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8 };

	if( quality <= 0 )	quality = 1;
	if( quality > 10 )	quality = 10;


	m_iBitsPerPixel = bitPerPixel[quality];
	m_iQuality = quality;

	if( m_palette != NULL ){
		palette_destroy( m_palette );
		m_palette = NULL;
	}
}



int CGifEncoder::Encode(struct image_type *im,const char *path) 
{	
char	file[256];
int	ret;
gifIo_type	*gifIo;

	m_dataSize = 0;

	// create palette
	if ( m_palette == NULL ) {
		m_palette = CreatePalette(m_iBitsPerPixel, &im, 1, m_iColorSpace);

		if (m_palette == NULL ) {
			m_iErrCode = IMAGE_CODEC_NOT_ENOUGH_PARAMETERS_FOR_ENCODING;// no palette
			return m_iErrCode;
		}
	}



	gpFilename_force_extension( (char *)path, ".gif", file );



	gifIo = image_write_gifIo_open_file( file, IMAGE_ROW(im), IMAGE_COLUMN(im),
							10, m_palette, -1, m_iDither, 0 );


	if( gifIo == NULL ){
		m_iErrCode = IMAGE_CODEC_COULD_NOT_OPEN_STREAM;
		return m_iErrCode;
	}


	if ( m_vimatixData && m_vimatixData[0] )
		image_write_gif_VimatixData( gifIo->fp, (const u_char*)m_vimatixData );



	ret = image_write_gifIo_add_frame( gifIo, im, 0 );

	m_dataSize = image_write_gifIo_close( gifIo );

/*
	fp = image_write_gif_open(file, IMAGE_ROW(im), IMAGE_COLUMN(im), m_palette );

	if( fp == NULL )	
		ret = -1;

	if ( m_vimatixData && m_vimatixData[0] )
		image_write_gif_VimatixData( fp, (const u_char*)m_vimatixData );

	if (image_write_gif_add_frame(fp, NULL, im, m_palette, 0, 0, m_iDither ) > 0 )	
		ret = image_write_gif_close( fp );
	else
		ret = -2;
*/


	if( ret > 0 ){
		m_iErrCode = 0;
		return 1 ;
	}

	m_iErrCode = IMAGE_CODEC_ERROR_WHILE_WRITING_OUT;
	return m_iErrCode;
/*
	switch (ret)
	{ 
	case -2:
		m_iErrCode = IMAGE_CODEC_ERROR_WHILE_WRITING_OUT;
		return m_iErrCode;
	default:
		m_iErrCode = IMAGE_CODEC_INTERNAL_ERROR;
		return m_iErrCode;		
	}
*/
}



/*********  CGifEncoder::Encode  **********
* @Description	: writes the image into the buffer
* 
* @Param  struct image_type* im :  image to write
* @Param  unsigned char**pTargetBuffer :  pointer to a buffer to allocate
* @Param  int quality :  ignored
* @Return  int  :  actual length of the buffer
*****************************************/
int CGifEncoder::Encode(struct image_type* im, unsigned char** data) 
{
char tmpFile[256];
int	bytes;

	*data = NULL;
	gp_filename_get_temporary( "gif", 0, ".gif", tmpFile );

	int ret = Encode(im,tmpFile);
	if ( ret > 0 )
//		gp_read_buffer_from_file(tmpFile, pTargetBuffer);
		gpFile_read_to_buffer( tmpFile, (char**)data, &bytes );

	gpFile_delete(tmpFile);
	return bytes;

}





bool CGifEncoder::CanEncode(const char *filename)
{ 
	return true;
};

bool CGifEncoder::CanEncode(char* data, int bytes, int bCopyData)
{
	return true;
};





palette_type* CGifEncoder::CreatePalette(int bitsPerPixel, struct image_type *im[], int numImages, int paletteType)
{


	switch( paletteType )
	{
	case PALETTE_BW:
	case PALETTE_WB:

		m_palette = palette_create( paletteType );		
		break;

	case PALETTE_GRAY:

		m_palette = palette_create_gray( bitsPerPixel );

		break;

	case PALETTE_HALFTONE:

		m_palette = palette_create_HALFTONE();
		break;

	case PALETTE_222:

		m_palette = palette_create_222();
		break;

	case PALETTE_WINDOW_SYSTEM:
		m_palette = palette_create_WINDOW_SYSTEM();
		break;

	case PALETTE_RGB:
	case PALETTE_SMART:
	default:
		
		m_palette = palette_alloc( 1<<bitsPerPixel );
		m_palette->nColor = 0;
		image_adaptive_palette( im, 1, m_palette, 1<<bitsPerPixel );
	break;


	}
	return m_palette;
}