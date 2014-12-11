
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Uigp/igp.h"
#include	"Utime\GpTime.h"
#include	"Ulog/Log.h"


#include "Uln/PlnType.h"

#include "PlnAdjust.h"



typedef struct seg_type {
	int i0;
	int i1;

	int	id;

	float d;

	float dt;
} seg_type;




static int	pln_coherent( pln_type *pl, pln_type *bpl, float height, int iFrame );

static int	pln_range( pln_type *pl, float h, float *gt0, float *gt1 );

static int	pln_adjust_dd( pln_type *pl, pln_type *bpl, float gt0, float gt1, lnFit_type *f );

static pln_type *	pln_adjust_do( pln_type *pl, pln_type *bpl, pt2dA_type *apt, seg_type *s );

static void	pln_adjust_fit_dump( pln_type *bpl, lnFit_type *af, int nA );



int
pln_coherent( plnA_type *apl, plnA_type *bpl, int height, int iFrame)
{
	int ret;

	if( apl->nA ==  0 || bpl->nA == 0 )
		return( 0 );

	ret = pln_coherent( apl->a[0], bpl->a[0], height, iFrame );


	return( ret );
}




int
pln_coherent( pln_type *pl, pln_type *bpl, float h, int iFrame )
{

	float gt,	gt0, gt1;

	int	i,	nA;

	float dt = 50;
	float dt1 = 70;

	//PLN_DUMP( bpl, "bpl", iFrame, NULL );
	//PLN_DUMP( pl, "pl", iFrame, NULL );


	pln_range( bpl, h, &gt0, &gt1 );

	int n = ( gt1 - gt0)/ dt + 2;

	lnFit_type *af = ( lnFit_type *)malloc( n*sizeof(lnFit_type));




	for( gt = gt0+dt, nA = 0 ; gt < gt1 - dt  ; gt += dt ){
		lt2_unity( &af[nA].lt );

		if( pln_fit_local( pl, bpl, gt, dt1, 1, 4,  &af[nA] ) < 0 )
			continue;


		

//		fprintf( stdout, "%d %f %f %f\n", apt->nA, af[i].dis, af[i].cover, hypot( af[i].lt.a0, af[i].lt.b0) );
#ifdef _DUMPA
		pln_type *tpl = pln_affine_lt( bpl, &af[nA].lt, NULL );
		PLN_DUMP( tpl, "tt", nA, "fit" );
		pln_destroy( tpl );
#endif




		nA++;

	}



	pln_adjust_fit_dump( bpl, af, nA );


	pln_type *tpl = NULL;


	int no = 0;
	for( i = nA-1 ; i >= 0 ; i-- ){
		lnFit_type *f = &af[i];



		float s = hypot( f->lt.a0, f->lt.b0 );
		if( s < 0.95 || s > 1.1 )	continue;


		gt = f->gt;
		if( pln_adjust_dd( pl, bpl, s*(gt-30), s*(gt+30), f ) > 0 ){
			fprintf( stdout, "G%d", iFrame );
			no++;
		}
	
	}



	free( af );


	return( no );

}


static int	pln_pt( pln_type *pl, pln_type *bpl, float gt0, float gt1, pt2dA_type *apt );

static int	pt2dA_seg(  pt2dA_type *apt, seg_type as[] );
static int	pt2dA_segT(  pt2dA_type *apt, seg_type as[] );


static int 
pln_adjust_dd( pln_type *pl, pln_type *bpl, float gt0, float gt1, lnFit_type *f )
{
	pln_type *tpl = pln_affine_lt( bpl, &f->lt, NULL );

	pt2dA_type *apt = pt2dA_alloc( 100 );

	pln_pt(  pl, tpl, gt0, gt1, apt );


	seg_type as[100];
	int nS = pt2dA_segT(  apt, as );

	int no = 0;
	int	i;

	for( i = nS-1 ; i >= 0  ; i-- ){
		seg_type *s = &as[i];


		if( s->dt > 24 ){
			s->i1 = s->i0 + 4;
			s->i0 = s->i0 - 4;
			pln_adjust_do( pl, bpl, apt, s );

			PLN_DUMP( pl, "TT", i, "AA" );
		//	fprintf( stdout, "G");
			no++;
			break;
		}

	}

	pt2dA_destroy( apt );

	return( no );
}



static int
pln_pt( pln_type *pl, pln_type *bpl, float gt0, float gt1, pt2dA_type *apt )
{
	int	n;
	vec2f_type	p;
	dPln_type	d;
	float	gt,	dt;

	
	n = 0;

	dt = 1.0;
	for( gt = gt0 ; gt < gt1 ; gt += dt, n++ ){

		pln_gt2p( bpl, gt, &p );

		if( pln_distance( pl, &p, &d ) < 0 )
			continue;

		if( d.gt < 0 || d.gt > pl->len )
			continue;

		pt2d_type *pt = &apt->a[apt->nA++];

		pt->p.x = gt;
		pt->p.y = d.u;
		pt->f = d.gt;
	}


	
	return( 1 );
}



