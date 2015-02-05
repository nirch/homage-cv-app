
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#ifdef _DUMP
#define SEG_PRINT( title, as, nS, fp )  seg_printf( title, as, nS, fp );
#else 
#define SEG_PRINT( title, as, nS, fp ) 
#endif


#include	"Uigp/igp.h"
//#include	"Utime\GpTime.h"
#include	"Ulog/Log.h"

   
#include "Uln/PlnType.h"

#include "PlnAdjust.h"


typedef struct eseg_type {
	int i0;
	int i1;

	int	id;
	//float d;

	float dt;

	int type;

	int	state;

} eseg_type;


int	pln_adjust_edge( pln_type *pl, plnA_type *eapl, float height, float ht[2], int iFrame );

static int	pt2dA_distane_plnA( pt2dA_type *apt, plnA_type *apl );
static int	pt2dA_distane_update( pt2dA_type *apt, int i0, int i1, plnA_type *apl, int iPl );


static int	pln_range( pln_type *pl, float h, float *gt0, float *gt1 );


static int	pt2dA_seg( pt2dA_type *apt, eseg_type as[], int *nS );

static void	seg_printf( char *title, eseg_type *as, int nS, FILE *fp  );

static int	pt2dA_seg_ended( pt2dA_type *apt, eseg_type as[], int *nS );


static int	pt2dA_seg_finish( pln_type *pl, pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int nS );


static int	pt2dA_seg_union( pt2dA_type *apt, plnA_type *apl, eseg_type as[], int *nS );


static int	pt2dA_seg_filter_order( pt2dA_type *apt, eseg_type as[], int *nS );
static int	pt2dA_seg_filter_order( pt2dA_type *apt, eseg_type *s, eseg_type as[2] );








static int	pt2dA_seg_filter_overlap( pt2dA_type *apt, eseg_type as[], int *nS );

static int	pt2dA_seg_filter_overlap( pt2dA_type *apt, eseg_type as[], int nS, int i0 );

static int	pt2dA_seg_union2( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS );

static int	pt2dA_seg_union3( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS );



static int	seg_overlap( pt2dA_type *apt, eseg_type *s0, eseg_type *s );


static int	pt2dA_seg_filter_thin( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS );


static int	pt2dA_seg_filter_head( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS, float ht[] );

static int	pt2dA_seg_filter_head_gap( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS, float ht[] );



static int	pt2dA_seg_filter_short( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS );


static void	pln_append_smart( pln_type *pl, pln_type *pl1 );


static int	segA_test( pln_type *bpl, plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS );


int
plnA_adjust_edge( plnA_type *apl, plnA_type *eapl, float height, float ht[2], int iFrame )
{
	if( apl->nA <= 0 )	return( 1 );

	int ret = pln_adjust_edge( apl->a[0], eapl, height, ht, iFrame );

	return( ret );
}


int
pln_adjust_edge( pln_type *pl, plnA_type *eapl, float height, float ht[2], int iFrame )
{
float gt0, gt1;

	pln_range( pl, height, &gt0, &gt1 );

	pt2dA_type *apt = pln_sampleP( pl, gt0, gt1, 1.0, NULL );

	pt2dA_distane_plnA( apt, eapl );

	apt->type = PT2D_4V;
	PT2DA_DUMP( apt, "edge", iFrame, "aa" );

	eseg_type	as[256];
	int nS;

	pt2dA_seg( apt, as, &nS );
	SEG_PRINT( "seg", as, nS, stdout );


	pt2dA_seg_filter_overlap( apt, as, &nS );
	SEG_PRINT( "overlap", as, nS, stdout );



	pt2dA_seg_union( apt, eapl, as, &nS );
	SEG_PRINT( "union", as, nS, stdout );



	pt2dA_seg_ended( apt, as, &nS );
	SEG_PRINT( "end", as, nS, stdout );

	pt2dA_seg_filter_order( apt, as, &nS );
	SEG_PRINT( "order", as, nS, stdout );




	int i;
	for( i = 0 ; i < nS ; i++ )
		as[i].type = 0;


	pt2dA_seg_filter_head( eapl, apt,  as, &nS, ht );
	SEG_PRINT( "head", as, nS, stdout );
	pt2dA_seg_filter_head_gap( eapl, apt,  as, &nS, ht );


	pt2dA_seg_union2( apt, eapl, as, &nS );
	SEG_PRINT( "union2", as, nS, stdout );


	pt2dA_seg_union3( apt, eapl, as, &nS );
	SEG_PRINT( "union3", as, nS, stdout );


	pt2dA_seg_filter_thin( eapl, apt, as, &nS );
	SEG_PRINT( "thin", as, nS, stdout );










	pt2dA_seg_filter_short( eapl, apt,  as, &nS );
	SEG_PRINT( "short", as, nS, stdout );



	segA_test(  pl, eapl, apt,  as, &nS);
	SEG_PRINT( "test", as, nS, stdout );

	pt2dA_seg_filter_head_gap( eapl, apt,  as, &nS, ht );


	pt2dA_seg_finish( pl, apt, eapl, as, nS );


//	PLN_DUMP( pl, "pl",1, NULL );

	return( 1 );

}




