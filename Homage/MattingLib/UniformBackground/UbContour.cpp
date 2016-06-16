
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"
#include "Ubox/Box2d.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"
#include "ImageLabel/ImageLabel.h"

#include "PlnAdjust/PlnAdjust.h"

#include "plnTracker/PlnHeadTracker/PlnHeadTracker.h"

//#define EXCEPTION


int	imageLabel_main_blob( imageLabel_type *abw, int color, vec2f_type *mp );


int	CUniformBackground::ProcessContour()
{

	if( m_prm->contour ==  0  )
		return( 1 );


	if( m_prm->enableEdge == 1 ) 
		ProcessEdgeContour();


	gpTime_start( &m_tCln );


	IMAGE_DUMPF( m_cim, "cim", m_iFrame, NULL, m_dFrame == m_iFrame  );





	m_abwC = imageLabelUS( m_cim, 128, 0, 1, m_abwC );


	// choose blob
	int	n;
	vec2f_type p,	v;
	float	r;
	if( m_headTracking != NULL && m_headTracking->GetHeadPose( &p, &v, &r) > 0 ){
		VEC2D_SWAP(p);
		imageLabelUS_set_mass( m_abwC );
		n = imageLabel_main_blob( m_abwC, 1, &p );
	}
	else
		n = imageLabel_bigest( m_abwC, 1 );

	
	if( m_cln != NULL ){
		cln_destroy( m_cln );
		m_cln = NULL;
	}

	m_cln = imageLabelUS_contour( m_abwC->im, n, 64, 0 );





	cln_translate( m_cln, -m_abwC->margin, -m_abwC->margin );

#ifdef _AA_
	int i;
	for( i = 0; i < m_abwC->nA ; i++  ){
		if( m_abwC->a[i].id != i )	continue;	
		if( m_abwC->a[i].color == 0 )
			continue;

		m_cln = imageLabelUS_contour( m_abwC->im, i );

		cln_translate( m_cln, -m_abwC->margin, -m_abwC->margin );
		break;
	}
#endif

	

	if( m_dFrame == m_iFrame ){
		CLN_DUMP( m_cln, "AA", m_iFrame,  NULL );
	}


	plnA_type *apl = cln_to_plnA( m_cln, 1 );


	plnA_smooth( apl );

	plnA_adjust_start( apl,  m_sim->height );




	ProcessContourAdjust( apl, m_sim->width, m_sim->height );




	plnF_add( m_fpl, apl, m_iFrame );


	gpTime_stop( &m_tCln );

	return( 1 );
}


static void	image1_mask_sharp( image_type *sim, float a );


