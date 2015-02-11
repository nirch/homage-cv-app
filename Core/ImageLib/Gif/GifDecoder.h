#ifndef _GIF_DECODER_H_
#define _GIF_DECODER_H_

#include "../Codecs/ImageDecoder.h"



class CGifDecoder :public CImageDecoder
{
public:
	CGifDecoder();
	virtual ~CGifDecoder();
	
//	virtual int GetMIMEType( char *mime, int mimeLen );
//	virtual int GetExtension( char *extension, int extensionLen );
		

	virtual int Decode(const char *filename, image_type **im);
	virtual int Decode(char *data, int bytes, image_type **im );
};


#endif 
