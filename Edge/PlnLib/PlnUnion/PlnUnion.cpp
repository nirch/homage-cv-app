
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif



#ifdef _DUMP
#define SEGF_PRINT( title, as, nS, fp )  if( falg != 0 ) pseg_print( title, as, nS, fp );
#define SEG_PRINT( title, as, nS, fp )  pseg_print( title, as, nS, fp );
#else 
#define SEG_PRINT( title, as, nS, fp ) 
#endif




#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

   
#include "Uln/PlnType.h"

#include "PlnUnion.h"


typedef struct pseg_type {

	float gt0;

	float gt1;

	float u;


	float	bgt1;
	float	bgt0;

	int	state;

} pseg_type;


static void	pseg_print( char *title, pseg_type ap[], int nP, FILE *fp );

int	pln_out_psegN( pln_type *bpl, pln_type *pl, float u0, pseg_type ap[], int *nP );

static void	pseg_union(pseg_type ap[], int *nP );


static void	psegA_bg(  pln_type *bpl, pln_type *pl, pseg_type ap[], int nP );

static void pseg_filter_in(pseg_type ap[], int *nP );

static void	pseg_filter_len( pln_type *pl, pseg_type ap[], int *nP );


pln_type *	plnA_merge( plnA_type *apl );


pln_type *	plnC_union( pln_type *bpl, pln_type *pl );

static float	psegA_max_u( pseg_type ap[], int nP );

static void	pseg_union1(pseg_type ap[], int *nP );


static int	pln_set_start( pln_type *pl );


#ifdef _AA_
plnA_type *
pln_merge_group( plnA_type *apl, int group0 )
{
int	i;

	plnA_type *gapl = NULL;

	plnA_type *bapl = plnA_alloc( apl->nA );
	
	int group = ( group0 < 0 )? 0 : group0;
	for( i = 0 ; i < apl->nA ; i++ ){

		if( apl->a[i]->group < group )
			continue;

		group = apl->a[i]->group;
		gapl = plnA_copy_group( apl, group, 0, gapl );

		PLNA_DUMP( gapl, "G", group, NULL );

		group += 1;


		pln_type *pl = plnA_merge( gapl );

		plnA_add( bapl, pl );

//		fprintf( stdout, "  ." );

		if( group0 >= 0 )
			break;
		
	}

	gapl->nA = 0;
	plnA_destroy( gapl );

	return( bapl );
}


static int			plnA_find_nrearest( plnA_type *apl, pln_type *pl );


pln_type *
	plnA_merge( plnA_type *apl )
{
int	i,	j;
	if( apl->nA == 1 ){
		pln_type *pl = pln_copy( apl->a[0] );
		return( pl );

	}

	plnA_reorder_length( apl );

	for( i = 0 ; i < apl->nA ; i++ )
		apl->a[i]->id = 0;



	PLNA_DUMP( apl, "merge", 1, "ALL" );



	pln_type *pl = pln_copy( apl->a[0] );
	apl->a[0]->id = 1;


	for( i = 1 ; i < apl->nA ; i++ ){

		j = plnA_find_nrearest( apl, pl );
		if( j < 0 )	break;



		PLN_DUMP( pl, "merge", i, "B" );
		PLN_DUMP( apl->a[j], "merge", i, "A" );

		pln_type *tpl = plnC_union( pl, apl->a[j] );
		if( tpl == NULL )
			continue;


		apl->a[j]->id = 1;

		pln_destroy( pl );
		pl = tpl;

	

		PLN_DUMP( pl, "merge",i, "M" );

		//pln_set_start( pl );
		//PLN_DUMP( pl, "merge",i, "N" );


	}


	pl->group = apl->a[0]->group;
	pl->size = apl->a[0]->size;
	pl->height = apl->a[0]->height;

	return( pl );
}

static int		
plnA_find_nrearest( plnA_type *apl, pln_type *pl )
{
	int	i,	mi;
	float mMax;

	mi = -1;
	for( i = 0 ; i < apl->nA ; i++ ){
		if( apl->a[i]->id != 0 )	continue;


		float m0,	m1;
		if( pln_distance_plnM( pl, apl->a[i], &m0, &m1 ) < 0 )
			continue;

		if( mi < 0 || m1 > mMax ){
			mMax = m1;
			mi = i;
		}
	}

	return( mi );
}


