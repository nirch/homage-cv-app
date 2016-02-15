/*******************
 ***  PlnTool.c  ***
 *******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"


#include "Uln/PlnType.h"


void	
plnA_interior_force_right( plnA_type *apl )
{
int	i;
	
	for( i = 0 ; i < apl->nA ; i++ )
		pln_interior_force_right( apl->a[i] );
}


void	
pln_interior_force_right( pln_type *pl )
{
	if( pln_interior_side( pl ) == 0 )
		pln_inverse( pl );
}

void	
	pln_interior_force_left( pln_type *pl )
{
	if( pln_interior_side( pl ) == 1 )
		pln_inverse( pl );
}



// if the interior of the close contour pl is on right  return 1 otherwise  0
int 
pln_interior_side( pln_type *pl )
{
	ln_type	*l, *lr;
	float		len;
	vec2d		gctr, p;


	// find the longest link
	len = -1.0;
	for ( l = pl->link, gctr = pl->ctr; l != NULL ; l= LN_NEXT( l ) ) {
		if ( l->len > len ) {
			len	= l->len;
			lr	= l;
			p	= gctr;
		}

		gctr.x += l->v.x;
		gctr.y += l->v.y;
	}

	p.x += lr->v.x/2.0 + 0.25*lr->u.x;
	p.y += lr->v.y/2.0 + 0.25*lr->u.y;

	return pln_point_side( pl, &p );

}



// return 1 if p inside p2, else 0
int		
pln_inside( pln_type *p, pln_type *p2 )
{
	return pln_point_side( p2, &p->ctr );
}

int		
pln_insideA( pln_type *pl, pln_type *bpl )
{
float	gt;
vec2f_type	p;

	for( gt = 0 ; gt < pl->len ; gt += 2 ){
		pln_gt2p( pl, gt, &p );

		if( pln_point_side( bpl, &p ) == 0 )
			return( 0 );
	}

	return( 1 );
}


float		
pln_insideP( pln_type *pl, pln_type *bpl )
{
	float	gt,	sum;
	vec2f_type	p;
	float	dt = 2;

	for( gt = 0, sum = 0 ; gt < pl->len ; gt += dt ){
		pln_gt2p( pl, gt, &p );

		if( pln_point_side( bpl, &p ) == 1 )
			sum += dt;
	}

	sum = sum / pl->len;
	return( sum );
}






// if the point  is in the right side of the close contour pl  return 1  otherwise 0
int 
pln_point_side( pln_type *pl, vec2d *p )
{
	ln_type *l;
	vec2d gctr;
	vec2d v1, v2;
	float d1, d2, dd, scal, vect, cos_delta, sin_delta;
	float al, sum;

	sum = 0;

	for ( l = pl->link, gctr = pl->ctr; l != NULL; l = LN_NEXT(l) ) {
		v1.x = gctr.x - p->x;
		v1.y = gctr.y - p->y;

		gctr.x += l->v.x;
		gctr.y += l->v.y;
		v2.x = gctr.x - p->x;
		v2.y = gctr.y - p->y;

		d1 = v1.x*v1.x + v1.y*v1.y;
		d2 = v2.x*v2.x + v2.y*v2.y;
		dd = sqrt(d1*d2);


		scal = v1.x*v2.x + v1.y*v2.y;
		cos_delta = scal/dd;
		vect = v1.x*v2.y - v1.y*v2.x;
		sin_delta = vect/dd;


		al = atan2( sin_delta, cos_delta );
		sum += al;
	}

	return ( ABS(sum) > M_PI );
}

