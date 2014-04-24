#ifndef _BMP_DECODER_H_
#define _BMP_DECODER_H_

#include "../Codecs/ImageDecoder.h"



class CBmpDecoder :public CImageDecoder
{
public:
	CBmpDecoder();
	virtual ~CBmpDecoder();
	
//	virtual int GetMIMEType( char *mime, int mimeLen );
//	virtual int GetExtension( char *extension, int extensionLen );
		

	virtual int Decode(const char *filename, image_type **im);
	virtual int Decode(char *data, int bytes, image_type **im );
};


#endif


