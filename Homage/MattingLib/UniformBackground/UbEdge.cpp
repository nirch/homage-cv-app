
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"
#include "ImageLabel/ImageLabel.h"

#include "PlnAdjust/PlnAdjust.h"





#include "EdgeLib/Ridge/RidgeDetector.h"




int	CUniformBackground::ProcessEdgeContourInit()
{

	m_dr = new CRidgeDetector();
	m_dr->Init(  (char *)NULL );

	m_dr->m_prm->detect.lm_min = 1.5;
	m_dr->m_prm->detect.sign = 1;
	m_dr->m_prm->detect.size = 7;
	m_dr->m_prm->detect.weight = 3;

	return( 1 );
}

int	CUniformBackground::ProcessEdgeContour()
{



	gpTime_start( &m_tEdge );

	m_eim  = image1_gridient_sobol_value( m_yim, m_eim );
	image_type *mim = image1_binary( m_eim, 8, NULL );

#define _TEST
#ifdef _TEST
	image1_threshold( m_eim, 8 );
	image_dump_scale( m_eim, 4, "EDGE", m_iFrame, "1" );
#endif

	//int nPixel = image1_nPixel_nonzero( mim );
	//fprintf( stdout, "%d ", nPixel );



	m_dr->Detect( m_eim, mim );

	image_destroy( mim, 1 );



	if( m_aplEdge != NULL )
		plnA_destroy( m_aplEdge );

	m_aplEdge = m_dr->Get( 16, 0.5, 1 );

	PLNA_DUMP( m_aplEdge, "edge", m_iFrame, NULL );

	gpTime_stop( &m_tEdge );


	return( 1 );
}

