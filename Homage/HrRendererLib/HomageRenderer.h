
#ifndef _BACKGROUND_REMOVAL_H_
#define _BACKGROUND_REMOVAL_H_








#include "ImageType/ImageType.h"
#include "Utime/GpTime.h"

#include "HrSource/HrSourceI.h"
#include "HrOutput/HrOutputI.h"




class CHomageRenderer 
{
public:
	
	CHomageRenderer();

	virtual ~CHomageRenderer();

	void DeleteContents();

	int IsProcess()	{ return m_process; }

    void setDuration(double duration, int fps);

	int	Process( CHrSourceI *b, CHrSourceI *u, CHrSourceI *f, CHrOutputI *out[], int nOut );

	int AddSource( CHrSourceI *s );
	int AddOutput( CHrOutputI *out );

	int SetBackground( image_type *bim );

	int AddLayer( int x0, int y0, image_type *sim, image_type *aim );

	int AddLayer( int x0, int y0, image_type *sim );


	void SetFrameSize( int width, int height );




	image_type * Process( image_type *bim,  
						int x0, int y0, float scale, image_type *him, image_type *himA,  
						image_type *fim[], int nFim,
						image_type *im );


	int	Process();


	image_type * GetImage()	{ return m_im; }


	class CHrSourceI * GetSource( int i  );

	class CHrOutputI * GetOuput( int i  );


	void	Trace( FILE *fp )	{}

    int fps;
    double duration;
    long long maxTimeStamp;
    long long timeDeltaPerFrame;

//private:
public:
	int	m_iFrame;

	image_type	*m_im;
	image_type *m_oim;


	int	m_nS;
	CHrSourceI	*m_as[16];

	int	m_nOut;
	CHrOutputI	*m_aOut[16];


	int m_process;
};


#endif


