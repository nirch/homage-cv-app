
#ifndef _HR_EFFECT_POSE_H
#define _HR_EFFECT_POSE_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectPose : public CHrEffectI
{
public:
	
	CHrEffectPose();

	~CHrEffectPose();

	void DeleteContents();



	int Init( char *inFile );
	int InitFromData( char *data );

	int SetFrameSize( int width, int height );

	 int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );

	 int GetPose( int iFrame, vec2f_type *p, float *scale, float *rx, float *ry, float *rz );

	int	Close();

private:
	int m_width;
	int m_height;

	int m_nFrame;

	int	m_iFrame;

	struct tfA_type *m_atf;


	image_type *m_im;


};






#endif


