
#ifndef _HR_EFFECT_CARTOON_H
#define _HR_EFFECT_CARTOON_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectCortoon : public CHrEffectI
{
public:
	
	CHrEffectCortoon();

	virtual ~CHrEffectCortoon();

	void DeleteContents();



	int Init( int nColor, char *file );


	 int Process( image_type *sim, int iFrame, image_type **im );

	 int GetPose( int iFrame, vec2f_type *p, float *angle, float *scale );

	int	Close();

private:
	int m_width;
	int m_height;




	image_type *m_im;

	palette_type *m_palette;
	int	m_nColor;


};






#endif


