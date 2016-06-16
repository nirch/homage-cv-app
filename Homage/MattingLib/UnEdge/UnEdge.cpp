
#include	<string.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Ulog/Log.h"
#include "Uvl/Vl2fType.h"




#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"

#include	"EdgeLib/Ridge/RidgeDetector.h"

#include	"UnEdge.h"



CUnEdge::CUnEdge()
{
	m_yim = NULL;
	m_eim = NULL;

	m_dr = NULL;

	m_apl = NULL;

	m_avl = NULL;


	gpTime_init( &m_gTime );
}


CUnEdge::~CUnEdge()
{
	IMAGE_DESTROY( m_yim );
	
	IMAGE_DESTROY( m_eim );

	PLNA_DESTROY( m_apl );

	if( m_dr != NULL ){
		delete m_dr;
		m_dr = NULL;
	}
}




int	CUnEdge::Init( char *prmFile )
{
	m_prm = unEdgePrm_alloc();

	m_dr = new CRidgeDetector();
	m_dr->Init(  (char *)NULL );

	m_dr->m_prm->detect.lm_min = 1.5;
	m_dr->m_prm->detect.sign = 1;
	m_dr->m_prm->detect.size = 5;
	m_dr->m_prm->detect.weight = 3;



	
	plnCurvaturePrm_set( &m_prm->c, 3, 25, 5 );
	m_prm->c.slT1 = 1.0;
	m_prm->c.slLen = 70;


	return( 1 );
}





int	CUnEdge::Process(  image_type *sim, int iFrame )
{

	gpTime_start( &m_gTime );

	m_yim = image1_from( sim, m_yim );

	m_eim  = image1_gridient_sobol_value( m_yim, m_eim );
	image_type *mim = image1_binary( m_eim, 8, NULL );

	if( m_yim == sim )
		m_yim = NULL;


//#define _TEST
#ifdef _TEST
//	int	nPixel = image1_nPixel_nonzero( mim );
//	fprintf( stdout, "%d", nPixel );
	image1_threshold( m_eim, 8 );
	image_dump_scale( m_eim, 4, "EDGE", m_iFrame, "1" );
#endif




	m_dr->Detect( m_eim, mim );

	image_destroy( mim, 1 );


	m_apl = m_dr->Get( 16, 0.5, 1 );

	PLNA_DUMP( m_apl, "edge", m_iFrame, NULL );

	gpTime_stop( &m_gTime );




	return( 1 );
}


int	CUnEdge::ProcessStraightLine( vl2fA_type **avl )
{
	int nT = 10;

	
	m_avl = plnA_straight(  m_apl, &m_prm->c, nT, m_avl );


	*avl = m_avl;

	return( 1 );
}