
#ifndef _HR_SOURCE_GREEN_WIN_H
#define _HR_SOURCE_GREEN_WIN_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrSourceI.h"



class CHrSourceGreenWin : public CHrSourceI
{
public:
	
	CHrSourceGreenWin();

	~CHrSourceGreenWin();

	void DeleteContents();

	void SetColor( int color )	{ m_color = color; }


	int Init( char *inFile );

	 int	ReadFrame( int iFrame, image_type **im );

	int	Close();

private:

//	int	m_iFrame;

	int	m_color;

	class CVideoDecoder	*m_videoDecoder;

	image_type *m_im;


};



#endif


