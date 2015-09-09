#ifndef __IMAGE_CODEC_H__
#define __IMAGE_CODEC_H__

extern "C"
{
#include	"Uigp/igp.h"

struct image_type;
struct palette_type;
}

#define IMAGE_CODEC_INTERNAL_ERROR				 (-100)
#define IMAGE_CODEC_UNSUPPORTED_FORMAT			(-101)
#define IMAGE_CODEC_COULD_NOT_OPEN_STREAM	 (-102)
#define IMAGE_CODEC_CORRUPTED_INPUT_STREAM		 (-103)
#define IMAGE_CODEC_NO_INPUT_STREAM_DETECTED	 (-104)
#define IMAGE_CODEC_ERROR_WHILE_WRITING_OUT		 (-105)
#define IMAGE_CODEC_NOT_ENOUGH_PARAMETERS_FOR_ENCODING (-106)


#define HEADER_VIMATIX_DATA		"@vim"




class CImageEncoder
{
public:
	CImageEncoder();
	virtual ~CImageEncoder();
	virtual bool CanEncode(const char *filename) = 0;
	virtual bool CanEncode(char* data, int bytes, int bCopyData) = 0;	
	
	virtual int Encode(struct image_type *im, const char *path) = 0;
	virtual int Encode(struct image_type *im, u_char **data) = 0;

	virtual int Encode(struct image_type *im, u_char **data, int *bytes, int Fdetach );

	virtual void SetParams( int *bitsPerPixel, int *colorSpace, int quality );

	virtual void SetQuality( int quality );

	virtual void SetBitsPerPixel( int bitsPerPixel );

	virtual void SetColorSpace( int colorSpace );

	virtual void SetDither( int dither );

	virtual void SetPalette(struct palette_type * palette);


	palette_type * CreatePalette( image_type *im[], int numImages, 
								int paletteType, int bitsPerPixel );


	int GetQuality( )	{ return m_iQuality; }
	int GetBitsPerPixel( )	{ return m_iBitsPerPixel; }
	int GetColorSpace( )	{ return m_iColorSpace; }
	int GetDither( )	{ return m_iDither; }

	int ErrorCode();
	int BytesWritten();

	virtual void SetVimatixData( char *data );

protected:
	int m_iErrCode;
	int m_iWidth, m_iHeight;
	int m_iBitsPerPixel,	m_iColorSpace,	m_iDither, m_iQuality;
	
	struct palette_type * m_palette;

	u_char	*m_data;
	int		m_dataSize;

	char	*m_vimatixData;
};





#endif
