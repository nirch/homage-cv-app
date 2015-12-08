
#ifndef _HR_SOURCE_I_H
#define _HR_SOURCE_I_H





#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"


#include "../HrEffect/HrEffectI.h"

#define HR_GIF	1
#define HR_PNG	2


class CHrSourceI 
{
public:
	
	CHrSourceI();

	virtual ~CHrSourceI();

	void DeleteContents();


	virtual int Init( char *file ) { return( -1 ); };

	virtual int Init( char *inFile, int aFrame[], int nF )	{ return( -1 ); };

	virtual int	ReadFrame( int iFrame, image_type **im ) = 0;


	virtual int Close() = 0;



	void SetAlpha( image_type *im );

	void MergeAlpha( image_type *sim );

	int ProcessEffect( image_type *sim, int iFrame, image_type **im );

	int AddEffect( CHrEffectI *e );

	CHrEffectI * GetEffect( int id );
	void	SetFrameSize( int width, int height );

private:
public:
	int	m_iFrame;

	int m_width;
	int m_height;

	image_type *m_alphaIm;



	int	m_nE;
	CHrEffectI	*m_ae[128];

};


image_type *	image3_green_alpha( image_type *sim, image_type *im );


#endif


