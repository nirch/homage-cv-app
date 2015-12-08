
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"
#include "Ubox/Box2d.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"


#include "MattingContour.h"
#include "ImageLabel/ImageLabel.h"

#include "PlnAdjust/PlnAdjust.h"

#include "plnTracker/PlnHeadTracker/PlnHeadTracker.h"

//#define EXCEPTION






int	CMattingContour::ProcessAdjust( plnA_type *apl )
{
	if(  apl->nA <= 0 )	
		return( -1 );



	PLNA_DUMPF( apl, "contor", m_iFrame, NULL, m_dFrame == m_iFrame );



	plnA_type *bapl = ( m_iFrame > 0 ) ? m_fpl->a[m_iFrame-1] : NULL; 


	

	//ProcessHead( apl );





	float gt[2];
	gt[0] = gt[1] = -1;
	if( m_headTracking != NULL )
		m_headTracking->HeadRange( apl, &gt[0], &gt[1] );

	if( plnA_adjust_thin( apl, bapl, gt, m_iFrame ) > 0 )
		plnA_adjust_start( apl, m_height );
	PLNA_DUMPF( apl, "contor", m_iFrame, "thin", m_dFrame == m_iFrame );






	if( m_prm->enableEdge != 0 ){

		plnA_adjust_bottom( apl, m_aplEdge, m_height, m_prm->fillBlobAll, m_iFrame );
		PLNA_DUMPF( apl, "contor", m_iFrame, "bottom", m_dFrame == m_iFrame );
	}





	if( m_prm->enableEdge != 0 ){

		float gt[2];
		gt[0] = gt[1] = -1;
		if( m_headTracking != NULL )
			m_headTracking->HeadRange( apl, &gt[0], &gt[1] );

		plnA_adjust_edge( apl, m_aplEdge, m_height, gt, m_iFrame );
		PLNA_DUMPF( apl, "contor", m_iFrame, "edge",  m_dFrame == m_iFrame );
	}





	plnA_adjust_in( apl, bapl, m_aplEdge, m_prm->in_d, m_iFrame );
	PLNA_DUMPF( apl, "contor", m_iFrame, "in", m_dFrame == m_iFrame );


	vec2f_type p;
	float r;
	if( m_headTracking != NULL && m_headTracking->GetHeadPose( &p, &r) > 0 ){
		plnA_adjust_in_head( apl, &p, r, m_iFrame );
		PLNA_DUMPF( apl, "contor", m_iFrame, "in-head", m_dFrame == m_iFrame );
	}






	plnA_adjust_intersect( apl );
	plnA_adjust_start( apl,  m_height );
	PLNA_DUMPF( apl, "contor", m_iFrame, "intersect", m_dFrame == m_iFrame );



	return( 1 );
}




