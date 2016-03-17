
#ifndef _HR_EFFECT_ALIGNMENT_H
#define _HR_EFFECT_ALIGNMENT_H




#include "Umath/Lt2Type.h"
#include "Ubox/Box2d.h"
#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "HrEffectI.h"



class CHrEffectAlignment : public CHrEffectI
{
public:
	
	CHrEffectAlignment();

	~CHrEffectAlignment();




	int Init( char *inFile );
	int InitFromData( char *data0, char *data );

	 int Process( image_type *sim, int iFrame, long long timeStamp, image_type **im );


	int	Close();

private:
    void DeleteContents();
	int m_width;
	int m_height;

	int m_nFrame;

	int	m_iFrame;

	box2f_type m_box0;

	box2f_type m_box;
	lt2_type m_lt;


	image_type *m_im;


};






#endif