static int	pt2dA_update_1( pln_type *pl, pt2dA_type *apt, int i0, int *i1 );

static int	pt2dA_update_0( pln_type *pl, pt2dA_type *apt, int *i0, int i1 );



static int
	pt2dA_seg_finish( pln_type *pl, pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int nS )
{
	int	i;
	pln_type	*apl[128];
	eseg_type *s;

	int n = 0;


	if( nS == 0 )	return( 1 );


	for( i = 0 ; i < nS  ; i++ ){
		s = &as[i];

		pln_type *epl;
		if( apt->a[s->i0].r > apt->a[s->i1].r ){
			epl = pln_copy_sub( eapl->a[s->id], apt->a[s->i1].r, apt->a[s->i0].r );
			pln_inverse( epl );
		}
		else
			epl = pln_copy_sub( eapl->a[s->id], apt->a[s->i0].r, apt->a[s->i1].r );

		apl[n++] = epl;
	}




	for( i = 0 ; i < nS-1  ; i++ ){
		int i0 = as[i].i1;
		int i1 = ( as[i+1].i0 < as[i+1].i1 )? as[i+1].i0 : as[i+1].i1;

		i0 += 5;
		i1 -= 5;

		pt2dA_update_0( apl[0], apt, &i0, i1 );
		pt2dA_update_1( apl[i+1], apt, i0, &i1 );

		vec2f_type p0,	v0,	dp;
		pln_tangetP( apl[0], apl[0]->len, &p0, &v0 );


		dp.x = apl[i+1]->ctr.x - p0.x;
		dp.y = apl[i+1]->ctr.y - p0.y;

		float t = hypot( dp.x, dp.y );


		if( t > 15 && apt->a[i1].f - apt->a[i0].f > 12 && as[i].type != 1 ){
			PLN_DUMP( apl[0], "AA", i, "FINISH-pre" );
			pln_type *tpl = pln_copy_sub( pl, apt->a[i0].f, apt->a[i1].f );
			pln_append( apl[0], tpl );

			PLN_DUMP( apl[0], "AA", i, "FINISH" );
		}

//		pln_append( apl[0], apl[i+1] );
		PLN_DUMP( apl[0], "AA", 0, "FINISH" );
		PLN_DUMP( apl[i+1], "AA", i+1, "FINISH" );
		pln_append_smart( apl[0], apl[i+1] );
	}




	//int i1 = as[nS-1].i1+5;
//	if( i1 > apt->nA-1 )	i1 = apt->nA -1;
	//if( i1 < apt->nA -1 ){
	int i1 = as[nS-1].i1;
	if( apt->a[i1].f + 5 < pl->len ){
		pln_type *tpl = pln_copy_sub( pl, apt->a[i1].f+5, pl->len );
		pln_append( apl[0], tpl );
	}
//	else	pln_close( apl[0], 1 );



	s = &as[0];
	float t = apt->a[s->i0].f - 5;
	if( t < 1.0 )	t = 1.0;
	pln_trim( pl, F_END, pl->len - t );


	pln_append( pl, apl[0] );

	pln_close( pl, 1 );

	return( 1 );
}




static int
pt2dA_update_1( pln_type *pl, pt2dA_type *apt, int i0, int *i1 )
{
	int i;
	vec2f_type	v,	tv;

	float tMax;
	int iMax = -1;

	float	t;

	pln_tanget( pl, 0, &tv );

	for( i = *i1 ; i > i0 && i > *i1 - 10 ; i-- ){


		v.x = pl->ctr.x - apt->a[i].p.y;
		v.y = pl->ctr.y - apt->a[i].p.x;

		VEC2D_NORMALIZE( v );
	

		t = VEC2D_INNER( tv, v );


		if( iMax < 0 || t > tMax ){
			tMax = t;
			iMax = i;
		}


		if( t > 0.4 ){
			*i1 = i;
			return( 1 );
		}

	}

	if( iMax > 0 )
		*i1 = iMax;
//	*i1 = i;


	return( 1 );
}


