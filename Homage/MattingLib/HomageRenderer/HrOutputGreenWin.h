
#ifndef _HR_OUTPUT_GREEN_WIN_H
#define _HR_OUTPUT_GREEN_WIN_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrOutputI.h"

#include "Avi/Avi/AviCodec.h"



class CHrOutputGreenWin : public CHrOutputI
{
public:
	
	CHrOutputGreenWin();

	~CHrOutputGreenWin();

	void DeleteContents();

	void SetColor( int color )	{ m_color = color; }

	void Set( int codec = AVI_CODEC_INDEO50 , int frameRate = 10, int bitrate = 10000 );

	int Init( char *outFile, int width, int height );

	virtual int	WriteFrame( image_type *im, int iFrame );

	int	Close();

private:

	int	m_iFrame;

	int m_color;

	int	m_width;
	int	m_height;
	int	m_codec;
	int	m_frameRate;
	int	m_bitrate;

	class CVideoEncoder *m_videoEncoder;


	image_type *m_im;
};


#endif


