/************************
 *** GifDecoder.cpp ***
 ***********************/

#include "GifDecoder.h"
extern "C"
{
#include "ImageType/ImageType.h"
#include "Uigp/igp.h"
#include "Ugio/GioType.h"

#include "ImageReadGif.h"
#include "ImageWriteGif.h"
}


CGifDecoder::CGifDecoder():CImageDecoder()
{

}


CGifDecoder::~CGifDecoder()
{

}
	


int CGifDecoder::Decode(const char *filename, image_type **im) 
{
char pVimData[1024] = "";	

	if( (m_image = image_read_gif_file_VimatixData((char*)filename, (u_char *)pVimData)) == NULL )
		return( IMAGE_CODEC_CORRUPTED_INPUT_STREAM );
	
	*im = m_image;

	if ( pVimData[0] )
		SetVimatixData( pVimData );

	return( 1);
}


int CGifDecoder::Decode(char *data, int bytes, image_type **im )
{
 char pVimData[1024] = "";	


	if( image_read_gif_is_valid_SINGATURE( data, bytes ) < 0 )
		return( IMAGE_DECODER_INVALID_SINGATURE );


	if( (m_image = image_read_gif_buffer_VimatixData(data, bytes, 0, (u_char *)pVimData )) == NULL )
		return( IMAGE_CODEC_CORRUPTED_INPUT_STREAM );
	
	*im = m_image;

	if ( pVimData[0] )
		SetVimatixData( pVimData );

	return( 1);
}





/*
int CGifDecoder::Decode(const char *filename, image_type **im) 
{
	
	
	if( (m_image = image_read_gif_file((char*)filename)) == NULL )
		return( IMAGE_CODEC_CORRUPTED_INPUT_STREAM );
	
	*im = m_image;

	return( 1);
	
}


int CGifDecoder::Decode(char *data, int bytes, image_type **im )
{
	if( (m_image = image_read_gif_buffer(data, bytes, 0 )) == NULL )
		return( IMAGE_CODEC_CORRUPTED_INPUT_STREAM );
	
	*im = m_image;
	return( 1);
}
*/