static int
	pt2dA_update_0( pln_type *pl, pt2dA_type *apt, int *i0, int i1 )
{
	int i;
	vec2f_type	v,	tv,	tp;
	float tMax;
	int iMax = -1;

	float	t;


	
	pln_tangetP( pl, pl->len, &tp, &tv );

	vec2f_type	lv;
	VEC2D_LEFT( tv, lv );

	float lt0 = 0;
	for( i = *i0 ; i < i1 && i < *i0 + 10 ; i++ ){
		v.x = apt->a[i].p.y - tp.x;
		v.y = apt->a[i].p.x - tp.y;
		float lt = VEC2D_INNER( lv, v );
		if( i > *i0 && (lt0<0) != (lt<0) ){
			*i0 = i;
			break;
		}
		lt0 = lt;
	}


	for( i = *i0 ; i < i1 && i < *i0 + 10 ; i++ ){


		v.x = apt->a[i].p.y - tp.x;
		v.y = apt->a[i].p.x - tp.y;

		VEC2D_NORMALIZE( v );


		t = VEC2D_INNER( tv, v );

		if( iMax < 0 || t > tMax ){
			tMax = t;
			iMax = i;
		}

		if( t > 0.4 ){
			*i0 = i;
			return( 1 );
		}

	}

	if( iMax > 0 )
		*i0 = iMax;


	return( 1 );
}




static int
pt2dA_distane_plnA( pt2dA_type *apt, plnA_type *apl )
{
int	i;
vec2f_type p;
dPln_type d;
pln_type *spl;

	for( i = 0 ; i < apt->nA ; i++ ){
		pt2d_type *pt = &apt->a[i];

		p.x = pt->p.y;
		p.y = pt->p.x;
	

		pt->group = plnA_distance( apl, &p, 10.0, &spl, &d );
		pt->n.x = 1.0;
		pt->n.y = 0;
//		pt->f = d.u;
		pt->n.y = d.u;

		if( pt->group < 0 )	continue;

		pt->r = d.gt;
	}

	return( 1 );
}


static int
pt2dA_distane_update( pt2dA_type *apt, int i0, int i1, plnA_type *apl, int iPl )
{
	int	i;
	vec2f_type p;
	dPln_type d;


	for( i = i0 ; i < i1 ; i++ ){
		pt2d_type *pt = &apt->a[i];

		p.x = pt->p.y;
		p.y = pt->p.x;


		pt->group = pln_distance( apl->a[iPl], &p, &d );
		pt->n.x = 1.0;
		pt->n.y = 0;
		pt->n.y = d.u;

		pt->group = iPl;

		pt->r = d.gt;
	}

	return( 1 );
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

		if( v.x > h - 0.1 && *gt1 > 10.0 && v.y > pl->ctr.y )
			break;
	}


	return( 1 );
}


static int	pt2dA_seg_ended( pt2dA_type *apt, eseg_type *s );

static int
pt2dA_seg_ended( pt2dA_type *apt, eseg_type as[], int *nS )
{
int	i,	j;

	for( i =0, j =0 ; i < *nS ; i++ ){
		pt2dA_seg_ended( apt, &as[i] );
		if( as[i].i1 - as[i].i0 > 8 )
			as[j++] = as[i];
	}

	*nS = j;

	return( 1 );
}


static int
pt2dA_seg_ended( pt2dA_type *apt, eseg_type *s )
{
int	i;

	for( i = s->i0 ; i < s->i1 - 4 ; i++ ){
		float t1 = apt->a[i+2].r - apt->a[i+1].r;
		float t0 = apt->a[i+1].r - apt->a[i].r;

		float t2 = t0 + t1;
		if( ABS( t2 ) > 1.5 )	break;
	}

	s->i0 = i;

	for( i = s->i1 ; i > s->i0 + 4 ; i-- ){
		float t1 = apt->a[i-2].r - apt->a[i-1].r;
		float t0 = apt->a[i-1].r - apt->a[i].r;

		float t2 = t0 + t1;
		if( ABS( t2 ) > 1.5 )	break;
	}

	s->i1 = i;

	return( 1  );
}





static int
pt2dA_seg_union( pt2dA_type *apt, plnA_type *apl, eseg_type as[], int *nS )
{
	int	i,	j;

	for( i =1, j =0 ; i < *nS ; i++ ){
		if( as[j].id != as[i].id ){
			as[++j] = as[i];
			continue;
		}

		int i1 = as[i].i0;
		int i0 = as[j].i1;


		float dt = apt->a[i1].r - apt->a[i0].r;


		
		if( ABS(dt) < 20 ){
			as[j].i1 = as[i].i1;
			pt2dA_distane_update( apt, i0+1, i1-1, apl, as[j].id );
			continue;
		}

		as[++j] = as[i];
	}

	*nS = j+1;

	return( 1 );
}


static int	pt2dA_seg_union2( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS, int i0 );

static int
pt2dA_seg_union2( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS )
{
	int	i;

	for( i = 0 ; i < *nS ;  ){

		if( pt2dA_seg_union2( apt, eapl, as, nS, i ) < 0 )
			i++;
	}

	return( 1 );
}


