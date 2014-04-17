#ifndef __GIF_CODEC_H__
#define __GIF_CODEC_H__
#include "../Codecs/ImageCodec.h"
extern "C"
{
	struct image_type;
	struct palette_type;
}




class CGifEncoder :public CImageEncoder
{
public:
	CGifEncoder();
	virtual ~CGifEncoder();

	virtual bool CanEncode(const char *filename);
	virtual bool CanEncode(char* data, int bytes, int bCopyData);

	virtual int Encode(struct image_type *im,const char *path) ;
	virtual int Encode(struct image_type* im,unsigned char**pTargetBuffer) ;
	
	virtual void SetParams( int *bitsPerPixel, int *colorSpace, int quality );

	void SetQuality( int quality );

	struct palette_type *CreatePalette(int bitsPerPixel, struct image_type *im[], int numImages,  int paletteType);

};


/*
different palette types for GIF:
_________________________________
|MACRO					| value	|
|-----------------------|-------|
|PALETTE_BW				|	1	|
|PALETTE_GRAY			|	3	|
|PALETTE_SMART			|	4	|
|PALETTE_WINDOW_SYSTEM	|	5	|
!===============================!
*/

#endif //__GIF_CODEC_H__
