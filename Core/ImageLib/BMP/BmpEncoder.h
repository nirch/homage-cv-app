#ifndef __BMP_CODEC_H__
#define __BMP_CODEC_H__
#include "../Codecs/ImageCodec.h"
extern "C"
{
	struct image_type;
	struct palette_type;
}


/*
class CBMPDecoder :public CImageDecoder
{
public:
	CBMPDecoder();
	virtual ~CBMPDecoder();
	
	virtual bool CanDecode(const char *filename);
	virtual bool CanDecode(char* data, int bytes, int bCopyData);

	virtual image_type *Decode(const char *filename);
	virtual image_type *Decode(char* data, int bytes, int bCopyData);
};
*/

class CBMPEncoder :public CImageEncoder
{
public:
	CBMPEncoder();
	virtual ~CBMPEncoder();

	virtual bool CanEncode(const char *filename);
	virtual bool CanEncode(char* data, int bytes, int bCopyData);

	virtual int Encode(struct image_type *im,const char *path) ;
	virtual int Encode(struct image_type* im,unsigned char**pTargetBuffer) ;


};


#endif //__BMP_CODEC_H__
