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





#include "Wutil/UWgpw/GpwType.h"



void CHunchDetector::Draw( gpw_type *gpw, int fCircle )
{
int	i,	color;

	if( m_ah != NULL ){
		for( i = 0 ; i < m_ah->nA ; i++ ){
			pHunch_type *h = m_ah->a[i];

			color = ( h->lm1 > 0 )? BLUE : GREEN;
			gpw_marker_0( gpw, h->p.x, h->p.y, 5, color );
		}
	}

}