int	CUniformBackground::ProcessContourAdjust( plnA_type *apl, int width, int height )
{
	if( m_prm->contour  < 2 || apl->nA <= 0 )	
		return( -1 );

#ifdef EXCEPTION
	try {
#endif


	PLNA_DUMPF( apl, "contor", m_iFrame, NULL, m_dFrame == m_iFrame );



	plnA_type *bapl = ( m_iFrame > 0 ) ? m_fpl->a[m_iFrame-1] : NULL; 


	


	if( m_headTracking != NULL ){
		pln_type	*spl;
		if( m_headTracking->Process( apl, &spl, m_iFrame ) > 0 )	
			plnF_add( m_fplH, plnA_from_pl(spl), m_iFrame );


		if( m_headBoxF == 0 ){
			m_headTracking->HeadBox( apl->a[0], 64, &m_headBox );
			m_headBoxF = 1;
		}
	}





	float gt[2];
	gt[0] = gt[1] = -1;
	if( m_headTracking != NULL )
		m_headTracking->HeadRange( apl, &gt[0], &gt[1] );

	if( plnA_adjust_thin( apl, bapl, gt, m_iFrame ) > 0 )
		plnA_adjust_start( apl, height );
	PLNA_DUMPF( apl, "contor", m_iFrame, "thin", m_dFrame == m_iFrame );






	if( m_prm->enableEdge != 0 ){

		plnA_adjust_bottom( apl, m_aplEdge, height, m_prm->fillBlobAll, m_iFrame );
		PLNA_DUMPF( apl, "contor", m_iFrame, "bottom", m_dFrame == m_iFrame );
	}





	if( m_prm->enableEdge != 0 ){

		float gt[2];
		gt[0] = gt[1] = -1;
		if( m_headTracking != NULL )
			m_headTracking->HeadRange( apl, &gt[0], &gt[1] );

		plnA_adjust_edge( apl, m_aplEdge, height, gt, m_iFrame );
		PLNA_DUMPF( apl, "contor", m_iFrame, "edge",  m_dFrame == m_iFrame );
	}





	plnA_adjust_in( apl, bapl, m_aplEdge, m_prm->in_d, m_iFrame );
	PLNA_DUMPF( apl, "contor", m_iFrame, "in", m_dFrame == m_iFrame );


	vec2f_type p,	v;
	float r;
	if( m_headTracking != NULL && m_headTracking->GetHeadPose( &p, &v, &r) > 0 ){
		plnA_adjust_in_head( apl, &p, r, m_iFrame );
		PLNA_DUMPF( apl, "contor", m_iFrame, "in-head", m_dFrame == m_iFrame );
		m_mp.x = p.y/2;
		m_mp.y = p.x/2;
	}




	//if( bapl != NULL ){
	//	PLNA_DUMP( bapl, "DD", m_iFrame, "thin-bpl-6" );
	//}

	//if( m_iFrame != 39 ){
	plnA_adjust_intersect( apl );
	plnA_adjust_start( apl,  height );
	PLNA_DUMPF( apl, "contor", m_iFrame, "intersect", m_dFrame == m_iFrame );
//	}

#ifdef _AA_
	if( m_iFrame > 0 && m_fpl->a[m_iFrame-1]->nA > 0 ){
		pln_coherent( apl, m_fpl->a[m_iFrame-1], height, m_iFrame );

		PLNA_DUMPF( apl, "contor", m_iFrame, "coherent", m_dFrame == m_iFrame );
	}
#endif



#ifdef EXCEPTION
	}

	catch (...) {
		fprintf( stdout, "EXCEPTION %d", m_iFrame );
		return( -1 );
	}
#endif


#ifdef EXCEPTION
	try {
#endif
	cln_type *cln = cln_from_plnA( apl, 1 );

	if( m_prm->smooth == 0 ){	
		m_cim = image1_mask_cln( cln, width, height, m_cim );
		image1_binaryM( m_cim, 128, m_cim );
		ProcessSmooth();
	}
	else {
		m_cim = image1_mask_cln( cln, width, height, m_cim );
		m_cimS = image_make_copy( m_cim, m_cimS );
	}

	cln_destroy( cln );

#ifdef EXCEPTION
}

catch (...) {
	GPLOG( "EXCEPTION %d", m_iFrame );
	return( -1 );
}
#endif

//	image1_mask_sharp( m_cimS, 1.25 );


if( m_iFrame == m_dFrame ){
	IMAGE_DUMP( m_cim, "SM", m_iFrame, NULL );
	IMAGE_DUMP( m_cimS, "SM", m_iFrame, "SS" );
}
	

	return( 1 );
}


int	CUniformBackground::WriteContour( char *file )
{
	cln_write( m_cln, file );

	return( 1 );
}



int	CUniformBackground::GetHeadPose( vec2f_type *p, float *angle, float *scale )
{
	if( m_headTracking == NULL )
		return( -1 );

	vec2f_type v;
	if( m_headTracking->GetHeadPose( p, &v, scale) < 0 )
		return( -1 );

	*angle = 0;

	return( 1 );
}






static void
image1_mask_sharp( image_type *sim, float a )
{
	u_char	*sp;
	int	i,	j;


	sp = (u_char *)sim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++ ){
			if( *sp == 0 || *sp == 255 )	continue;

			int t = (*sp - 128)*a + 0.5;
			*sp = PUSH_TO_RANGE( t, 0, 255 );
		}
	}
}