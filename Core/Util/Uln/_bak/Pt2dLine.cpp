/******************
 ***  ptLine.h  ***
 ******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"


#include "PtLine.h"

void
pln_destroy( pln_type *pl )
{
	if( pl->link != NULL )
		lnL_destroy( pl->link );

}


ptLineA_type *	
ptLineA_alloc( int n )
{
ptLineA_type	*am;

	am = ( ptLineA_type *)malloc( sizeof( ptLineA_type) );
	
	am->NA = n;
	
	am->a = ( pln_type *)malloc( am->NA * sizeof( pln_type) );

	am->nA = 0;

	return( am );
}


void 
ptLineA_destroy( ptLineA_type *am )
{
pln_type	*pl;
int	i;


	for( i = 0 ; i < am->nA ; i++ ){
		pl = & am->a[i];

		pln_destroy( pl );
	}


	free( am->a );

	free( am );
}


int 
ptLineA_decrease( ptLineA_type *al )
{
	int	i;

	for( i = 0 ; i < al->nA ;  ){
		if( al->a[i].link != NULL ){
			i++;
			continue;
		}

		al->nA--;
		al->a[i] = al->a[al->nA];
	}

	return( al->nA );
}


void
pln_inverse( pln_type *pl )
{
vec2f_type	p;

	lnL_end_point(&pl->ctr, pl->link, &p );
	pl->ctr = p;

	pl->link = lnL_inverse( pl->link );
}

int
pln_create( pln_type *pl, pt2dA_type *apt )
{


	pl->i[0] = apt->p[0].id;
	pl->i[1] = apt->p[apt->nP-1].id;

	pl->nP = apt->nP;


	pt2d_approximate_link( apt, 0, apt->nP, &pl->ctr, &pl->link );

	pl->len = lnL_length( pl->link );

	return( 1 );
}



pt2dA_type *
pln_get_pt( pt2dA_type *apt, int i0, pt2dA_type *capt )
{
pt2d_type	*pt;//,	*npt;
int	i,	prev,	tmp;

	if( capt == NULL )
		capt = pt2dA_alloc( apt->nP );



	//pt = &apt->p[i0];
	//if( (i = pt->i[0]) > 0 ){
	//	npt = &apt->p[i];
	//	if( npt->id == 2 && (npt->i[0] == i0 || npt->i[1] == i0) )
	//		prev = pt->i[1];
	//	else	prev = pt->i[0];
	//}
	//else	prev = pt->i[0];


	prev = pln_get_prev(apt, i0 );




	capt->nP = 0;
	for( i = i0 ; ; i ){
		pt = &apt->p[i];

		if( pt->id != 2 && pt->id != 3 )
			break;

		capt->p[capt->nP] = *pt;
		capt->p[capt->nP].id = i;
		capt->nP++;

		pt->group = 1;

		if( pt->id == 3 && pt != &apt->p[i0] )
			break;


		tmp = i;
		i = ( pt->i[0] == prev )? pt->i[1] : pt->i[0];
		prev = tmp;

	}

	return( capt );
}




int
pln_get_prev( pt2dA_type *apt, int i0 )
{
pt2d_type	*pt,	*npt;
int	i;

	pt = &apt->p[i0];


	if( (i = pt->i[0]) < 0 )
		return( pt->i[0] );

	npt = &apt->p[i];
	if( npt->id == 2 && (npt->i[0] == i0 || npt->i[1] == i0) )
		return( pt->i[1] );

	return( pt->i[0] );
}



int
pln_get_next( pt2dA_type *apt, int i0 )
{
	pt2d_type	*pt,	*pt0;
	int	i;


	pt0 = &apt->p[i0];
	if( pt0->id != 3 )
		return(0);


	for( i = 0 ; i < 2 ; i++ ){
		int si = pt0->i[i];
		if( si < 0 )	continue;

		pt = &apt->p[si];
		if( (pt->i[0] != i0  &&  pt->i[1] != i0) || pt->id != 2 )
			return( i );
	}


	return( -1 );
}



int
pln_get_next( pt2dA_type *apt, int i0, int prev )
{
	pt2d_type	*pt0;

	pt0 = &apt->p[i0];


	if( pt0->i[0] == prev )	return( pt0->i[1] );
	if( pt0->i[1] = prev )	return( pt0->i[0] );

	return( -2 );
}





int
pln_get_pt( pt2dA_type *apt, int i0, int i1, int nP, pt2dA_type *capt )
{
	pt2d_type	*pt;
	int	i,	prev,	tmp;


	capt->nP = 0;
	capt->p[capt->nP] = apt->p[i0];
	capt->p[capt->nP].id = i0;
	capt->nP++;


	prev = i0;

	for( i = i1 ; capt->nP < nP ; i ){
		pt = &apt->p[i];

		if( pt->id != 2 && pt->id != 3 )
			break;

		capt->p[capt->nP] = *pt;
		capt->p[capt->nP].id = i;
		capt->nP++;

		pt->group = 1;

		if( pt->id == 3 && pt != &apt->p[i1] )
			break;


		tmp = i;
		i = ( pt->i[0] == prev )? pt->i[1] : pt->i[0];
		prev = tmp;

	}

	return( capt->nP );
}




pt2dA_type *
pln_smooth( pt2dA_type *apt, float r, pt2dA_type	*tapt )
{
pt2d_type	*pt;
int	i,	nI;
poly1d4_type pl;
float	t;


	if( tapt == NULL )
		tapt = pt2dA_alloc( apt->nP );

	tapt->nP = 0;


	for( i = 0 ; i < apt->nP ; i++ ){

		pt = &apt->p[i];

		if( i == 0 || i == apt->nP-1 )
			nI = 4;
		else 
			if( i == 1 || i == apt->nP-2 )
				nI = 3;
			else nI = 2;


		tapt->p[i] = apt->p[i];
		if( pt2d_approximate_polynom_2O( apt, i, nI, &apt->p[i].p, &apt->p[i].n, 6*r, &pl ) < 5 ){
			continue;
		}

		pt = &tapt->p[i];
		pt->p.x += pl.a0 * -pt->n.y;
		pt->p.y += pl.a0 * pt->n.x;

		vec2f_type	v;
		v.x = pt->n.x + pl.a1 * -pt->n.y;
		v.y = pt->n.y + pl.a1 * pt->n.x;

		t = hypot( v.x, v.y );
		pt->n.x = v.x / t;
		pt->n.y = v.y / t;

		tapt->nP++;
	}

	tapt->nP = apt->nP;
	tapt->type = PT2D_4V;
	pt2dA_dump( tapt, "1", 1, NULL );



	return( tapt );
}



int 
ptLineA_select( ptLineA_type *am, vec2f_type *v, float dis )
{
pln_type	*pl;
int	i,	iMin;
float	u,	t,	gt,	uMin;
ln_type	*l;


	iMin = -1;
	uMin = dis;
	for( i = 0 ; i < am->nA ; i++ ){
		pl = & am->a[i];

		ln_distance( &pl->ctr, pl->link, NULL, v, &l, &u, &t, &gt );
		if( gt < 0 || gt > pl->len )
			continue;

		if( ABS(u) < ABS(uMin) ){
			uMin = u;
			iMin = i;
		}
	}

	return( iMin );

}



void
pln_reduce( pln_type *pl, int direct, float t )
{
	vec2f_type	ctr;
	ln_type	*link;

	if( direct == F_BEGIN )
		lnL_copy_sub( &pl->ctr, pl->link,  t, pl->len, &ctr, &link );
	else
		lnL_copy_sub( &pl->ctr, pl->link,  0, pl->len-t, &ctr, &link );

	free( pl->link );
	pl->ctr = ctr;
	pl->link = link;
}