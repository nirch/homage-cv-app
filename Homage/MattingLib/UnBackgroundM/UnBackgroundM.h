
#ifndef _UNIFORMA_BACKGROUND_H_
#define _UNIFORMA_BACKGROUND_H_


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"

#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"



typedef struct umPrm_type {
	int dx;
	int av;

} umPrm_type;





class CUnBackgroundM 
{
public:
	
	CUnBackgroundM();

	~CUnBackgroundM();

	void DeleteContents();




	int	Init( char *xmlFile, char *ctrFile, int width, int height );




	int	ReadMask( char *inFile, int width, int height );


	
	int	Process( image_type *sim );

	int	ProcessInitBackground( image_type *sim, image_type *mim );


	image_type *GetBim( image_type *bim );

	int GetState()	{ return( m_state ); }



protected:


	
	int	ReadPrm( char *inFile );
	

	umPrm_type *m_prm;

	int	m_iFrame;
	int	m_dFrame;

	int m_flip;

	float m_T;
	
	float m_bT;

	float m_bnT;

	int m_contour;



	box2i_type	m_roi;
	int	m_N;


	image_type	*m_bim;
	int	m_nc[2];
	
	image_type	*m_bimDx;


public:

	clnA_type *m_ac;
	int	m_nM;
	image_type	*m_mim[4];

	int	m_state;


	vec2f_type m_mp;


	gpTime_type	m_rTime;
};



umPrm_type *	umPrm_alloc();

void	umPrm_destroy( umPrm_type *prm );


#endif


