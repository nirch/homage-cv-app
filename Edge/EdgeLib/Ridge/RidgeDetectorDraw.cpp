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





#include "Wutil/UWgpw/GpwType.h"



void CRidgeDetector::Draw( gpw_type *gpw, int fLine, int fCircle )
{
int color[] = {0, RED, YELLOW, GREEN };
int	i,	j;


	if( fCircle != 0 && m_rim != NULL ){

		int dt = 1.0 / gpw->Unit  + 0.5;
		dt = PUSH_TO_RANGE( dt, 1, 5 );

		pRidge_type *rp = (pRidge_type *)m_rim->data;
		for( i = 0; i < m_rim->height ; i++ ){
			for( j = 0 ; j < m_rim->width ; j++, rp++ ){
				if( rp->state == 0 )	continue;


				gpw_marker_0( gpw, rp->p.y + m_box.y0, rp->p.x + m_box.x0, dt, color[rp->state&0x3] );
			}
		}
	}


	if( fLine != 0 && m_ar != NULL ){
		for( i = 0 ; i < m_ar->nR ; i++ ){
//			pln_draw( gpw, m_ar->r[i]->pl, CYAN, CYAN, 0 );
			gpw_pln( gpw, m_ar->r[i]->pl, GREEN, GREEN, 0 );

		}
	}

}


