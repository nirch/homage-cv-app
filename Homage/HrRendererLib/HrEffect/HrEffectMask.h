
#ifndef _HR_EFFECT_MASK_H
#define _HR_EFFECT_MASK_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectMask : public CHrEffectI
{
public:
	
	CHrEffectMask();

	virtual ~CHrEffectMask();

	void DeleteContents();



	int Init( image_type *im );

	int Init( char *file );


	 int Process( image_type *sim, int iFrame, image_type **im );

	 int GetPose( int iFrame, vec2f_type *p, float *angle, float *scale );

	int	Close();

private:
	int m_width;
	int m_height;



	

	image_type *m_alphaIm;

	image_type *m_im;


};






#endif


