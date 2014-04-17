/*******************
 ***  PlnTool.c  ***
 *******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"


#include "Uln/PlnType.h"


pln_type *
pln_alloc( int no )
{
	pln_type *pl;

	pl = ( pln_type *)malloc( sizeof(pln_type) );

	pl->state = 0;
	pl->link = NULL;

	if( no > 0 )
		pl->link = lnL_alloc( no );

	return( pl );
}


pln_type *
pln_copy( pln_type *spl )
{
pln_type *pl;

	pl = pln_alloc(0);
	*pl = *spl;

	pl->link = lnL_copy( spl->link, NULL );

	return( pl );
}


void
pln_destroy( pln_type *pl )
{
	if( pl->link != NULL )
		lnL_destroy( pl->link );

	free( pl );
}


int
pln_set( pln_type *pl, vec2f_type *ctr, ln_type *l )
{


	pl->i[0] = 0;
	pl->i[1] = 0;

	pl->nP = 0;

	pl->ctr = *ctr;
	pl->link = l;

	pl->len = lnL_length( pl->link );

	return( 1 );
}


plnA_type *	
plnA_alloc( int n )
{
plnA_type	*apl;

	apl = ( plnA_type *)malloc( sizeof( plnA_type) );
	
	apl->NA = n;
	
	apl->a = ( pln_type **)malloc( apl->NA * sizeof( pln_type*) );

	apl->nA = 0;

	return( apl );
}


void 
plnA_destroy( plnA_type *apl )
{
pln_type	*pl;
int	i;


	for( i = 0 ; i < apl->nA ; i++ ){
		pl = apl->a[i];

		pln_destroy( pl );
	}


	free( apl->a );

	free( apl );
}

void 
plnA_clear( plnA_type *apl )
{
int	i;


	for( i = 0 ; i < apl->nA ; i++ ){

		pln_destroy( apl->a[i] );
		apl->a[i] = NULL;
	}

	apl->nA = 0;
}


int 
plnA_decrease( plnA_type *apl )
{
	int	i;

	for( i = 0 ; i < apl->nA ;  ){
		if( apl->a[i] != NULL && apl->a[i]->link != NULL ){
			i++;
			continue;
		}

		apl->nA--;
		apl->a[i] = apl->a[apl->nA];
	}

	return( apl->nA );
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


pln_type *
pln_from_point( vec2d p[], int no, int Fclose )
{
pln_type	*pln;

	pln = pln_alloc( 0 );

	lnL_from_points( p, no, Fclose, &pln->ctr, &pln->link );

	pln->state = ( Fclose ==1 )? PLN_CLOSE : PLN_OPEN;

	return( pln );
}


void
pln_scale( pln_type *pl, float scale )
{
ln_type	*l;

	pl->ctr.x *= scale;
	pl->ctr.y *= scale;

	pl->len *= scale;

	for( l = pl->link ; l != NULL ; l = LN_NEXT(l) ){

		l->ctr.x *= scale;
		l->ctr.y *= scale;

		l->v.x *= scale;
		l->v.y *= scale;

		l->len *= scale;
	}
}


pln_type *
pln_from_lnA( ln_type al[], int nAl )
{
	pln_type *pl;
	ln_type	*l;
	int	i;


	pl = pln_alloc( nAl );

	pl->ctr = al[0].ctr;
//	pl->link = lnL_alloc( nAl );

	pl->len = 0;
	for( l = pl->link, i = 0 ; l != NULL ; l = LN_NEXT(l), i++ ){
		l->a = al[i].a;
		l->c_prb = al[i].c_prb;
		l->v = al[i].v;
		l->u = al[i].u;

		l->len = al[i].len;

		l->ctr = al[i].ctr;

		pl->len += l->len;
	}

	return( pl );
}

int 
plnA_select( plnA_type *apl, vec2f_type *v, float dis )
{
pln_type	*pl;
int	i,	iMin;
float	u,	t,	gt,	uMin;
ln_type	*l;


	iMin = -1;
	uMin = dis;
	for( i = 0 ; i < apl->nA ; i++ ){
		pl = apl->a[i];

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


int 
plnA_length( plnA_type *apl )
{
pln_type	*pl;
int	i;
float	len;


	for( i = 0, len = 0 ; i < apl->nA ; i++ ){
		pl = apl->a[i];

		len += pl->len;
	}

	return( len );
}


void
pln_trim( pln_type *pl, int direct, float gt )
{
	vec2f_type	ctr;
	ln_type	*link;

	if( direct == F_BEGIN )
		lnL_copy_sub( &pl->ctr, pl->link,  gt, pl->len, &ctr, &link );
	else
		lnL_copy_sub( &pl->ctr, pl->link,  0, pl->len-gt, &ctr, &link );

	free( pl->link );
	pl->ctr = ctr;
	pl->link = link;

	pl->len = lnL_length( pl->link );
}


void
pln_appendL( pln_type *pl, vec2f_type *ctr, ln_type *link )
{
	lnL_connect_s( &pl->ctr, pl->link, ctr, link, 1.0 );

	pl->len = lnL_length( pl->link );
}

void
pln_append( pln_type *pl, pln_type *pl1 )
{
	lnL_connect_s( &pl->ctr, pl->link, &pl1->ctr, pl1->link, 1.0 );

	pl1->link = NULL;

	pl->len = lnL_length( pl->link );
}



void
pln_end_point( pln_type *pl, vec2d *p )
{
ln_type	*l;

	for( l = pl->link, *p = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){
		p->x += l->v.x;
		p->y += l->v.y;
	}
}


void
pln_gt2p( pln_type *pl, float gt, vec2f_type *p )
{
ln_type	*l;
float	t;
vec2f_type	ctr;

	lnL_gt2lt_p( &pl->ctr, pl->link, gt, &ctr, &l, &t );

	ln_t2xy( &ctr, l, t, p );
}


//int
//pln_write( pln_type *pl, char *file )
//{
//	int	ret;
//
//	ret = ln_write_to_file( &pl->ctr, pl->link, file );
//
//	return( ret );
//}



void
pln_box( pln_type *pl, box2f_type *box )
{
	lnL_box( &pl->ctr, pl->link, NULL, box );
}



int
pln_distance( pln_type *pl, vec2f_type *p, dPln_type *d )
{
	int	ret;

	ret = ln_distance( &pl->ctr, pl->link, NULL, p, &d->l, &d->u, &d->t, &d->gt );

	return( ret );
}


int
plnA_distance( plnA_type *apl, vec2f_type *p, float D, pln_type **spl, dPln_type *sd )
{
	dPln_type d;
	int i;

	*spl = NULL;

	sd->u = D;
	for( i = 0 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];
		if( pln_distance( pl, p, &d ) < 0 )
			continue;

		if( d.gt < 0 || d.gt > pl->len )	continue;


		if( ABS(d.u) < ABS(sd->u) ){
			*spl = pl;
			*sd = d;
		}
	}

	if( *spl == NULL )
		return( -1 );

	return( 1 );
}



void
pln_sample( pln_type *pl, float t0, float r, int n, int direct, pt2dA_type *apt )
{
	vec2f_type	p;
	float	t;
	int	i;

	if( direct == 0 ){
		for( t = t0, i = 0 ; i < n ; i++, t += r ){
			if( t > pl->len )	t = pl->len;

			pln_gt2p( pl, t, &p );

			apt->p[apt->nP].p.x = p.y;
			apt->p[apt->nP].p.y = p.x;
			apt->nP++;

		}

		return;
	}


	for( t = t0, i = 0 ; i < n ; i++, t -= r ){

		pln_gt2p( pl, t, &p );

		apt->p[apt->nP].p.x = p.y;
		apt->p[apt->nP].p.y = p.x;
		apt->nP++;
	}
}