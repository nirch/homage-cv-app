
#ifndef _BACKGROUND_REMOVAL_H_
#define _BACKGROUND_REMOVAL_H_








#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"

#include "HrSourceI.h"
#include "HrOutputI.h"




class CHomageRenderer 
{
public:
	
	CHomageRenderer();

	~CHomageRenderer();

	void DeleteContents();


	int	Process( CHrSourceI *b, CHrSourceI *f,  CHrSourceI *h, CHrOutputI *out );



	int SetBackground( image_type *bim );

	int AddLayer( int x0, int y0, image_type *sim, image_type *aim );

	int AddLayer( int x0, int y0, image_type *sim );




	image_type * Process( image_type *bim,  
						int x0, int y0, float scale, image_type *him, image_type *himA,  
						image_type *fim[], int nFim,
						image_type *im );




	image_type * GetImage()	{ return m_im; }





	void	Trace( FILE *fp )	{}




private:

	int	m_iFrame;

	image_type	*m_im;
	image_type *m_oim;


};


#endif


