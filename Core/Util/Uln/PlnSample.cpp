
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"


#include "PlnType.h"



void
	pln_sample( pln_type *pl, float t0, float r, int n, int direct, pt2dA_type *apt )
{
	vec2f_type	p;
	float	t;
	int	i;

	if( direct == 0 ){
		for( t = t0, i = 0 ; i < n ; i++, t += r ){
			if( t > pl->len ){
				if( pl->state == PLN_OPEN ){
					if( t > pl->len + 0.75*r )	break;
					t = pl->len;
				}
				else t -= pl->len;
			}

			pln_gt2p( pl, t, &p );

			apt->p[apt->nP].p.x = p.y;
			apt->p[apt->nP].p.y = p.x;
			apt->nP++;

		}

		return;
	}


	for( t = t0, i = 0 ; i < n ; i++, t -= r ){

		if( t < 0 ){
			if( t < -0.75*r )	break;
			t = 0;
		}

		pln_gt2p( pl, t, &p );

		apt->p[apt->nP].p.x = p.y;
		apt->p[apt->nP].p.y = p.x;
		apt->nP++;
	}
}



pt2dA_type *
	plnA_sample( plnA_type *apl, pt2dA_type *apt )
{
	int i;
	float len;

	for( i = 0, len = 0 ; i < apl->nA ; i++ )
		len += apl->a[i]->len;

	int n = 2*len ;
	apt = pt2dA_realloc( apt, n );

	for( i = 0 ; i < apl->nA ; i++ )
		pln_sampleN( apl->a[i], 1.0, 1.0, apt );



	pt2dA_density_filter_r(apt, 0.25*1.0 );
	return( apt );
}


pt2dA_type *
	pln_sampleN( pln_type *pl, float D, float r, pt2dA_type *apt )
{
	pt2d_type	*pt;
	vec2f_type	p;
	float	dt,	t;
	float	n;


	n = pl->len / D + 0.5 + 2;
	//dt = pl->len /n;// + 2;
	dt = D;

	apt = pt2dA_realloc( apt, n );
	apt->axis = PT2D_AXIS_XY;

	for( t = 0 ; t < pl->len ; t += dt ){

		pln_gt2p( pl, t, &p );

		pt = &apt->p[apt->nP++];
		pt->p.x = p.y;
		pt->p.y = p.x;

		pt->n.x = 1;
		pt->n.y = 0;

		pt->r = r;
		pt->f = t;
		pt->id = 0;
	}


	return( apt );
}


pt2dA_type *
	pln_sampleP( pln_type *pl, float gt0, float gt1, float dt, pt2dA_type *apt )
{
	pt2d_type	*pt;
	vec2f_type	p;
	float	t;
	float	n;

	if( gt0 < 0 )	gt0 = 0;
	if( gt1 < 0 || gt1 > pl->len )	gt1 = pl->len;


	n = pl->len / dt + 2;

	apt = pt2dA_realloc( apt, n );
	apt->nA = 0;

	for( t = gt0 ; t < gt1 ; t += dt ){

		pln_gt2p( pl, t, &p );

		pt = &apt->p[apt->nP++];
		pt->p.x = p.y;
		pt->p.y = p.x;

		pt->n.x = 1;
		pt->n.y = 0;

		pt->r = 1.0;
		pt->f = t;
		pt->id = 0;
	}

	return( apt );
}


pt2dA_type *
pln_samplePV( pln_type *pl, float gt0, float gt1, float dt, pt2dA_type *apt )
{
	pt2d_type	*pt;
	
	float	t;
	float	n;

	if( gt0 < 0 )	gt0 = 0;
	if( gt1 < 0 || gt1 > pl->len )	gt1 = pl->len;


	n = pl->len / dt + 2;

	apt = pt2dA_realloc( apt, n );
	apt->type = PT2D_4V;
	apt->nA = 0;

	for( t = gt0 ; t < gt1 ; t += dt ){

		vec2f_type	p,	v;
		pln_tangetP( pl, t, &p, &v );
///		pln_gt2p( pl, t, &p );

		pt = &apt->p[apt->nP++];
		pt->p.x = p.y;
		pt->p.y = p.x;

		pt->n.x = v.y;
		pt->n.y = v.x;

		pt->r = 1.0;
		pt->f = t;
		pt->id = 0;
	}

	return( apt );
}



pt2dA_type *
pln_sampleC( pln_type *pl, float gt0, int n, float dt, pt2dA_type *apt )
{
	pt2d_type	*pt;
	vec2f_type	p;
	float	gt;
	int	i;


	if( n * dt > pl->len )	
		n = pl->len / dt;

	apt = pt2dA_realloc( apt, n );

	apt->nA = 0;

	for( gt = gt0, i = 0 ; i < n ; i++, gt += dt ){

		pt = &apt->p[apt->nP++];

		if( gt > pl->len )
			gt = gt - pl->len;

		pln_gt2p( pl, gt, &p );

		pt->p.x = p.y;
		pt->p.y = p.x;

		pt->n.x = 1;
		pt->n.y = 0;

		pt->r = 1.0;
		pt->f = gt;
		pt->id = 0;
	}

	return( apt );
}


vl2fA_type *
plnA_sample_vl( plnA_type *apl, vl2fA_type *avl )
{
	int i;
	float len;

	for( i = 0, len = 0 ; i < apl->nA ; i++ )
		len += apl->a[i]->len;

	int n = 2*len ;
	avl = vl2fA_realloc( avl, n );

	for( i = 0 ; i < apl->nA ; i++ )
		pln_sample_vl( apl->a[i], 2, avl );


	return( avl );
}


vl2fA_type *
pln_sample_vl( pln_type *pl, int d, vl2fA_type *avl )
{
pt2dA_type *apt;
int	i;

	int n = ( avl != NULL )? avl->nA + pl->len : pl->len;

	avl = vl2fA_realloc( avl, n );

	apt = pt2dA_alloc( pl->len + 4 );

	pln_sampleN( pl, 1.0, 1.0, apt );

	for( i = d ; i < apt->nA - d ; i++ ){

		vl2f_type vl;
		pt2d_approximate_line_vl( apt, i-d, i+d+1, &vl );
		if( vl.e > 0.2 )	continue;


		vl2f_swap( &vl );

		avl->a[avl->nA++] = vl;
	}


	pt2dA_destroy( apt );

	return( avl );
}


