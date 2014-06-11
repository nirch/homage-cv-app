
#ifndef _COLOR_BACKGROUND_H_
#define _COLOR_BACKGROUND_H_


#include "Uvec/Vec3d.h"

#include "ImageType/ImageType.h"




class CColorBackground 
{
public:
	
	CColorBackground();

	~CColorBackground();

	void DeleteContents();

	int	ProcessHistogram( image_type *sim, image_type *mim );


	int	Process( image_type *sim, image_type *mim , image_type **cim );



protected:

	int	m_iFrame;

	float colorShiftThresh;


	vec2i_type	m_v;





public:
	image_type *m_him;
	image_type *m_cim;

};


#endif


