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

	m_nRmim = 0;
	m_rmim[0] = m_rmim[1] = m_rmim[2] = m_rmim[3] = NULL;
	

	gpTime_init( &m_tDetector );
	gpTime_init( &m_tLine );

	m_prm = pRidgePrm_alloc();
}



CRidgeDetector::~CRidgeDetector()
{
int	i;
	DeleteContents();


	for( i = 0 ; i < m_nRmim ; i++ ){
		image_destroy( m_rmim[i], 1 );
		m_rmim[i] = NULL;
	}
	m_nRmim = 0;




	//if( m_rmim != NULL ){
	//	image_destroy( m_rmim, 1 );
	//	m_rmim = NULL;
	//}
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


	gpTime_start( &m_tDetector );
	m_rim = pRidge_detector( imy, mim, m_rmim, m_nRmim, &m_prm->detect, m_rim );
	gpTime_stop( &m_tDetector );

	int no = sim->width*sim->height / 16;

	gpTime_start( &m_tLine );
	m_ar = plRidgeA_alloc( no );
	pRidge_line( m_rim, 2, &m_prm->line, m_ar );
	gpTime_stop( &m_tLine );

	if( imy != sim )
		image_destroy( imy, 1 );



	return( 1 );
}



int CRidgeDetector::SetMask( image_type *sim, image_type *mim, int add )
{
	image_type *imy = image1_from( sim, NULL );

	if(  add == 0 ){
		m_rmim[0] = pRidge_detector( imy, mim, NULL, 0, &m_prm->detect, m_rmim[0] );
		m_nRmim = 1;
	}
	else {
		if( m_nRmim < 2 ){
			m_rmim[m_nRmim] = pRidge_detector( imy, mim, NULL, 0, &m_prm->detect, m_rmim[m_nRmim] );
			m_nRmim++;
		}
		else m_rmim[m_nRmim-1] = pRidge_detector( imy, mim, NULL, 0, &m_prm->detect, m_rmim[m_nRmim-1] );

	}


	if( imy != sim )
		image_destroy( imy, 1 );

	return( 1 );
}



plnA_type * CRidgeDetector::Get( float tLen, float tLm, int fData )
{
plnA_type	*apl;
	int	i;

	if( m_ar == NULL )	return( NULL );

	apl = plnA_alloc( m_ar->nR );

	
	for( i = 0 ; i < m_ar->nR ; i++ ){
		plRidge_type *r = m_ar->r[i];

		if( r->pl->len < tLen || ABS(r->lm) < tLm )
			continue;


		//pln_type *pl = m_ar->r[i]->pl;
		//if(  pl->len > tLen ){

		pln_type *pl = r->pl;

		if( fData == 1 )
			pl = pln_copy( pl );

		pl->qulity = r->lm;
		apl->a[apl->nA++] = pl;
//		}
	}

	return( apl );

}



void CRidgeDetector::Trace( FILE *fp )
{
	gpTime_print( fp, "Ridge-Detector", &m_tDetector );
	gpTime_print( fp, "Ridge-Line", &m_tLine );
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


image_type * CRidgeDetector::GetRimage( image_type *im )
{

		pRidge_type	*dp;
		int	i,	j;

		im = image_realloc( im, m_rim->width, m_rim->height, 1, IMAGE_TYPE_U8, 1 );


		u_char *tp = im->data;

		dp = (pRidge_type *)m_rim->data;
		for( i = 0 ; i < m_rim->height-0 ; i++ ){
			dp = (pRidge_type *)IMAGE_PIXEL( m_rim, i, 0 );
			for( j = 0 ; j < m_rim->width-0 ; j++, dp++, tp++ ){


				if( dp->state == 0 || dp->ng[0]< 0 && dp->ng[1] < 0 ){
					*tp = 0;
					continue;
				}

				*tp = 255;
			}
		}

		return( im );
}



plnA_type * CRidgeDetector::Component( float dT, float sLen, float mLen )
{
int	i;

	plnA_type *apl =  Get( 1, 1, 0 );

	if( apl->nA == 0  ){
		plnA_destroy( apl );
		return( NULL );
	}

	PLNA_DUMP( apl, "Aa", 1, "before-comp" );


	plnA_group( apl, dT );

	plnA_type *bapl = plnA_alloc( apl->mGroup + 1 );

	plnA_type *gapl = NULL;

	for( i = 0 ; i < apl->mGroup ; i++ ){
		gapl = plnA_get_group( apl, i, 0, gapl );
		if( gapl->nA == 0 )	continue;;

		float len = plnA_length( gapl );
		float len1 = plnA_max_length( gapl );

		if( len < sLen || mLen < mLen )
			continue;

		pln_type *pl = plnA_bounding( gapl, 10 );

		bapl->a[bapl->nA++] = pl;
	}


	apl->nA = 0;
	plnA_destroy( apl );

	gapl->nA = NULL;
	plnA_destroy( gapl );

	return( bapl );
}