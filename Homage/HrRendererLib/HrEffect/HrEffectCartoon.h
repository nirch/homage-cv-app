
#ifndef _HR_EFFECT_CARTOON_H
#define _HR_EFFECT_CARTOON_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectCartoon : public CHrEffectI
{
public:
	
	CHrEffectCartoon();

	virtual ~CHrEffectCartoon();

	int Init( int nColor, char *file );

    int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );

    int GetPose( int iFrame, vec2f_type *p, float *angle, float *scale );

	int	Close();

private:
    void DeleteContents();
	int m_width;
	int m_height;




	image_type *m_im;

	palette_type *m_palette;
	int	m_nColor;


};






#endif


