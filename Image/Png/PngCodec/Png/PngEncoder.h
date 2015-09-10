#ifndef __PNG_ENCODER_H__
#define __PNG_ENCODER_H__

#include "Codecs/ImageCodec.h"


extern "C"
{
	struct image_type;
	struct palette_type;
}



class CPNGEncoder :public CImageEncoder
{
public:
	CPNGEncoder();
	virtual ~CPNGEncoder();

	void SetParams(int *bitsPerPixel, int *colorSpace, int quality );
	void SetQuality( int quality );

	virtual bool CanEncode(const char *filename);
	virtual bool CanEncode(char* data, int bytes, int bCopyData);

	virtual int Encode(struct image_type *im,const char *path) ;
	virtual int Encode(struct image_type* im,unsigned char**pTargetBuffer) ;


	int	Encode(struct image_type *im, u_char **data, int *bytes );
};


#endif 