pln_type *
plnA_mergeN( plnA_type *apl )
{
	if( apl->nA == 1 ){
		pln_type *pl = pln_copy( apl->a[0] );
		return( pl );

	}

	plnA_reorder_length( apl );

	PLNA_DUMP( apl, "merge", 1, "ALL" );

	int i;

	pln_type *pl = pln_copy( apl->a[0] );


	for( i = 1 ; i < apl->nA ; i++ ){
		PLN_DUMP( pl, "merge", i, "B" );
		PLN_DUMP( apl->a[i], "merge", i, "A" );

		pln_type *tpl = plnC_union( pl, apl->a[i] );
		if( tpl == NULL )
			continue;

		

		pln_destroy( pl );
		pl = tpl;

		PLN_DUMP( pl, "merge",i, "M" );

	}


	pl->group = apl->a[0]->group;
	pl->size = apl->a[0]->size;
	pl->height = apl->a[0]->height;

	return( pl );
}
#endif


pln_type *
plnC_union( pln_type *bpl, pln_type *pl )
{
pseg_type ap[200];
int nP;

	pln_out_psegN( bpl, pl, 0.1, ap,  &nP );


	if( psegA_max_u( ap, nP) < 1.0 )
			pln_out_psegN( bpl, pl, 0.5, ap,  &nP );


	SEG_PRINT( "one", ap, nP, stdout );
	if( nP <= 0 )
		return( NULL );


	pseg_union1( ap, &nP );
	SEG_PRINT( "union1", ap, nP, stdout );


	pseg_union( ap, &nP );
	SEG_PRINT( "union", ap, nP, stdout );

	if( nP <= 0 )
		return( NULL );


	pseg_filter_in( ap, &nP );
	SEG_PRINT( "filter", ap, nP, stdout );

	pseg_filter_len( pl, ap, &nP );
	SEG_PRINT( "filter-in", ap, nP, stdout );

	if( nP == 0 ){
		pln_type *spl = pln_copy( bpl );
		return( spl );
	}

	psegA_bg(  bpl, pl,  ap, nP );

	pln_type *spl = NULL;

	int	i;
	for( i = 0 ; i < nP ; i++ ){
		pseg_type *s = &ap[i];

		pln_type *tpl = pln_copy_sub( pl, s->gt0, s->gt1 );


		if( spl != NULL )
			pln_append( spl, tpl );
		else spl = tpl;


		int i1 = ( i < nP -1 )? i+1 : 0;
		pseg_type *s1 = &ap[i1];

		

		tpl = pln_copy_sub( bpl, s->bgt1, s1->bgt0 );

		if( tpl->len < 0.5 ){
			pln_destroy( tpl );
			continue;
		}


		pln_append( spl, tpl );
	}


	pln_close( spl, 0.5 );

	PLN_DUMP( spl, "AA", 1, NULL );

	return( spl );
}


int
	pln_out_psegN( pln_type *bpl, pln_type *pl, float u, pseg_type ap[], int *nP )
{
	float	gt,	dt;

	dt = 1.0;

	int state = 0;

	*nP = 0;

	pseg_type *ps = &ap[(*nP)++];
	ps->state = -1;

	for( gt = 0 ; gt < pl->len ; gt += dt ){

		vec2f_type	p;
		pln_gt2p( pl, gt, &p );

		dPln_type d;
		if( pln_distance( bpl, &p, &d ) < 0 )
			d.u = -100;

		d.u += u;

		if( ps->state == -1 ){
			ps->gt0 = ps->gt1 = gt;
			ps->u = d.u;
			ps->state = ( d.u < 0 );
			continue;
		}

		state = (d.u <= 0 );

		if( ps->state == state ){
			ps->gt1 = gt;
			if( ABS(ps->u) < ABS(d.u) )
				ps->u = d.u;

			continue;
		}


		ps = &ap[(*nP)++];
		ps->gt0 = ps->gt1 = gt;
		ps->u = d.u;
		ps->state = ( d.u < 0 );

	}

	if( *nP <= 1 )
		return( 1 );


	if( ap[(*nP)-1].state == ap[0].state ){
		ap[0].gt0 = ap[(*nP)-1].gt0;
		if( ABS(ap[(*nP)-1].u) > ABS(ap[0].u) )
			ap[0].u = ap[(*nP)-1].u;

		*nP -= 1;
	}


	return( 1 );

}


