/*************************
 ***   VaHolding.cpp   ***
 *************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>

#include "Ucamera/Pt2dType.h"
#include "Uln/PlnType.h"
#include "HunchDetector.h"


#include	"ImageDump/ImageDump.h"
#include	"ImageApp/ImageApp.h"



static image_type *	image_cutB( image_type *sim, box2i *box, int margin, box2i *lbox, vec2f_type *ctr );






CHunchDetector::CHunchDetector()
{

	m_ah = NULL;

	m_prm = pHunchPrm_alloc();
}



CHunchDetector::~CHunchDetector()
{
	DeleteContents();
}


void CHunchDetector::DeleteContents()
{

	if( m_ah != NULL ){
		pHunchA_destroy( m_ah );
		m_ah = NULL;
	}
}

void CHunchDetector::Init( char *prmFile )
{
	if( m_prm == NULL )
		m_prm = pHunchPrm_alloc();


	pHunchPrm_read( m_prm, prmFile );

}

void CHunchDetector::Init( pHunchPrm_type *prm )
{
	if( m_prm == NULL )
		m_prm = pHunchPrm_alloc();


	*m_prm = *prm;
}

void CHunchDetector::Init()
{
	m_ja[0] = jet2App_init( m_prm->size, m_prm->weight );
	m_ja[1] = jet2App_init( m_prm->size+2, m_prm->weight );
	m_ja[2] = jet2App_init( m_prm->size+4, m_prm->weight );
}





int CHunchDetector::Detect( image_type *sim, box2i_type *box )
{
	DeleteContents();

	m_box = *box;
	box2i_type	lbox;
	vec2f_type	ctr;

	image_type *im = image_cutB( sim, box, 10, &lbox, &ctr );
	




	image_type *imy = image_to_y( im, NULL );

	image_type *mim = image_reallocL( imy, NULL );
	image1_const( mim, 1 );


	m_ah = pHunchA_alloc( im->width * im->height / (4*4) + 1000 );

	//	pHunch_detect( imy, mim, &lbox, prm, *ah );
	pHunch_detectN( imy, mim, &lbox, m_prm, m_ah );



	pHunchA_filter( m_ah, 0.5 );

	image_destroy( im, 1 );
	image_destroy( imy, 1 );
	image_destroy( mim, 1 );



	pHunchA_translate( m_ah, ctr.x, ctr.y );

	pHunchA_swap( m_ah );

	return( 1 );
}




int CHunchDetector::DetectL( image_type *yim, box2i_type *box )
{
	if( m_ah == NULL )
		m_ah = pHunchA_alloc( yim->width * yim->height / (4*4)  );

	pHunchA_clear( m_ah );

//	pHunch_detect( yim, box, m_prm, m_ah );
	pHunch_detect( yim, box, m_ja, m_prm, m_ah );

	return( 1 );
}



static image_type *
image_cutB( image_type *sim, box2i *box, int margin, box2i *lbox, vec2f_type *ctr )
{
	box2i mbox = *box;
	box2i_extend( &mbox, margin );
	box2i_crop( &mbox, 0, 0, sim->width-1, sim->height-1 );

	ctr->x = mbox.x0;
	ctr->y = mbox.y0;


	lbox->x0 = box->x0 - ctr->x;
	lbox->x1 = box->x1 - ctr->x;
	lbox->y0 = box->y0 - ctr->y;
	lbox->y1 = box->y1 - ctr->y;



	image_type *im = image_cut( sim, mbox.y0, mbox.x0, mbox.y1-mbox.y0+1, mbox.x1-mbox.x0+1, NULL );

	return( im );
}






void CHunchDetector::TraceStatus( LogCallback logCallback )
{
	(*logCallback)( "Hunch:  %d  \n", m_ah->nA );
}



void CHunchDetector::Trace( LogCallback logCallback, vec2f_type *p, float D )
{
	int i;


	i = pHunchA_select( m_ah, p, D );

	if( i >= 0 ){
		pHunch_type *h = m_ah->a[i];
		(*logCallback)( "Ridge: %d   f: %.2f  lm: %.2f   len: %.2f \n", i, h->f, h->lm1, h->lm2 );

		return;
	}


}
