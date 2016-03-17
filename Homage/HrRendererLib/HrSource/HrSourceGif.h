
#ifndef _HR_SOURCE_GIF_H
#define _HR_SOURCE_GIF_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrSourceI.h"



class CHrSourceGif : public CHrSourceI
{
public:
	CHrSourceGif();
	virtual ~CHrSourceGif();
	int Init( char *inFile );
    int	ReadFrame( int iFrame, long long timeStamp, image_type **im );
	int	Close();
private:
    void DeleteContents();
	//int m_width;
	//int m_height;
	int m_nFrame;
	//int	m_iFrame;
	struct gifIo_type	*m_gifIo;
	image_type *m_im;
};






#endif


