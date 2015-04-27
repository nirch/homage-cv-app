
#ifndef _HR_SOURCE_GIF_H
#define _HR_SOURCE_GIF_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrSourceI.h"



class CHrSourceLive : public CHrSourceI
{
public:
	
	CHrSourceLive();

	~CHrSourceLive();

	void DeleteContents();



	int Init( int nImage, char *file );

	int	AddFrame( image_type *sim, int iFrame );

	 int	ReadFrame( int iFrame, image_type **im );

	int	Close();

private:
	int m_width;
	int m_height;

	int m_nFrame;

	int	m_iFrame;



	class CVConditionMutex *m_mutex;

	int		m_iImage;
	imageA_type *m_aim;

	image_type	*m_im;


};



#endif


