/************************
 *** BmpDecoder.cpp ***
 ***********************/

#include "BmpDecoder.h"


extern "C"
{
#include "ImageType/ImageType.h"
#include "ImageType/PaletteType.h"
#include "Uigp/igp.h"
#include "Ugio/GioType.h"

#include "ImageBmp.h"
}


CBmpDecoder::CBmpDecoder():CImageDecoder()
{
}


CBmpDecoder::~CBmpDecoder()
{
}



int CBmpDecoder::Decode(const char *filename, image_type **im)
{
	gio_type	*gio;
	
	if( (gio = gio_open_file_to_read( (char *)filename )) == NULL )	
		return( IMAGE_CODEC_COULD_NOT_OPEN_STREAM );


	m_image = image_read_bmp( gio );
	gio_close( gio );

	if ( m_image == NULL )
		return( IMAGE_CODEC_INTERNAL_ERROR );

	*im = m_image;

	return(1);
	
}



int CBmpDecoder::Decode(char *data, int bytes, image_type **im )
{
	gio_type	*gio;

	if( image_read_bmp_is_valid_SINGATURE( data, bytes ) < 0 )
		return( IMAGE_DECODER_INVALID_SINGATURE );
	
	if( (gio = gio_open_buffer_to_read( data, bytes, 0 )) == NULL )	
		return( IMAGE_CODEC_COULD_NOT_OPEN_STREAM );


	m_image = image_read_bmp( gio );
	gio_close( gio );

	if ( m_image == NULL )
		return( IMAGE_CODEC_INTERNAL_ERROR );

	*im = m_image;
	return(1);
	
}

