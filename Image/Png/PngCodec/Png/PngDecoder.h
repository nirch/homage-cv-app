#ifndef _PNG_DECODER_H_
#define _PNG_DECODER_H_

#include "Codecs/ImageDecoder.h"



class CPngDecoder :public CImageDecoder
{
public:
	CPngDecoder();
	virtual ~CPngDecoder();
	
//	virtual int GetMIMEType( char *mime, int mimeLen );
//	virtual int GetExtension( char *extension, int extensionLen );
		

	virtual int Decode(const char *filename, image_type **im);
	virtual int Decode(char *data, int bytes, image_type **im );
};


#endif
