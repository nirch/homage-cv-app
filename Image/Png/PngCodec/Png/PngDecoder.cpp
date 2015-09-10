#include	<string.h>

#include "PngDecoder.h"


extern "C"
{
#include "ImageType/ImageType.h"
#include "ImageWritePng.h"
#include "ImageReadPng.h"
#include "ImageType/PaletteType.h"
#include "Uigp/igp.h"
#include "Ugio/GioType.h"
}


CPngDecoder::CPngDecoder():CImageDecoder()
{

}
CPngDecoder::~CPngDecoder()
{

}
	

/*
bool CPngDecoder::CanDecode(const char *filename)
{
	FILE *fp;
	fp = fopen(filename,"rb");
	if ( fp == NULL ) 
		return false;
	u_char buf[10];
	buf[0] = 0;
	fread(buf,1,10,fp);
	fclose(fp);

	if (memcmp(buf,"\211PNG\r\n\032\n",8) == 0)
		return true;
	return false;
}



bool CPngDecoder::CanDecode(char* data, int bytes, int bCopyData)
{
	if (memcmp(data,"\211PNG\r\n\032\n",8) == 0)
		return true;

	return false;
}
*/




int
CPngDecoder::Decode(const char *filename, image_type **im)
{	
	if( (m_image = image_read_png_file((char*)filename)) == NULL )
		return( IMAGE_CODEC_CORRUPTED_INPUT_STREAM );

	*im = m_image;

	return( 1);	
}


int CPngDecoder::Decode(char *data, int bytes, image_type **im )
{
char filename[256];

	if( image_read_png_is_valid_SINGATURE( data, bytes ) < 0 )
		return( IMAGE_DECODER_INVALID_SINGATURE );


	gp_filename_get_temporary("png",0,".png",filename);
	gpFile_write_buffer(filename,data,bytes);

	m_image = image_read_png_file((char*)filename);
	unlink(filename);

	if( m_image == NULL ) 
		return( IMAGE_CODEC_CORRUPTED_INPUT_STREAM );

	*im = m_image;

	return( 1 );

}
