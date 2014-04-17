
#ifndef _BACKGROUND_REMOVAL_H_
#define _BACKGROUND_REMOVAL_H_




#define MAX_N	10
#define MAX_H	10

#define DB_UPDATE						0x01
#define DB_STATUS_BACKGROUND_CHANGE		0x02
#define DB_STATUS_NO_MOTION				0x04


#define ECC_0		0x00
#define ECC_BG		0x01
#define ECC_FR		0x02
#define ECC_MIX		0x04



#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"





class CBackgroundRemoval 
{
public:
	
	CBackgroundRemoval();

	~CBackgroundRemoval();

	void DeleteContents();




	void SetBackground( image_type *sim );

	image_type * Process( image_type *sim, int iFrame );

	image_type *Process( image_type *rgbSim, image_type *sim, image_type *sim4, image_type *im );


	image_type *GetBackground()	{ return m_bim; }
	image_type *GetRgbBackground()	{ return m_rgbBim; }


	void	SetRgbBackground( int flag )	{ m_rgb = flag; }



	image_type * GetSilhouette()	{ return m_silhouetteIm; }
	int GetSilhouette( image_type **silhouetteIm )	{ *silhouetteIm = m_silhouetteIm; return(1); }


	void	Trace( FILE *fp )	{}




private:

	int	m_iFrame;

	image_type	*m_rgbBim;
	image_type	*m_bim;
	image_type	*m_bim4;

	image_type *m_yim;





	image_type	*m_silhouetteIm;
	

	int	m_tVal0;


	int	m_rgb;

};


#endif


