
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"



#include "Uln/PlnType.h"


#include	"ImageType/ImageType.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageDump/ImageDump.h"






int	pln_aa( plnA_type *apl, plnA_type *bpl);

pt2dA_type *	pt2dA_max( pt2dA_type *apt );

int		pt2dA_delete( pt2dA_type *apt, pt2dA_type *mapt, float dT, int in );


static int	pt2dA_thin_in( pt2dA_type *apt, int i0, int i1 );



int		pln_thin( pln_type *pl, int iFrame );

void	pt2dA_crop( pln_type *pl, pt2dA_type *apt );



int
plnA_adjust_thin( plnA_type *apl, int iFrame )
{
int	i,	j,	ret;
	
	ret = 0;

	for( i = 0 ; i < apl->nA ; i++ )
		ret += pln_thin( apl->a[i], iFrame );


	for( i = 0, j = 0 ; i < apl->nA ; i++ ){
		if( apl->a[i]->len < 64 ){
			pln_destroy( apl->a[i] );
			continue;
		}
		apl->a[j++] = apl->a[i];
	}

	apl->nA = j;


	return( ret );
}


int
pln_thin( pln_type *pl, int iFrame )
{


	int n = pl->len;



	pt2dA_type *apt = pt2dA_alloc( n +10 );
	pln_sampleN( pl, 1, 1, apt );
	pt2dA_curvtur( apt );

	apt->type = PT2D_4V;

	PT2DA_DUMP( apt, "thin", iFrame, NULL );

	pt2dA_type *capt = pt2dA_max( apt );

	PT2DA_DUMP( capt, "thin", iFrame, "extremom" );


	int k = pt2dA_delete( apt, capt, 8.0, 1 );
	
	k += pt2dA_delete( apt, capt, 8.0, 0 );

	if( k > 0 ){
		pt2dA_crop(pl, apt );
		PLN_DUMP( pl, "thin", iFrame, NULL );
	}

	pt2dA_destroy( apt );
	pt2dA_destroy( capt );


	return( k );

}

int	pt2dA_update_start( pt2dA_type *apt, int *i1 );

int	pt2dA_update_end( pt2dA_type *apt, int *i0 );


void
pt2dA_crop( pln_type *pl, pt2dA_type *apt )
{
	int	i;

	for( i = apt->nA-1 ; i >= 0 ; ){
		for( ; i > 0 && apt->a[i].id >= 0 ; i-- );


		if( i == 0 )	return;
		
		int i0 = i;

		for( ; i > 0 && apt->a[i].id < 0 ; i-- );
		int i1 = i+1;

		pt2dA_update_end( apt, &i0 );
		pt2dA_update_start( apt, &i1 );

		pln_type *pl1 = pln_split( pl, apt->a[i0].f, 0.5 );

		pln_trim( pl, F_END, apt->a[i0].f - apt->a[i1].f );

		pln_append( pl, pl1 );

	}
}

int
	pt2dA_update_start( pt2dA_type *apt, int *i1 )
{
	int i,	i0,	iMax;
	float t,	tMax;
	i0 = *i1 - 4;
	iMax = -1;
	if( i0 < 1 )	i0 = 1;
	for( i = *i1 ; i > i0 ; i-- ){
		t = VEC2D_INNER( apt->a[i].n, apt->a[i-1].n );
		if( iMax < 0 || t > tMax ){
			tMax = t;
			iMax = i;
		}
	}

	*i1 = iMax;

	return( 1 );
}

int
	pt2dA_update_end( pt2dA_type *apt, int *i0 )
{
	int i,	i1,	iMax;
	float t,	tMax;
	i1 = *i0 + 4;
	iMax = -1;
	if( i1 >= apt->nA )	i1 = apt->nA-1;
	for( i = *i0 ; i < i1 ; i++ ){
		t = VEC2D_INNER( apt->a[i].n, apt->a[i+1].n );
		if( iMax < 0 || t > tMax ){
			tMax = t;
			iMax = i;
		}
	}

	*i0 = iMax;

	return( 1 );
}


pt2dA_type *
pt2dA_max( pt2dA_type *apt )
{
	pt2dA_type	*capt = pt2dA_alloc( apt->nA );
	int	i;

	for( i = 10 ; i < apt->nA - 10 ; i++ ){
		pt2d_type *pt = &apt->a[i];

		if( ABS(pt->r) < 0.1 )	continue;

		if( ABS(pt->r) > ABS( apt->a[i-1].r) && ABS(pt->r) > ABS( apt->a[i+1].r) ){
			capt->a[capt->nA] = *pt;
			capt->a[capt->nA].id = i;
			capt->nA++;
		}
	}

	return( capt );
}

int	pt2dA_distanceA( pt2dA_type *apt, int i0, float dT, float *dis );


int 
pt2dA_delete( pt2dA_type *apt, pt2dA_type *mapt, float dT, int in )
{
int i,	j,	n;
float dis;

	for( i = 0, n = 0 ; i < mapt->nA ; i++ ){
		pt2d_type *pt = &mapt->a[i];
		if( pt->id < 0 )	continue;

		int k = pt2dA_distanceA( mapt, i, dT, &dis );
		
		if( k < 0 )		continue;

		//if( dis > dT*dT )	continue;

	

		if( in == 1 &&  pt2dA_thin_in( apt, pt->id, mapt->a[k].id ) < 0 )
			continue;

		if(  mapt->a[k].id - pt->id > 0.5 *apt->nA )
			continue;

		for( j = pt->id ; j <= mapt->a[k].id ; j++ )
			apt->a[j].id = -1;


		float dt = pt->f - mapt->a[k].f;
//		if( ABS( dt ) < 2*dT )	continue;
#ifdef _DEBUG
		GPLOG( ("%d %d   %f %f\n", i, k, dis, dt) );
#endif

		for( j = i+1 ; j < k ; j++ ){
			mapt->a[j].id = -1;
		}
		i = k;

		n++;
	}

	return( n );
}

static int
pt2dA_thin_in( pt2dA_type *apt, int i0, int i1 )
{
int	i,	no;

	vec2f_type	v,	u;
	v.x = apt->a[i1].p.x - apt->a[i0].p.x;
	v.y = apt->a[i1].p.y - apt->a[i0].p.y;
	VEC2D_NORM2( v );
	VEC2D_LEFT( v, u );
	VEC2D_NORMALIZE( u );

	float d = VEC2D_INNER( apt->a[i0].p, u );

	int n = no = 0;
	for( i = i0 + 1 ; i <= i1-1 ; i++ ){
		pt2d_type *pt = &apt->a[i];

		float t =  VEC2D_INNER( pt->p, u ) - d;
		if( t > 0 )
			n++;
		no++;
	}

	if( n > 0.5*no )
		return( 1 );

	return( -1 );
}




int
pt2dA_distanceA( pt2dA_type *apt, int i0, float dT, float *dis )
{
	vec2d	v;
	float	d;
	int	i,	iMin;

	pt2d_type *pt0 = &apt->p[i0];

	iMin = -1;
	*dis = dT*dT;
	for( i = i0+1 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		if( pt->id < 0 )	continue;

		float dt = pt->f - pt0->f;
		if( dt < dT )	continue;  

		v.x = pt->p.x - pt0->p.x;
		v.y = pt->p.y - pt0->p.y;

		d = v.x*v.x + v.y*v.y;
		if(  d > *dis && d > dT*dT )
			continue;

		if( dt*dt < 4*d )
			continue;

		*dis = d;
		iMin = i;
	}

	
	return( iMin );
}