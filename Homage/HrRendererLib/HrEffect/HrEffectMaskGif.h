
#ifndef _HR_EFFECT_MASK_GIF_H
#define _HR_EFFECT_MASK_GIF_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectMaskGif : public CHrEffectI
{
public:
	
	CHrEffectMaskGif();

	virtual ~CHrEffectMaskGif();

	int Init( image_type *im );

	int Init( char *file );

    int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );

    int GetPose( int iFrame, vec2f_type *p, float *angle, float *scale );

	int	Close();

private:
    void DeleteContents();

	int m_width;
	int m_height;

	int m_nFrame;

	int	m_iFrame;

	struct gifIo_type	*m_gifIo;




	image_type *m_alphaIm;

	image_type *m_im;


};






#endif