static int
pt2dA_seg_union2( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS, int i0 )
{
	int	i;

	if( as[i0].type == 2 )
		return( -1 );

	for( i = i0 + 1 ; i < *nS ; i++ ){
		if( as[i].type == 2 )
			return( -1 );

		if( as[i0].id == as[i].id )	break;
	}

	if ( i >= *nS )	return( -1 );



	int k1 = as[i].i0;
	int k0 = as[i0].i1;

	int dk0 = as[i0].i1 - as[i0].i0;
	int dk1 = as[i].i1 - as[i].i0;
	int dk = MIN( dk0, dk1 );



	if( k1 - k0 > 0.4 * apt->nA )
		return( -1 );


	float dt = apt->a[k1].r - apt->a[k0].r;

	vec2f_type	v0	,p0,	v1,	p1;
	pln_tangetP( eapl->a[as[i0].id], apt->a[k0].r, &p0, &v0 );
	if( apt->a[k0].r < apt->a[as[i0].i0].r ){
		v0.x = -v0.x;
		v0.y = -v0.y;
	}

	pln_tangetP( eapl->a[as[i0].id], apt->a[k1].r, &p1, &v1 );

	if( apt->a[k1].r > apt->a[as[i].i1].r ){
		v1.x = -v1.x;
		v1.y = -v1.y;
	}

	vec2f_type	fp;
	fp.x = p1.x -  p0.x;
	fp.y = p0.y -  p0.y;
	float df = hypot( fp.x, fp.y );
	fp.x /= df;
	fp.y /= df;

	float t = VEC2D_INNER( v0, v1 );
	float t0 = VEC2D_INNER( v0, fp );
	float t1 = VEC2D_INNER( v1, fp );

	if( t0 > 0.95 && t0 > 0.95 && t1 > 0.95 && df < 1.1 * ABS(dt) && df < 60 ){

		pt2dA_distane_update( apt, k0+1, k1-1, eapl, as[i0].id );

		as[i0].dt = ( as[i0].dt*(as[i0].i1 - as[i0].i0 + 1)   + as[i].dt*(as[i].i1 - as[i].i0 + 1) )/( as[i].i1 - as[i0].i0 + 1 );



		as[i0].i1 = as[i].i1;

		int di = i - i0;
		(*nS) -=  di;

		for( i = i0 +1 ; i < *nS ; i++ )
			as[i] = as[i+di];

		return( 1 );
	}




	vec2f_type	dp;
	dp.x = apt->a[k1].p.x -  apt->a[k0].p.x;
	dp.y = apt->a[k1].p.y -  apt->a[k0].p.y;
	float t2 = hypot( dp.x, dp.y );

	float t3 =  apt->a[k1].r -  apt->a[k0].r;
	VEC2D_NORMALIZE( dp );


	VEC2D_SWAP(dp);

	 t0 = VEC2D_INNER( v0, dp );
	 t1 = VEC2D_INNER( v1, dp );




	 t = VEC2D_INNER( v0, v1 );
	if( ABS(t3) > 1.5 * t2 )
		return( -1 );

	if( dt < 0 )	dt =-dt;
	if(  dt > 60 ||  ( dt > 40 && dt > dk ) )
		return( -1 );



	as[i0].dt = ( as[i0].dt*(as[i0].i1 - as[i0].i0 + 1)   + as[i].dt*(as[i].i1 - as[i].i0 + 1) )/( as[i].i1 - as[i0].i0 + 1 );
	as[i0].i1 = as[i].i1;

	pt2dA_distane_update( apt, k0+1, k1-1, eapl, as[i0].id );

	int di = i - i0;
	(*nS) -=  di;

	for( i = i0 +1 ; i < *nS ; i++ )
		as[i] = as[i+di];



	return( 1 );
}


static int	pt2dA_seg_union3( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS, int i0 );

static int
pt2dA_seg_union3( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS )
{
	int	i;

//	for( i = 0 ; i < *nS ; i++ )
//		as[i].type = 0;

	for( i = 0 ; i < *nS ; i++ ){

		pt2dA_seg_union3( apt, eapl, as, nS, i );
	}

	return( 1 );
}

static int
pt2dA_seg_union3( pt2dA_type *apt, plnA_type *eapl, eseg_type as[], int *nS, int i0 )
{
	int	i,	k1;
	float	t;

	if( as[i0].type == 2 )
		return( -1 );


	int k0 = as[i0].i1;
	int dk0 = as[i0].i1 - as[i0].i0;
	vec2f_type	dp;

	vec2f_type	v0	,v1;
	for( i = i0 + 1 ; i < *nS ; i++ ){
		k1 = as[i].i0;
		dp.x = apt->a[k1].p.x -  apt->a[k0].p.x;
		dp.y = apt->a[k1].p.y -  apt->a[k0].p.y;

		if( dp.x*dp.x + dp.y*dp.y > 20*20 )
			continue;


		pln_tanget( eapl->a[as[i0].id], apt->a[k0].r, &v0 );
		if( apt->a[k0].r < apt->a[as[i0].i0].r ){
			v0.x = -v0.x;
			v0.y = -v0.y;
		}

		pln_tanget( eapl->a[as[i].id], apt->a[k1].r, &v1 );

		if( apt->a[k1].r > apt->a[as[i].i1].r ){
			v1.x = -v1.x;
			v1.y = -v1.y;
		}
		t = VEC2D_INNER( v0, v1 );
		if( t > 0.88 )	break;

	}


	if ( i >= *nS )
		return( -1 );



	int dk1 = as[i].i1 - as[i].i0;
	int dk = MIN( dk0, dk1 );



	if( k1 - k0 > 0.4 * apt->nA )
		return( -1 );


	float t2 = hypot( dp.x, dp.y );


	float dt = apt->a[k1].r - apt->a[k0].r;






	VEC2D_SWAP(dp);

	float t0 = VEC2D_INNER( v0, dp );
	float t1 = VEC2D_INNER( v1, dp );




	if( t < 0.9 )
		return( -1 );


	as[i0].type = 1;

	if( i == i0 + 1 )
		return( 1 );


	fprintf( stdout, "M" );
//	as[i0].type = 1;

	int di = i - i0 - 1;
	(*nS) -=  di;

	for( i = i0 + 1 ; i < *nS ; i++ )
		as[i] = as[i+di];



	return( 1 );
}





