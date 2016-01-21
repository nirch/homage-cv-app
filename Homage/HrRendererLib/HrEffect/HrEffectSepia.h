
#ifndef _HR_EFFECT_SEPIA_H
#define _HR_EFFECT_SEPIA_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectSepia : public CHrEffectI
{
public:
	
	CHrEffectSepia();

	virtual ~CHrEffectSepia();

	void DeleteContents();



	int Init( float fR, float fG, float fB, char *file );


	 int Process( image_type *sim, int iFrame, image_type **im );

	 int GetPose( int iFrame, vec2f_type *p, float *angle, float *scale );

	int	Close();

private:
	int m_width;
	int m_height;

	float	m_fR;
	float	m_fG;
	float	m_fB;



	image_type *m_im;


};






#endif


