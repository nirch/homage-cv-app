
#ifndef _HR_EFFECT_GRAY_H
#define _HR_EFFECT_GRAY_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectGray : public CHrEffectI
{
public:
	
	CHrEffectGray();

	virtual ~CHrEffectGray();

	void DeleteContents();



	int Init( char *file );


	 int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );

	 int GetPose( int iFrame, vec2f_type *p, float *angle, float *scale );

	int	Close();

private:
	int m_width;
	int m_height;




	image_type *m_im;


};






#endif


