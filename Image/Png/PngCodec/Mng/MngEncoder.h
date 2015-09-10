#ifndef __MNG_ENCODER_H__
#define __MNG_ENCODER_H__

#include "Codecs/ImageCodec.h"

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PngCodecD.lib" )
#else
#pragma comment( lib, "PngCodec.lib" )
#endif

extern "C"
{
	struct image_type;
	struct palette_type;
}



class CMNGEncoder :public CImageEncoder
{
public:
	CMNGEncoder();
	virtual ~CMNGEncoder();

	void SetParams(int *bitsPerPixel, int *colorSpace, int quality );
	void SetQuality( int quality );

	virtual bool CanEncode(const char *filename);
	virtual bool CanEncode(char* data, int bytes, int bCopyData);

	virtual int Encode(struct image_type *im,const char *path) ;
	virtual int Encode(struct image_type* im,unsigned char**pTargetBuffer) ;


	int	Encode(struct image_type *im, u_char **data, int *bytes );
};


#endif 