static int
pt2dA_seg_filter_order( pt2dA_type *apt, eseg_type as[], int *nS )
{
	int	i,	j,	k,	n;

	eseg_type bs[2];

	for( i =0, j =0 ; i < *nS ; i++ ){


		if( (n = pt2dA_seg_filter_order( apt, &as[i], bs )) < 0 )
			continue;
		as[j++] = as[i];

		if( n == 2 ){
			if( j > i ){
				*nS += 1;
				for( k = *nS ; k > j ; k--)
					as[k] = as[k-1];
			}
			as[j++] = bs[1];
			if( j > i + 1 )	i++;
		}
	}

	*nS = j;

	return( 1 );
}





static int
pt2dA_seg_filter_order( pt2dA_type *apt, eseg_type *s, eseg_type as[2] )
{
int	i,	j,	iMax,	iMin;

	iMax  = iMin = s->i0;
	for( i = s->i0+1 ; i <= s->i1 ; i++ ){
		if( apt->a[i].group != s->id )	continue;
		if( apt->a[iMax].r < apt->a[i].r )
			iMax = i;

		if( apt->a[iMin].r > apt->a[i].r )
			iMin = i;
	}


	int m0 = iMax - 0.5*(s->i1 + s->i0);
	int m1 = iMin - 0.5*(s->i1 + s->i0);
	if( ABS(m1) < ABS(m0) )
		iMax = iMin;



	if( iMax < s->i0 + 2 ){
		s->i0 = iMax;
		return( 1 );
	}

	if( iMax > s->i1 - 2 ){
		s->i1 = iMax;
		return( 1 );
	}


	as[0].i0 = s->i0;
	as[0].i1 = iMax-1;
	as[0].id = s->id;
	as[0].dt = 0;
	for( i = as[0].i0 ; i <= as[0].i1 ; i++ )
		as[0].dt += ABS( apt->a[i].n.y );
	as[0].dt = as[0].dt / ( as[0].i1 - as[0].i0 +1 );


	as[1].i0 = iMax+1;
	as[1].i1 = s->i1;
	as[1].id = s->id;
	as[1].dt = 0;
	for( i = as[1].i0 ; i <= as[1].i1 ; i++ )
		as[1].dt += ABS( apt->a[i].n.y );
	as[1].dt = as[1].dt / ( as[1].i1 - as[1].i0 +1 );


	if( seg_overlap( apt, &as[0], &as[1] ) > 0 ){

		if( as[0].dt < as[1].dt )
			*s = as[0];
		else *s = as[1];

		float dt = apt->a[s->i1].r - apt->a[s->i0].r;
		if( s->i1 - s->i0 < 6 || ABS(dt) < 10 )
			return( -1 );

		return( 1 );
	}



	for( i = 0, j = 0 ; i < 2 ; i++ ){
		float dt = apt->a[as[i].i1].r - apt->a[as[i].i0].r;
		if( as[i].i1 - as[i].i0 < 6 || ABS(dt) < 10 )
			continue;

		as[j++] = as[i];
	}


	if( j == 0 )	return( -1 );
	
	*s = as[0];

	return( j );
}




static int
pt2dA_seg_filter_overlap( pt2dA_type *apt, eseg_type as[], int *nS )
{
	int	i,	j;

	for( i =0, j = 0 ; i < *nS ; i++ ){
		if( as[i].id < 0 )	continue;

		if( pt2dA_seg_filter_overlap( apt, as, *nS, i  ) < 0 )
			continue;

		as[j++] = as[i];
	}

	*nS = j;

	return( 1 );
}


