
#ifndef _DYNAMIC_BACKGROUND_H_
#define _DYNAMIC_BACKGROUND_H_




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
#include "HeadDetection/FacePose.h"
#include "Utime/GpTime.h"

#include "BgState.h"




class CDynamicBackground 
{
public:
	
	CDynamicBackground();

	~CDynamicBackground();

	void DeleteContents();


	int ProcessFrame( image_type *rgbIm, image_type *sim, facePose_type *pose, int iFrame, image_type **im );

	int ProcessFrame( image_type *rgbIm, image_type *im, facePose_type *pose, int iFrame );


	void ProcessMotion( image_type *sim );


	void DbEdgeMotion( image_type *sim );


	image_type *produceSilhouette( image_type *sim, image_type *im );



	image_type *produceSilhouetteClose( image_type *sim, image_type *im );

	image_type *produceSilhouetteClose( image_type *rgbSim, image_type *sim, image_type *sim4, image_type *im, int fBody = 1 );



	image_type *GetBackground()	{ return m_bim; }
	image_type *GetRgbBackground()	{ return m_rgbBim; }


	int ProcessBox( image_type *sim );

	int GetBoxImage( image_type **im );

	int GetBoxImage( image_type *mim, image_type **im );

	image_type *GetDbEdgeImage( image_type *im );

	void	SetTno( int tNo )	{ m_tNo = ( tNo < MAX_N-1)? tNo : MAX_N-1;  }

	void	SetExpand( int ex )	{ m_ex = ex; }

	void	SetUpdate( int  flag )	{ m_update = flag; }

	void	SetUpdateBodyHistogram( int  flag )	{ m_bodyHistogram = flag; }

	void	SetUseBodyHistogram( int  flag )	{ m_useBodyHistogram = flag; }

	void	SetCloseBody( int  flag )	{ m_closeBody = flag; }


	void	SetRgbBackground( int flag )	{ m_rgb = flag; }

	void	SetSilhouetteUpdate( int silhouetteUpdate )	{ m_silhouetteUpdate = silhouetteUpdate; }


	image_type * GetSilhouetteImage()	{ return m_silhouetteIm; }
	image_type * GetNccImage()	{ return m_nccIm; }
	image_type * GetGimImage()	{ return m_gim; }

	int	GetStatus()		{ return( m_state ); }

	float	GetUpdatePercent()	{ return( m_pUpdate ); }


	image_type *GetNoiseImage( image_type *sim, image_type *im );


	void	Trace( FILE *fp );

	void	SetDbEdge( int flag )	{ m_dbEdge = flag; }
	void	SetDbRgb( int flag )	{ m_dbRgb = flag; }
	void	SetFrame0( int iFrame )	{ m_iFrame0 = iFrame; }



private:

	int InitBackground( image_type *rgbSim, image_type *sim, image_type *sim4 );

	int ReplaceBackground( image_type *rgbSim, image_type *bim, image_type *bim4 );

	int	RestoreBackground();

	image_type *produceSilhouette( image_type *rgbSim, image_type *sim, image_type *sim4, image_type *im );
	void produceSilhouetteUpdate( image_type *rgbSim, image_type *sim, image_type *sim4, image_type *im );
	int	m_iFrame;
	int	m_iFrame0;


	image_type	*m_aIm4[MAX_N];

	image_type	*m_eIm[MAX_N];	// edge image

	image_type	*m_rgbBim;
	image_type	*m_bim;
	image_type	*m_bim4;


	bgStage_type	*m_bgP;

	image_type	*m_eBim;
	image_type	*m_gim;




	image_type	*m_gbim;
	image_type	*m_gvar;



	image_type	*m_silhouetteIm;
	


	int	m_h[256];
	int	m_nH;



	facePose_type	m_pose;
	int	m_poseFrame;



	image_type	*m_inBoxIm;


	int	m_bodyHistogram;
	int	m_useBodyHistogram;
	int	m_closeBody;



	int	m_tNo;
	int	m_tVal0;
	int	m_tS;

	int	m_nSataticBg;

	int	m_ex;


	int	m_bodyWidth;

	int	m_update;

	int	m_state;

	int	m_nUpdate;
	float m_pUpdate;

	gpTime_type	m_tBg;
	gpTime_type	m_tSh;

	int	m_rgb;


	int	m_silhouetteUpdate;


	// update Silhate
	image_type	*m_nccIm;	


	int	m_dbEdge;
	int	m_dbRgb;

	int	m_processBox;
};


#endif


