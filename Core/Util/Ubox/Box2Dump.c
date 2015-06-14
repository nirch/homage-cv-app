
#include	"Uigp/igp.h"
#include	"Uvec/Vec2d.h"
#include	"Box2d.h"

#include	"Ucamera/Pt2dType.h"
#include	"Uln/PlnType.h"


void
box2iA_dump( box2i ab[], int nB, char *prefix, int index, char *suffix )
{
	plnA_type	*apl;

	apl = plnA_from_boxi( ab, nB );

	plnA_dump( apl, prefix, index, suffix );

	plnA_destroy( apl );
}



#ifdef _AA_
void
box2iA_dump( box2i ab[], int nB, char *prefix, int index, char *suffix )
{
pt2dA_type	*apt;
int	i;

	apt = pt2dA_alloc( nB );
	apt->type = PT2D_4;

	apt->axis = PT2D_AXIS_XY;
	apt->nP = 0;

	for( i = 0 ; i < nB ; i++ ){
		pt2d_type *p = &apt->p[apt->nP++];

		p->p.x = 0.5*(ab[i].x0 + ab[i].x1);
		p->p.y = 0.5*(ab[i].y0 + ab[i].y1);
		p->r = ab[i].x1 - ab[i].x0;

		p->group = 1;
	}



	pt2dA_dump( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}
#endif


void
box2fA_dump( box2f_type ab[], int nB, char *prefix, int index, char *suffix )
{
	pt2dA_type	*apt;
	int	i;

	apt = pt2dA_alloc( nB );
	apt->type = PT2D_4;

	apt->axis = PT2D_AXIS_XY;
	apt->nP = 0;

	for( i = 0 ; i < nB ; i++ ){
		pt2d_type *p = &apt->p[apt->nP++];

		p->p.x = 0.5*(ab[i].x0 + ab[i].x1);
		p->p.y = 0.5*(ab[i].y0 + ab[i].y1);
		p->r = ab[i].x1 - ab[i].x0;

		p->group = 1;
	}



	pt2dA_dump( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}