static void
psegA_bg(  pln_type *bpl, pln_type *pl, pseg_type ap[], int nP )
{
	int	i;
	for( i = 0 ; i < nP ; i++ ){
		pseg_type *ps = &ap[i];

		vec2f_type	p;
		pln_gt2p( pl, ps->gt0, &p );

		dPln_type d;
		pln_distance( bpl, &p, &d );

		ps->bgt0 = d.gt;



		pln_gt2p( pl, ps->gt1, &p );
		pln_distance( bpl, &p, &d );

		ps->bgt1 = d.gt;
	}

}



static void
pseg_print(  char *title, pseg_type ap[], int nP, FILE *fp )
{
int	i;

	fprintf( fp, "%s  %d\n", title, nP );
	for( i = 0 ; i < nP ; i++ ){
		pseg_type *p = &ap[i];
		fprintf( fp, "%d   %.2f  %.2f  %.4f\n", i, p->gt0, p->gt1, p->u );
	}
}

static float
	psegA_max_u( pseg_type ap[], int nP )
{
	int	i;

	float u = 0;
	for( i = 0 ; i < nP ; i++ ){
		pseg_type *p = &ap[i];
		if( p->u > u )
			u = p->u;
	}

	return( u );
}

static void
pseg_union1(pseg_type ap[], int *nP )
{
	int	i,	j;

	for( i = 1, j = 0 ; i < *nP ; i++ ){
		if( ap[j].u > 0.5 && ap[i].u > -0.25 ){
			ap[j].gt1 = ap[i].gt1;
			ap[j].bgt1 = ap[i].bgt1;
			continue;
		}

		j++;

		ap[j] = ap[i];
	}

	*nP = j+1;
}


static void
pseg_union(pseg_type ap[], int *nP )
{
	int	i,	j,	a[100];


	

	for( i = 0, j = 0 ; i < *nP ; i++ ){
		if( ABS(ap[i].u) > 0.5 ){
				a[j++] = i;

			ap[i].state |= 0x2;
		}
	}

	int n = j;



	for( i = 0 ; i < n-1 ; i++ ){
		int i0 = a[i];
		int i1 = a[i+1];

		if( ap[i0].state == ap[i1].state ){
			ap[i0].gt1 = ap[i1].gt1;

			if( ABS(ap[i0].u) < ABS(ap[i1].u) )
				ap[i0].u = ap[i1].u;


			ap[i1].state &= 0x01;
			continue;
		}
	}




	if( n > 1 ){
		int i0 = a[n-1];
		int i1 = a[0];

		if( ap[i0].state == ap[i1].state ){
			ap[i0].gt1 = ap[i1].gt1;

			if( ABS(ap[i0].u) < ABS(ap[i1].u) )
				ap[i0].u = ap[i1].u;


			ap[i1].state &= 0x01;
		}
	}




	for( i = 0, j = 0 ; i < *nP ; i++ ){
		if( ap[i].state & 0x02 )
			ap[j++] = ap[i];
	}



	*nP = j;
}




static void
pseg_filter_in(pseg_type ap[], int *nP )
{
	int	i,	j;



	for( i = 0, j = 0 ; i < *nP ; i++ ){
		if( ap[i].u < 0 && ABS(ap[i].gt1 - ap[i].gt0) > 0 )
			ap[j++] = ap[i];
	}


	*nP = j;
}


static void
	pseg_filter_len( pln_type *pl, pseg_type ap[], int *nP )
{
	int	i,	j;



	for( i = 0, j = 0 ; i < *nP ; i++ ){
		pseg_type *s = &ap[i];
		float len;
		if( s->gt0 > s->gt1 )
			len = pl->len - s->gt0 + s->gt1;
		else len = s->gt1 - s->gt0;

		//	float len = ap[i].gt1 - ap[i].gt0;
		if( len > 0 && len < 4 )	continue;

		ap[j++] = ap[i];
	}
	*nP = j;
}

static int
pln_set_start( pln_type *pl )
{
ln_type	*l,	*l1;
vec2f_type	ctr;
float	len;

	l1 = lnL_last( pl->link );
	len = MIN( pl->link->len, l1->len );
	if( len > 6 )	return( 1 );


	for( l = pl->link, ctr = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){
		l1 = LN_NEXT( l );

		if( l1 == NULL )	break;

		ctr.x += l->v.x;
		ctr.y += l->v.y;

		len = MIN( l->len, l1->len );
		if( len < 6 )	continue;


		ln_type *l2 = lnL_last( pl->link );
		l2->p[1] = pl->link;
		pl->link->p[0] = l2;


		pl->link = l1;
		pl->ctr = ctr;
		l->p[1] = NULL;
		l1->p[0] = NULL;

		return( 1 );
	}

	return( 1 );
}