static int
pt2dA_seg_filter_overlap( pt2dA_type *apt, eseg_type as[], int nS, int i0 )
{
	int	i;
	eseg_type	*s0 = &as[i0];
	for( i = i0+1 ; i < nS ; i++ ){
		eseg_type	*s = &as[i];
		if( s->id != s0->id )	continue;



		if( seg_overlap( apt, s0, s ) < 0 )
			continue;
#ifdef _AA_
		float t0 = apt->a[s->i0].r;
		float t1 = apt->a[s->i1].r;
		if( t0 > t1 )	{
			float tmp = t0;
			t0 = t1;
			t1 = tmp;
		}


		float t00 = apt->a[s0->i0].r;
		float t01 = apt->a[s0->i1].r;
		if( t00 > t01 )	{
			float tmp = t00;
			t00 = t01;
			t01 = tmp;
		}


		if( t1 < t00 || t01 < t0 )
			continue;
#endif

		if( s0->dt > s->dt ){
			s0->id = -1;
			return( -1 );
		}


		s->id = -1;
	}


	return( 1 );
}



static int
seg_overlap( pt2dA_type *apt, eseg_type *s0, eseg_type *s )
{


	float t0 = apt->a[s->i0].r;
	float t1 = apt->a[s->i1].r;
	if( t0 > t1 )	{
		float tmp = t0;
		t0 = t1;
		t1 = tmp;
	}


	float t00 = apt->a[s0->i0].r;
	float t01 = apt->a[s0->i1].r;
	if( t00 > t01 )	{
		float tmp = t00;
		t00 = t01;
		t01 = tmp;
	}


	if( t1-0.1 < t00 || t01-0.1 < t0 )
		return( -1 );


	return( 1 );
}



static int
pt2dA_seg( pt2dA_type *apt, eseg_type as[], int *nS )
{
int	i;
eseg_type	*s;

	
	*nS = 0;

	s = NULL;
	for( i = 0 ; i < apt->nA ;  ){

		for( ; i < apt->nA ; i++ ){
			if( apt->p[i].group >= 0 )
				break;
		}

		s = &as[*nS];
		s->i0 = s->i1 = i;
		s->id = apt->a[i].group;
		s->dt = ABS( apt->a[i].n.y );

		for( ; i < apt->nA ; i++ ){
			if( apt->p[i].group != s->id )
				break;

			s->dt += ABS( apt->a[i].n.y );
			s->i1 = i;
		}
		
		if( s->i1 - s->i0 > 4 ){
			s->dt = s->dt / ( s->i1 - s->i0 + 1 );
			*nS += 1;
		}

		s = NULL;
	}

	if( s != NULL && s->i1 - s->i0 > 4 )
		*nS += 1;

	return( *nS );
}

static void
seg_printf( char *title, eseg_type *as, int nS, FILE *fp  )
{
int i;
	
	fprintf( fp, "%s\n", title );

	for( i = 0 ; i < nS ; i++ )
		fprintf( fp, "%d %d %d\n", as[i].id, as[i].i0, as[i].i1 );
}



static int	pt2dA_seg_filter_thin( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int i0, int nS, float dT );


static int
pt2dA_seg_filter_thin( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS )
{
	int	i,	j;


	for( i =0, j =0 ; i < *nS ;  ){

		as[j++] = as[i];
		i = pt2dA_seg_filter_thin( apl, apt, as, i, *nS, 15 );
	}

	*nS = j;

	return( 1 );
}


static int
pt2dA_seg_filter_thin( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int i0, int nS, float dT )
{
int	i;

	eseg_type *s0 = &as[i0];
	

	for( i = i0+2 ; i < nS ; i++ ){
		eseg_type *s = &as[i];

		if( s->i0 - s0->i1 > 0.4 * apt->nA )
			return( i0 + 1);

		vec2f_type	p0,	p1,	dp;
	
		pln_gt2p( apl->a[s->id], apt->a[s->i0].r, &p0 );
		pln_gt2p( apl->a[s0->id], apt->a[s0->i1].r, &p1 );


	


		dp.x = p1.x - p0.x;
		dp.y = p1.y - p0.y;

		float t = hypot( dp.x, dp.y );

		if( t > dT )
			continue;

	



		return( i );

	}

	return( i0+1 );
}




static int	pt2dA_seg_filter_shortN( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int i0, int nS, float dT );


static int
pt2dA_seg_filter_shortN( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS )
{
	int	i,	j;


	for( i =0, j =0 ; i < *nS ;  ){

		if( pt2dA_seg_filter_shortN( apl, apt, as, i, *nS, 15 ) < 0 )
			continue;

		as[j++] = as[i];
	}

	*nS = j;

	return( 1 );
}


static int
	pt2dA_seg_filter_shortN( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int i0, int nS, float dT )
{
//	int	i;

	eseg_type *s0 = &as[i0];


	return( 1 );


}



