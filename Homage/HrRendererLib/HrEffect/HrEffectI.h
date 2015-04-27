
#ifndef _HR_EFFECT_I_H
#define _HR_EFFECT_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"





class CHrEffectI 
{
public:
	
	CHrEffectI();

	virtual ~CHrEffectI();

	void DeleteContents();



	virtual int	Process( image_type *sim, int iFrame, image_type **im ) = 0;

	virtual int Close() = 0;

	void SetAlpha( image_type *im );

	void MergeAlpha( image_type *sim );

private:


};


image_type *	image3_green_alpha( image_type *sim, image_type *im );


#endif


