 
#include	<string.h>
#include	<math.h>

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif


#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"

#include "Wutil/UWgpw/GpwType.h"
#include "Uln/PlnType.h"





#include	"pEdgeType.h"
#include	"LineDetector.h"




void		
CLineDetector::Draw( gpw_type *gpw )
{
	int	aColor[] = { BLUE, YELLOW, MAGENTA, CYAN };
	int	i;

	ln_type *l;

	for( i = 0; i < m_ae->nA ; i++ ){
		pln_type *pl = m_ae->a[i]->pl;
		for( l = pl->link ; l != NULL ; l = LN_NEXT(l) )
			gpw_line( gpw, l->ctr.y, l->ctr.x, l->ctr.y + l->v.y, l->ctr.x + l->v.x, 0xFF0000 );
	}

}




void CLineDetector::Draw( gpw_type *gpw, int fLine, int fCircle )
{
int	i,	j;



	if( fCircle != 0 && m_gim != NULL ){

		int dt = 1.0 / gpw->Unit  + 0.5;
		dt = PUSH_TO_RANGE( dt, 1, 5 );

		pEdge_type *rp = (pEdge_type *)m_gim->data;
		for( i = 0; i < m_gim->height ; i++ ){
			for( j = 0 ; j < m_gim->width ; j++, rp++ ){
				if( rp->state == 0 )	continue;

				float x = m_scale*rp->p.y + m_box.y0;
				float y = m_scale*rp->p.x + m_box.x0;

				gpw_marker_0( gpw, x, y, dt, GREEN );
			}
		}
	}


	if( fLine != 0 && m_ae != NULL ){
		for( i = 0; i < m_ae->nA ; i++ ){
			pln_type *pl = m_ae->a[i]->pl;
			gpw_pln( gpw, pl, RED, CYAN, 1 );
		}
	}

}