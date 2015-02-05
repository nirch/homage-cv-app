/*************************
 ***   VaHolding.cpp   ***
 *************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>

#include "Ucamera/Pt2dType.h"
#include "Uln/PlnType.h"
#include "RidgeDetector.h"


#include	"ImageDump/ImageDump.h"









CRidgeDetector::CRidgeDetector()
{

	m_ar = NULL;

	m_rim = NULL;

	m_rmim = NULL;

	m_prm = pRidgePrm_alloc();
}



CRidgeDetector::~CRidgeDetector()
{
	DeleteContents();


	if( m_rmim != NULL ){
		image_destroy( m_rmim, 1 );
		m_rmim = NULL;
	}
}


void CRidgeDetector::DeleteContents()
{
	if( m_ar != NULL ){
		plRidgeA_destroy( m_ar );
		m_ar = NULL;
	}

	if( m_rim != NULL ){
		image_destroy( m_rim, 1 );
		m_rim = NULL;
	}

	//if( m_rmim != NULL ){
	//	image_destroy( m_rmim, 1 );
	//	m_rmim = NULL;
	//}
}

void CRidgeDetector::Init( char *prmFile )
{
	if( m_prm == NULL )
		m_prm = pRidgePrm_alloc();


	pRidgePrm_read( m_prm, prmFile );

}

void CRidgeDetector::Init( pRidgePrm_type *prm )
{
	if( m_prm == NULL )
		m_prm = pRidgePrm_alloc();


	*m_prm = *prm;
}


#ifdef _AA_
int CRidgeDetector::Detect( image_type *sim, box2i *box )
{
	DeleteContents();

	m_box = *box;


	image_type *im = image_cut( sim, box->y0, box->x0, box->y1-box->y0+1, box->x1-box->x0+1, NULL );
	image_type *imy = image1_from( im, NULL );



	m_rim = pRidge_detector( imy, &m_prm->detect, m_rim );



	m_ar = plRidgeA_alloc( 10000 );
	pRidge_line( m_rim, 2, &m_prm->line, m_ar );


	plRidgeA_translate( m_ar, box->y0, box->x0 );


	if( imy != im )
		image_destroy( imy, 1 );
	


	return( 1 );
}
#endif


int CRidgeDetector::Detect( image_type *sim, image_type *smim, box2i *box )
{
	DeleteContents();

	m_box = *box;

	image_type	*im = image_cut( sim, box->y0, box->x0, box->y1-box->y0+1, box->x1-box->x0+1, NULL );

	image_type	*mim = NULL; 
	if( smim != NULL )
		mim = image_cut( smim, box->y0, box->x0, box->y1-box->y0+1, box->x1-box->x0+1, NULL );



	Detect( im, mim );

	plRidgeA_translate( m_ar, box->y0, box->x0 );


	
	image_destroy( im, 1 );
	if( mim != NULL )
		image_destroy( mim, 1 );


	return( 1 );
}



int CRidgeDetector::Detect( image_type *sim, image_type *mim )
{
	DeleteContents();

	image_type *imy = image1_from( sim, NULL );


	m_rim = pRidge_detector( imy, mim, m_rmim, &m_prm->detect, m_rim );


	int no = sim->width*sim->height / 16;

	m_ar = plRidgeA_alloc( no );
	pRidge_line( m_rim, 2, &m_prm->line, m_ar );


	if( imy != sim )
		image_destroy( imy, 1 );



	return( 1 );
}



int CRidgeDetector::SetMask( image_type *sim, image_type *mim )
{
	image_type *imy = image1_from( sim, NULL );

	m_rmim = pRidge_detector( imy, mim, NULL, &m_prm->detect, m_rmim );


	if( imy != sim )
		image_destroy( imy, 1 );

	return( 1 );
}



plnA_type * CRidgeDetector::Get( float tLen )
{
plnA_type	*apl;
	int	i;

	if( m_ar == NULL )	return( NULL );

	apl = plnA_alloc( m_ar->nR );

	
	for( i = 0 ; i < m_ar->nR ; i++ ){
		pln_type *pl = m_ar->r[i]->pl;
		if(  pl->len > tLen )
			apl->a[apl->nA++] = pl;
	}

	return( apl );

}



void CRidgeDetector::TraceStatus( LogCallback logCallback )
{
	(*logCallback)( "Ridge:  %d  \n", m_ar->nR );
}



void CRidgeDetector::Trace( LogCallback logCallback, vec2f_type *p, float D )
{
	int i;


	i = plRidgeA_select( m_ar, p, D );

	if( i >= 0 ){
		plRidge_type *r = m_ar->r[i];
		(*logCallback)( "Ridge: %d   lm: %.2f   len: %.2f \n", i, r->lm, r->pl->len );

		return;
	}


}



int CRidgeDetector::Union( float lenT, float dT, float aT )
{
	if( m_ar == NULL )	return( -1 );

	plRidgeA_union( m_ar, lenT, dT, aT );

	return( 1 );
}