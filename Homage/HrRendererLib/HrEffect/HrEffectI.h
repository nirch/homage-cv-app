
#ifndef _HR_EFFECT_I_H
#define _HR_EFFECT_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#define EFFECT_MASK				1
#define EFFECT_POSE				2
#define EFFECT_ALIGNMENT		3

class CHrEffectI 
{
public:
	
	CHrEffectI();

	virtual ~CHrEffectI();

	void DeleteContents();

	virtual int Init( char *file )	{return( -1 ); }
	virtual int InitFromData( char *data )	{return( -1 ); }

	virtual int	Process( image_type *sim, int iFrame, image_type **im ) = 0;

	virtual int Close() = 0;

	void SetAlpha( image_type *im );

	void MergeAlpha( image_type *sim );

private:


};


image_type *	image3_green_alpha( image_type *sim, image_type *im );


#endif


