#ifndef __IMAGE_DECODER_H__
#define __IMAGE_DECODER_H__

extern "C"
{
#include	"Uigp/igp.h"

struct image_type;
struct palette_type;
}


#define IMAGE_DECODER_INTERNAL_ERROR				 (-100)
#define IMAGE_DECODER_COULD_NOT_OPEN_STREAM			 (-101)
#define IMAGE_DECODER_INVALID_SINGATURE				 (-102)
#define IMAGE_DECODER_CORRUPTED_INPUT_STREAM		 (-103)
#define IMAGE_DECODER_NO_INPUT_STREAM_DETECTED		 (-104)
#define IMAGE_DECODER_ERROR_WHILE_WRITING_OUT		 (-105)

#include	"ImageCodec.h"



class CImageDecoder
{
public:
	CImageDecoder();
	virtual ~CImageDecoder();	

//	virtual int GetMIMEType( char *mime, int mimeLen ) = 0;
//	virtual int GetExtension( char *extension, int extensionLen ) = 0;
		

	virtual int Decode(const char *filename, image_type **im) = 0;
	virtual int Decode(char *data, int bytes, image_type **im ) = 0;


public:
	image_type	*m_image;


	virtual char *GetVimatixData()	{ return( m_vimatixData ); }



protected:
	void	SetVimatixData(  char *data );
	char	*m_vimatixData;
};




#endif