static int
pt2dA_seg(  pt2dA_type *apt, seg_type as[] )
{
int	i;

	int nS = 0;

	pt2d_type *pt = &apt->a[0];

	for( i = 0 ; i < apt->nA ; i++ ){

		for( ; i < apt->nA ; i++, pt++ ){
			if( ABS(pt->p.y) > 2 )	break;
		}

		if( i >=  apt->nA )
			break;

		seg_type *s = &as[nS++];
		s->i0 = s->i1 = i;
		s->d = ABS(pt->p.y);

		for( ; i < apt->nA ; i++, pt++ ){
			if( ABS(pt->p.y) < 2 )	break;

			if( s->d < ABS( pt->p.y))
				s->d = ABS( pt->p.y);

			s->i1 = i;
		} 

		s->i1 = i-1;
	}


	return( nS );
}


static int
pt2dA_segT(  pt2dA_type *apt, seg_type as[] )
{
	int	i;

	int nS = 0;

	pt2d_type *pt = &apt->a[0];

	for( i = 5 ; i < apt->nA-5 ; i++ ){

		float dt =  apt->a[i+1].f - apt->a[i].f;

		if( dt > 3 ){
			seg_type *s = &as[nS++];
			s->dt = dt;
			s->i0 = i;
		}
	}

	return( nS );
}



static int
pln_range( pln_type *pl, float h, float *gt0, float *gt1 )
{

	vec2f_type v;
	ln_type *l;

	*gt0 = 0;
	*gt1 = 0;

	for( l = pl->link, v = pl->ctr ; l != NULL ; l = LN_NEXT(l) ){

		*gt1 += l->len;
		v.x += l->v.x;
		v.y += l->v.y;

		if( v.x > h - 0.1 )
			break;
	}


	return( 1 );
}




static pln_type *
	pln_adjust_do( pln_type *pl, pln_type *bpl, pt2dA_type *apt, seg_type *s )
{
	float gt0,	gt1;
	vec2f_type p[2],	bp[2];

	gt0 = apt->a[s->i0].f;
	gt1 = apt->a[s->i1].f;

	float dt = gt1 - gt0;
	if( ABS(dt) < 0.5 )	return( NULL);

	pln_gt2p( pl, gt0, &p[0] );
	pln_gt2p( pl, gt1, &p[1] );



	vec2f_type	v0,	v1;
	pln_tanget( pl, gt0, &v0 );
	pln_tanget( pl, gt1, &v1 );


	// test continuse
	vec2f_type	dp;
	dp.x = p[1].x - p[0].x;
	dp.y = p[1].y - p[0].y;
	float t = hypot( dp.x, dp.y );
	float t0 = VEC2D_INNER( v0, dp )/t;
	float t1 = VEC2D_INNER( v1, dp )/t;

//	if( t0 < 0.4 || t1 < 0.4 )
//		return( NULL );




	pln_type *pl0 = pln_copy_sub( pl, 0, gt0 );
	pln_type *pl1 = pln_copy_sub( pl, gt1, pl->len );


	pln_type *bpl0 = pln_copy_sub( bpl, apt->a[s->i0].p.x, apt->a[s->i1].p.x );
	bp[0] = bpl0->ctr;
	pln_end_point( bpl0, &bp[1] );

	lt2_type lt;
	lt2_similarityA( &lt, bp, p );
	pln_lt2( bpl0, &lt );



	pln_trim( pl, F_END, pl->len - gt0 );
	pln_append( pl, bpl0 );
	pln_append( pl, pl1 );

;

	return( pl );
}






static void
pln_adjust_fit_dump( pln_type *bpl, lnFit_type *af, int nA )
{
int	i;

	pt2dA_type *apt = pt2dA_alloc( nA );


	
	for( i = 0 ; i < nA ; i++  ){

		vec2f_type	p;
		pln_gt2p( bpl, af[i].gt, &p );

		

		vec2f_type	p1;
		p1.x = LT2_F1( af[i].lt, p.x, p.y );
		p1.y = LT2_F2( af[i].lt, p.x, p.y );

		pt2d_type *pt = &apt->a[apt->nA++];
		pt->p.y = p.x;
		pt->p.x = p.y;

		pt->r = af[i].gt;

		pt->n.y = p1.x - p.x;
		pt->n.x = p1.y - p.y;
		
	}

	apt->axis = PT2D_AXIS_XY;
	apt->type = PT2D_4V;
	PT2DA_DUMP( apt, "AA", 2, "AA" );

	pt2dA_destroy( apt );

}