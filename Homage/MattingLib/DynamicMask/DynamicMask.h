
#ifndef _DYNAMIC_MASK_H_
#define _DYNAMIC_MASK_H_


#include "Uvec/Vec2d.h"
#include "Umath/LT2sType.h"

#include "ImageType/ImageType.h"




class CDynamicMask 
{
public:
	
	CDynamicMask();

	~CDynamicMask();

	void DeleteContents();

	int	Init( image_type *sim, image_type *mim, pt2dA_type *apt, int dd );
	
	int	Process( image_type *sim, int iFrame );

	int	ProcessMask( image_type *sim, int iFrame );
	int	ProcessMaskN( image_type *yim, image_type *sim, int iFrame );

	int	MoveIm( image_type *sim, int iFrame );


protected:

	int	m_iFrame;




	vec2i_type	m_v;

	int	m_nT;
	class CLkRegistration **m_at;

	struct pt2dA_type *m_apt;

	image_type	*m_mim;
	image_type	*m_yim;


public:
	image_type *m_cim;

	struct  vec2fA_type	*m_as;
	struct vec2fA_type	*m_ap;

	lt2_type	m_lt;


	vec2f_type m_p;
	float m_angle;
	float m_scale;

	int m_state;

};


#endif