static int
pt2dA_seg_filter_head( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS, float ht[] )
{
	int	i,	j,	k,	no;

	if( ht[0] < 0 )
		return( -1 );

	if( ht[0] > ht[1] ){
		return( -1 );
		//float tmp = ht[0];
		//ht[0] = ht[1];
		//ht[1] = tmp;
	}


	eseg_type	hs[2];
	

	for( i =0, j =0, no = 0 ; i < *nS ; i++ ){

		float gt0 = apt->a[as[i].i0].f;
		float gt1 = apt->a[as[i].i1].f;


		if( gt1 < ht[0]+5 || gt0 > ht[1]-5 ){
			as[j++] = as[i];
			continue;
		}


		int n = 0;
		if( ht[0] - gt0 > 16 ){
			hs[n] = as[i];
			hs[n].i1 = ht[0];
			hs[n].type = 0;

			pt2d_type *pt = &apt->a[hs[n].i1];
			dPln_type d;
			vec2f_type	p;
			p.x = pt->p.y;
			p.y = pt->p.x;
			pln_distance( apl->a[hs[n].id], &p, &d );
			pt->r = d.gt;
			n++;
		}
		else	if( j > 0 ){
			as[j-1].type = 0;
		}

		if( gt1 - ht[1] > 16 ){
			hs[n] = as[i];
			hs[n].i0 = ht[1];

			pt2d_type *pt = &apt->a[hs[n].i0];
			dPln_type d;
			vec2f_type	p;
			p.x = pt->p.y;
			p.y = pt->p.x;
			pln_distance( apl->a[hs[n].id], &p, &d );
			pt->r = d.gt;

			n++;
		}

		if( n == 0 )
			continue;

		if( n == 1 ){
			as[j++] = hs[0];
			continue;
		}

		as[j++] = hs[0];
		if( j > i ){
	
			for( k = *nS ; k > i ; k-- )
				as[k] = as[k-1];

			(*nS)++;
			i++;
		}

		as[j++] = hs[1];



//		if( j > 0 )
//			as[j-1].type = 0;


		n++;
	}

	*nS = j;





	return( no );
}


static int
pt2dA_seg_filter_head_gap( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS, float ht[] )
{
int	i;

	for( i =*nS-1 ; i >=0 ; i-- ){
		float gt1 = apt->a[as[i].i1].f;

		if( gt1 < ht[0]+5 ){
			as[i].type = 2;
			break;
		}
	}

	return( 1 );
}


static int
pt2dA_seg_filter_short( plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS )
{
	int	i,	j,	n;

	float t,	t0,	t1,	u0,	u1,	u;

	

	for( i =0, j =0, n = 0 ; i < *nS ; i++ ){

		if( as[i].type == 2 ){
			as[j++] = as[i];
			continue;
		}

		t = as[i].i1 - as[i].i0;

		t0 = ( i > 0 )?	as[i].i0 - as[i-1].i1 :  as[i].i0;
		t1 = ( i > 0 )?	as[i+1].i0 - as[i].i1 :  apt->nA - as[i].i1;

		u0 = apt->a[as[i].i0].n.y;
		u1 = apt->a[as[i].i1].n.y;

		if( u0 < 0 )	u0 = -u0;
		if( u1 < 0 )	u1 = -u1;

		u = MAX( u0, u1 );

		if( t < 24 &&  t < 0.75*MAX(t1, t0) && t < 1.5*MIN(t0, t1 ) && ABS(u) > 5 ){
			n++;
			continue;
		}

		if( j > 0 )
			as[j-1].type = 0;

		as[j++] = as[i];




//		float gt0 = apt->a[as[i].i0].f;
//		float gt1 = apt->a[as[i].i1].f;

	}

	*nS = j;

	if( n > 0 )
		fprintf( stdout, "S" );

	return( n );
}

static void
pln_append_smart( pln_type *pl, pln_type *pl1 )
{
	dPln_type d;
	pln_distance( pl, &pl1->ctr, &d );

	if( d.gt > 1.0 && d.gt < pl->len && ABS(d.u) < 10 )
		pln_trim( pl, F_END, pl->len - d.gt );

	vec2f_type	p;
	pln_end_point( pl, &p );

	pln_distance( pl1, &p, &d );

	if( d.gt > 0 && d.gt < pl1->len )
		pln_trim( pl1, F_BEGIN, d.gt );

	pln_append( pl, pl1 );

}


static int	seg_test( pln_type *bpl, plnA_type *apl, pt2dA_type *apt, eseg_type *s );
static int	seg_test_weak( pln_type *bpl, plnA_type *apl, pt2dA_type *apt, eseg_type *s );

static int
segA_test( pln_type *bpl, plnA_type *apl, pt2dA_type *apt, eseg_type as[], int *nS )
{
	int i,	j;
	for( i = 0 ; i < *nS ; i++ )
		as[i].state = 0;

	for( i = 0 ; i < *nS ; i++ ){
		seg_test( bpl, apl, apt, &as[i]  );
	}


	for( i = 0, j = 0 ; i < *nS ; i++ ){
		if( as[i].state == 0 ){
			if( seg_test_weak( bpl, apl, apt, &as[i]  ) > 0 ){
				if( as[i].type == 0 && i > 0 &&  i < *nS-1 ){
					if( as[i+1].type == 0  )
						as[i-1].type = 0;
					else as[i+1].type = 0;
				}
				continue;
			}
		}



		as[j++] = as[i];
	}

	*nS = j;

	return( 1 );
}




