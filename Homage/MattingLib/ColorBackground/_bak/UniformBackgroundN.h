
#ifndef _COLOR_BACKGROUNDN_H_
#define _COLOR_BACKGROUNDN_H_


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"
#include "ImageType/ImageType.h"





class CUniformBackgroundN
{
public:
	
	CUniformBackgroundN();

	~CUniformBackgroundN();

	void DeleteContents();

	int	ProcessHistogram( image_type *sim, image_type *mim );


	int	Process( image_type *sim, image_type *mim , image_type **cim );


	void	Trace( FILE *fp );

protected:

	int	m_iFrame;

	float m_T;


	vec2i_type	m_v;


	int	m_r;
	int m_g;
	int m_b;


public:
	image_type	*m_bnIm;
	image_type *m_cim;


	gpTime_type	m_gTime;

};


#endif


