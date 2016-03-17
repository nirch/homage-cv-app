
#ifndef _HR_EFFECT_MORPHING_H
#define _HR_EFFECT_MORPHING_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "../HrEffectI.h"



class CHrEffectMorphing : public CHrEffectI
{
public:
	
	CHrEffectMorphing();

	~CHrEffectMorphing();

	//int Init( char *splFile,  char *tFile, char *plnFile, char *tfFile );
    int Init( char *splFile,  image_type *tim, char *plnFile, char *tfFile );
	int InitFromData( char *data );


	 int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );

	int	GetPose( tfA_type *atf, int iFrame, float *T, float *a );

	int	Close();

private:
    void DeleteContents();

	int m_width;
	int m_height;

	int m_nFrame;

	int	m_iFrame;

	struct tfA_type *m_atf;


	image_type *m_im;


	image_type *m_sim;
	plnA_type *m_apl;

	image_type *m_imT;
	plnA_type *m_aplT;
	

	pt2dA_type	*m_apt;
	pt2dA_type	*m_aptT;


	vec2f_type	m_p0;
	vec2f_type	m_dp0;


	image_type *m_mbt;
	image_type *m_mbtI;
};






#endif


