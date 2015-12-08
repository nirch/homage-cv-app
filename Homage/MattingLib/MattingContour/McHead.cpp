
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"
#include "Ubox/Box2d.h"

//#ifdef _DEBUG
#define _DUMP 


#include "MattingContour.h"


#include "plnTracker/PlnHeadTracker/PlnHeadTracker.h"

//#define EXCEPTION



int	CMattingContour::InitHeadTracker( int iHead )
{
	cln_type *cln = clnA_detach( m_ac, iHead );
	if ( cln != NULL ){


		if( m_headTracking == NULL )
			m_headTracking = new CPlnHeadTracker();
		m_headTracking->Init( cln );
	}

	m_fplH = plnF_alloc( 100 );

	return( 1 );
}



int	CMattingContour::ProcessHead( plnA_type *apl, int iFrame )
{

	pln_type	*spl;

	if( m_headTracking->Process( apl, &spl, iFrame ) > 0 )	
		plnF_add( m_fplH, plnA_from_pl(spl), iFrame );
		

	if( m_headBoxF == 0 ){
		m_headTracking->HeadBox( apl->a[0], 64, &m_headBox );
		m_headBoxF = 1;
	}


	return( 1 );
}




