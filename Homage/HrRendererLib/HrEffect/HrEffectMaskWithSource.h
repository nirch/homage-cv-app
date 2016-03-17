
#ifndef _HR_EFFECT_DYNAMIC_MASK_H
#define _HR_EFFECT_DYNAMIC_MASK_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"
#include "../HrSource/HrSourceI.h"

class CHrEffectMaskWithSource : public CHrEffectI
{
public:
	CHrEffectMaskWithSource();
    ~CHrEffectMaskWithSource();
    
    int InitWithSource( CHrSourceI *source );

    void DeleteContents();
    
    int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );
	int	Close();

private:
    CHrSourceI *source;
    image_type *m_mask_owned_by_source;
    image_type *m_alpha;
	image_type *m_im;
};






#endif


