/**************************
 ***   ImageEncoder.h   ***
 **************************/
#include	<string.h>


#include "ImageCodec.h"


extern "C"
{
#include "ImageType/ImageType.h"
#include "ImageType/PaletteType.h"
}


CImageEncoder::CImageEncoder()
{
	m_palette = NULL;
	m_iWidth = m_iHeight = 0;
	m_iBitsPerPixel = 0;
	m_iColorSpace = 0;
	m_iQuality = 0;
	m_iErrCode = 0;
	m_dataSize = 0;

	m_data = NULL;


	m_vimatixData = NULL;
}


CImageEncoder::~CImageEncoder()
{
	if ( m_palette != NULL)
		palette_destroy(m_palette);	

	if( m_data != NULL )
		free( m_data );

	if( m_vimatixData != NULL ){
		free( m_vimatixData );
		m_vimatixData = NULL;
	}
}


void CImageEncoder::SetVimatixData( char *data )
{
	if( m_vimatixData != NULL )
		free( m_vimatixData );

	if ( !data || !data[0] )
		return;

	m_vimatixData = (char *)malloc( strlen(data) + 5 );
	strcpy( m_vimatixData, HEADER_VIMATIX_DATA );
	strcat( m_vimatixData, data );
	//m_vimatixData = strdup( data );
}


void CImageEncoder::SetParams(int *bitsPerPixel, int *colorSpace, int quality ) 
{ 
	m_iQuality = quality;
	m_iBitsPerPixel = *bitsPerPixel; 
	m_iColorSpace = *colorSpace; 
}

void CImageEncoder::SetQuality( int quality )
{ 	
	m_iQuality = quality; 
}

void CImageEncoder::SetBitsPerPixel( int bitsPerPixel )
{ 
	m_iBitsPerPixel = bitsPerPixel; 
}

void CImageEncoder::SetColorSpace( int colorSpace )
{ 	
	m_iColorSpace = colorSpace; 
}

void CImageEncoder::SetDither( int dither )
{ 	
	m_iDither = dither; 
}	

void CImageEncoder::SetPalette(struct palette_type * palette)
{
	if ( m_palette != NULL ) 
		palette_destroy(m_palette);
	m_palette = palette_copy(palette, NULL );
}

int CImageEncoder::ErrorCode()
{
	return m_iErrCode;
}


int CImageEncoder::BytesWritten()
{
	return m_dataSize;
}


/*
int CImageEncoder::Encode( struct image_type *im,
							u_char **data, int *bytes )
{
int	err;

	err = Encode( im, data );

	*bytes = m_dataSize;

	return( err );
}
*/


int CImageEncoder::Encode( struct image_type *im,
							u_char **data, int *bytes, int Fdetach )
{
int	err;

	if( m_data != NULL )
		free( m_data );

	err = Encode( im, &m_data );

	*data = m_data;

	*bytes = m_dataSize;

	if( Fdetach == 1 )
		m_data = NULL;

	return( err );
}



palette_type * 
CImageEncoder::CreatePalette( image_type *im[], int numImages, 
								int paletteType, int bitsPerPixel )
{
palette_type *palette;

	switch( paletteType ){
	case PALETTE_BW:
	case PALETTE_WB:

		palette = palette_create( paletteType );		
		break;

	case PALETTE_GRAY:

		palette = palette_create_gray( bitsPerPixel );

		break;

	case PALETTE_HALFTONE:

		palette = palette_create_HALFTONE();
		break;

	case PALETTE_222:

		palette = palette_create_222();
		break;

	case PALETTE_WINDOW_SYSTEM:
		palette = palette_create_WINDOW_SYSTEM();
		break;


	case PALETTE_ADAPTIVE:
	default:
		
		palette = palette_alloc( 1<<bitsPerPixel );
		palette->nColor = 0;
		image_adaptive_palette( im, numImages, palette, 1<<bitsPerPixel );
	break;
	}


	return palette;
}