
#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include "Uigp/igp.h"


#include "PlnType.h"
#include "PlnItrator.h"



int	
iPln_set( iPln_type *ip, pln_type *pl )
{
	ip->pl = pl;

	ip->gt = 0;
	
	ip->ctr = pl->ctr;
	ip->l = pl->link;
	ip->t = 0;

	return( 1 );
}


int	
iPln_next( iPln_type *pi, float dt )
{
	if( pi->gt + dt > pi->pl->len )	return( -1 );

	pi->gt += dt;
	pi->t += dt;



	while( pi->t > pi->l->len ){
		pi->t -= pi->l->len;
		pi->ctr.x += pi->l->v.x;
		pi->ctr.y += pi->l->v.y;

		pi->l = LN_NEXT( pi->l );
		if( pi->l == NULL )	return( -1 );
	}

	return( 1 );
}