static int
seg_test( pln_type *bpl, plnA_type *apl, pt2dA_type *apt, eseg_type *s )
{
	int	i,	n,	no;

	float t,	t1, t2;


	float su = 0;
	for( i = s->i0, n = 0, no = 0 ; i <= s->i1 ; i++ ){
		 su += apt->a[i].n.y;
		 if( ABS(apt->a[i].n.y) < 1.5 )	no++;
		 n++;
	}

	float au = su / n;

	//if( ABS(u) < 1.0 )
	if( ABS(au) > 1.5 && no < 0.2*(s->i1 - s->i0))

		return( 1 );

	pln_type *pl = apl->a[s->id];

	
	for( i = s->i0 ; i < s->i0 + 15 && i < s->i1 ; i++ ){
		if( ABS(apt->a[i].n.y) < 1.25  ){
			t = apt->a[i].n.y - apt->a[i+1].n.y;
			if( ABS(t) < 0.25 )
				break;
		//if( ABS(apt->a[i].n.y) < 1.5 )	break;
		}
	}

	vec2f_type	v0,	v1,	v2,	v3;
	if( i > s->i0 ){
		pln_tanget( pl, apt->a[s->i0].r, &v0 );
		pln_tanget( pl, apt->a[i].r, &v1 );
		int	i1 = 0.5*( s->i0 + i );
		pln_tanget( pl, apt->a[i1].r, &v3 );

		t = VEC2D_INNER( v0, v1 );
		t2 = VEC2D_INNER( v0, v3 );
		if( ABS(t2) < ABS(t) )
			t = t2;

		pln_tanget( bpl, apt->a[i].f, &v2 );
		t1 = VEC2D_INNER( v1, v2 );

		if( ABS(t1) > ABS(t) && ABS(t)< 0.99 ){
			s->i0 = i;
			s->state |= 0x01;
		}
	}
	else s->state |= 0x01;


	for( i = s->i1 ; i > s->i1 - 10 && i > s->i0 ; i-- ){
		if( ABS(apt->a[i].n.y) < 1.25  ){
			t = apt->a[i].n.y - apt->a[i-1].n.y;
			if( ABS(t) < 0.25 )
				break;
		}
	}

	//vec2f_type	v0,	v1,	v2;
	if( i < s->i1 ){
		pln_tanget( pl, apt->a[s->i1].r, &v0 );
		pln_tanget( pl, apt->a[i].r, &v1 );

		//t = VEC2D_INNER( v0, v1 );

		int	i1 = 0.5*( s->i0 + i );
		pln_tanget( pl, apt->a[i1].r, &v3 );

		t = VEC2D_INNER( v0, v1 );
		t2 = VEC2D_INNER( v0, v3 );
		if( ABS(t2) < ABS(t) )
			t = t2;

		pln_tanget( bpl, apt->a[i].f, &v2 );
		t1 = VEC2D_INNER( v1, v2 );

		if( ABS(t1) > ABS(t) && ABS(t)< 0.99 ){
			s->i1 = i;
			s->state |= 0x02;
		}
	}
	else s->state |= 0x02;

	if( s->i1 - s->i0 < 6 )
		s->state = 0;


	return( 1 );


}

static int
	seg_test_weak( pln_type *bpl, plnA_type *apl, pt2dA_type *apt, eseg_type *s )
{
	int	i,	n,	no;

	float t,	t1;

	if( s->i1 - s->i0 > 16 )
		return( -1 );

	if( s->i1 - s->i0 < 6 )
		return( 1 );

	float su = 0;
	for( i = s->i0, n = 0, no = 0 ; i <= s->i1 ; i++ ){
		su += apt->a[i].n.y;
		if( ABS(apt->a[i].n.y) < 1.5 )	no++;
		n++;
	}

	float au = su / n;

	//if( ABS(u) < 1.0 )
	if( ABS(au) < 1.5 || no > 0.5*(s->i1 - s->i0))
		return( 1 );

	pln_type *pl = apl->a[s->id];



	vec2f_type	v0,	v1,	v2;


//	pln_tanget( bpl, apt->a[s->i0].f, &v0 );
//	pln_tanget( bpl, apt->a[s->i1].f, &v1 );

	pln_tangetS( bpl, apt->a[s->i0].f, &v0 );
	pln_tangetS( bpl, apt->a[s->i1].f, &v1 );

	int	i1 = 0.5*( s->i0 + s->i1 );
	pln_tangetS( bpl, apt->a[i1].f, &v2 );

	t1 = VEC2D_INNER( v0, v2 );


	t = VEC2D_INNER( v0, v1 );

	if( ABS(t) < 0.97 )
		return( -1 );


	return( 1 );
}



