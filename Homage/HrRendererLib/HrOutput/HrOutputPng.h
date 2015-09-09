
#ifndef _HR_OUTPUT_PNG_H
#define _HR_OUTPUT_PNG_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrOutputI.h"





class CHrOutputPng : public CHrOutputI
{
public:
	
	CHrOutputPng();

	virtual ~CHrOutputPng();

	void DeleteContents();

	int Init( char *outFile, int width, int height, int frameSpeed );

	int SetPalette( char *data );

	virtual int	WriteFrame( image_type *im, int iFrame );

	int	Close();

private:

	int	m_iFrame;

	int	m_color;

	int m_delay;

	int	m_width;
	int	m_height;


	char	m_file[256];


	image_type *m_im;

	imageA_type *m_aim;


	class CPngWriter *m_recorder;
};


#endif


