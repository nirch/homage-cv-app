
#ifndef _HR_EFFECT_ALPHA_H
#define _HR_EFFECT_ALPHA_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectAlpha : public CHrEffectI
{
public:
	
	CHrEffectAlpha();

	virtual ~CHrEffectAlpha();

	void DeleteContents();


	int Init( char *file );
    int InitFromData( char *data );


	 int Process( image_type *sim, int iFrame, image_type **im );


	 int GetPose( tfA_type *atf, int iFrame, float *a );

	int	Close();

private:
	int m_width;
	int m_height;





	image_type *m_im;


	struct tfA_type *m_atf;

};






#endif


