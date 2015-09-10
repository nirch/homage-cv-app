/************************
 *** MngEncoder.cpp   ***
 ************************/

#include	<string.h>

#include "MngEncoder.h"


extern "C"
{
#include "ImageType/ImageType.h"
#include "ImageWriteMng.h"
#include "ImageType/PaletteType.h"
#include "Uigp/igp.h"
#include "Ugio/GioType.h"
}


CMNGEncoder::CMNGEncoder():CImageEncoder()
{

}


CMNGEncoder::~CMNGEncoder()
{

}



void CMNGEncoder::SetParams(int *bitsPerPixel, int *colorSpace, int quality )
{

	m_iQuality = quality;

	m_iColorSpace = *colorSpace;
	if( !PALETTE_IS_VALID(m_iColorSpace) )
		m_iColorSpace = PALETTE_RGB;


	m_iBitsPerPixel = *bitsPerPixel;

	if( m_iColorSpace == PALETTE_RGB )
		m_iBitsPerPixel = 24;

	if( m_iColorSpace != PALETTE_RGB ){
		if( m_iBitsPerPixel <= 0 || m_iBitsPerPixel > 8 )
			m_iBitsPerPixel =  8;
	}



	if( m_iBitsPerPixel == 8 )
		m_iQuality = 10;

	*bitsPerPixel = m_iBitsPerPixel;
	*colorSpace = m_iColorSpace;
}


void CMNGEncoder::SetQuality( int quality )
{
static int bitPerPixel[] = { 0, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8 };

	if( quality <= 0 )	quality = 1;
	if( quality > 10 )	quality = 10;

	
	m_iBitsPerPixel = bitPerPixel[quality];

	if( m_iColorSpace == PALETTE_RGB )
		m_iColorSpace = PALETTE_ADAPTIVE;

	m_iQuality = quality;
}


int CMNGEncoder::Encode( image_type *im, const char *fileName ) 
{
image_type	*im8;
char	file[512];
int	ret;

	strcpy( file, fileName );
	gp_filename_force_extension( file, ".mng" );


	if( m_iColorSpace != PALETTE_RGB ){
		if( m_palette == NULL )
			m_palette = CreatePalette( &im, 1, m_iColorSpace, m_iBitsPerPixel );

		int dither = 0;
		if( dither != 0 ){
				im8 = image_24to8_dithering_floyd( im,  m_palette, NULL );
		}
		else	im8 = image_24to8( im,  m_palette, NULL );


		ret = image_write_mng( im8, m_palette, file );

		image_destroy( im8, 1 );
	}
	else
		ret = image_write_mng( im, NULL, file );


//	if( (ret = image_write_mng( im,file) ) > 0 ){
	if( ret > 0 ){
		m_dataSize = ret;
		m_iErrCode = 1;
		return m_iErrCode;
	}




	m_dataSize = 0;
	switch (ret){ 	
	case -1:
		m_iErrCode = IMAGE_CODEC_COULD_NOT_OPEN_STREAM;
		return m_iErrCode;
	case -2:
		m_iErrCode = IMAGE_CODEC_ERROR_WHILE_WRITING_OUT;
		return m_iErrCode;
	case -3:
	default:
		m_iErrCode = IMAGE_CODEC_INTERNAL_ERROR;
		return m_iErrCode;		
	}		
}



int CMNGEncoder::Encode(struct image_type *im, u_char **data, int *bytes ) 
{
char tmpFile[256];

	gp_filename_get_temporary( "mng", 0, ".mng", tmpFile );	

	int ret = Encode( im, tmpFile );

	if ( ret > 0 )
		gpFile_read_to_buffer( tmpFile, (char**)data, bytes );

	unlink(tmpFile);

	return ret;
}


int CMNGEncoder::Encode(struct image_type* im, unsigned char **data ) 
{
char tmpFile[256];
int	bytes;

	*data = NULL;
	gp_filename_get_temporary( "mng", 0, ".mng", tmpFile );	

	int ret = Encode(im, tmpFile);

	if ( ret > 0 )
		gpFile_read_to_buffer( tmpFile, (char**)data, &bytes );

	unlink(tmpFile);
	return bytes ;
}




bool CMNGEncoder::CanEncode(const char *filename)
{ 
	return true;
};

bool CMNGEncoder::CanEncode(char* data, int bytes, int bCopyData)
{
	return true;
};





