
#ifndef _HR_EFFECT_I_H
#define _HR_EFFECT_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#define EFFECT_MASK				1
#define EFFECT_POSE				2
#define EFFECT_ALIGNMENT		3
#define EFFECT_MASKGIF			4
#define EFFECT_MORPHING			5
#define EFFECT_SEPIA			6
#define EFFECT_GRAY				7
#define EFFECT_ALPHA			8
#define EFFECT_CARTOON			9
#define EFFECT_MASK_SOURCE		10
class CHrEffectI 
{
public:
	
	CHrEffectI();

	virtual ~CHrEffectI();

	virtual int Init( char *file )	{return( -1 ); }
	virtual int InitFromData( char *data )	{return( -1 ); }

	virtual int	Process( image_type *sim, int iFrame, long long timeStamp,image_type **im ) = 0;

	virtual int Close() = 0;

	void SetAlpha( image_type *im );

	void MergeAlpha( image_type *sim );
    
    void SwitchBR();

private:
    void DeleteContents();
public:
	int	m_id;
    int m_switchBR;

};


image_type *	image3_green_alpha( image_type *sim, image_type *im );


#endif


