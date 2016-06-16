



#include	<stdio.h>
#include	<math.h>


#ifdef _DEBUG
//#define	_DUMP
#endif

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"






#include "Umath/EigenType.h"
#include "Ucamera/Pt2dType.h"
#include "Uln/PlnType.h"


#include "PlnCurvature.h"

#include "CSegType.h"

#ifdef _AA_

typedef struct cseg_type {

	int	i0;
	int	i1;

	float u;
	int	iu;

	float	m0,	m1;
	float	su;

	int	state;

} cseg_type;



void	cseg_print( cseg_type *as, int nA );

#endif

int	pt2dA_straight_seg_r( pt2dA_type *apt, int cyclic, float T, cseg_type ap[], int *nP );

static int	pt2fA_straight_app(  pt2dA_type *apt, int i0, int i1, int nT, vl2f_type *bvl );



vl2fA_type *
plnA_straight( plnA_type *apl, plnCurvaturePrm_type *prm, int nT, vl2fA_type *avl )
{
int	i;

	avl = vl2fA_realloc( avl, 100 );

	avl->nA = 0;
	for( i = 0 ; i < apl->nA ; i++ )
		pln_straight( apl->a[i], prm, nT, avl );

	return( avl );
}

int
plnA_straight( plnA_type *apl, int i0, int i1, plnCurvaturePrm_type *prm, int nT, vl2fA_type *avl )
{
	int	i;

//	avl = vl2fA_realloc( avl, 100 );

//	avl->nA = 0;
	for( i = i0 ; i < i1 ; i++ )
		pln_straight( apl->a[i], prm, nT, avl );

	return( 1 );
}




int
	pln_straight( pln_type *pl, plnCurvaturePrm_type *prm, int nT, vl2fA_type *avl )
{
	int i;


	int N = 5;

	int cyclic = ( pl->state == PLN_CLOSE )? 1 : 0;

	PLN_DUMP( pl, "AA", 1, "Curvature-N"  );
	pt2dA_type *apt = pln_samplePV( pl, 0, pl->len, 1.0, NULL );





	pt2dA_curvature( apt, cyclic, N );//, prm );


	PT2DA_DUMP( apt, "PLN_S", 1, NULL );


	float	T = 0.185;
	float	T1 = 0.1;


	//cseg_type as[1024];
	cseg_type *as = ( cseg_type *)malloc( (apt->nA)*sizeof(cseg_type) );
	int nS;
	pt2dA_straight_seg_r(apt, cyclic, T, as, &nS );

#ifdef _DUMP
	cseg_print( as, nS );
#endif



	for( i = 0 ; i < nS ; i++ ){
		cseg_type *s = &as[i];
		if( s->i1 - s->i0 < nT )	continue;


		if( s->m0 > T1 || s->m1 < -T1 )
			continue;

		if( ABS(s->su) > prm->slT1 )
			continue;

		//if( ABS(s->su ) > prm->slT )
		//	continue;

		if( avl->nA >= avl->NA )
			vl2fA_realloc( avl, avl->nA + 16 );


		if( ABS(s->su ) > prm->slT ){
			if( pt2fA_straight_app(   apt, s->i0, s->i1, 4, &avl->a[avl->nA] ) > 0  && avl->a[avl->nA].d > 0.5*prm->slLen ){
				vl2f_swap( &avl->a[avl->nA] );
				avl->nA ++;

				continue;
			}

			continue;
		}


		if( ABS(s->su ) > prm->slT )
			continue;


		if( pt2d_approximate_line_vl( apt, s->i0, s->i1, &avl->a[avl->nA] ) > 0 && avl->a[avl->nA].d > 0.5*prm->slLen ){
			vl2f_swap( &avl->a[avl->nA] );
			avl->nA ++;
		}

	}

	VL2FA_DUMP( avl, "VL", 1, "straight" );



	free( as );

	pt2dA_destroy( apt );


	return( 1 );
}

int
	pt2dA_straight_seg_r( pt2dA_type *apt, int cyclic, float T, cseg_type ap[], int *nP )
{
	int	i;



	int state = 0;

	*nP = 0;

	cseg_type *ps = &ap[(*nP)++];
	ps->state = -1;

	for( i = 1 ; i < apt->nA-1 ; i++ ){

		pt2d_type *pt = &apt->a[i];


		float u = pt->r;

		if( ABS(u) > T ){
			if( ps->state >= 0 ){
				ps = &ap[(*nP)++];
				ps->state = -1;	
			}
			continue;
		}



		if( ps->state == -1 ){
			ps->i0 = ps->i1 = i;
			ps->u = u;
			ps->su = 0;
			ps->m0 = ps->m1 = u;
			ps->iu = i;
			ps->state = 0;
			continue;
		}


		ps->i1 = i;
		if( ABS(ps->u) < ABS(u) ){
			ps->u = u;
			ps->iu = i;
		}
		if( u < ps->m0 )	ps->m0 = u;
		else if( u > ps->m1 )	ps->m1 = u;
		ps->su += u;

	}

	if( ps->state < 0 )
		(*nP) --;


	if( cyclic == 1 ){
		if( ap[(*nP)-1].i1 == apt->nA -1 &&  ap[0].i0 == 0 ){
			ap[0].i0 = ap[(*nP)-1].i0;
			if( ABS(ap[(*nP)-1].u) > ABS(ap[0].u) ){
				ap[0].u = ap[(*nP)-1].u;
				ap[0].iu = ap[(*nP)-1].iu;
			}

			if( ap[(*nP)-1].m0 < ap[0].m0 )
				ap[0].m0 = ap[(*nP)-1].m0;

			if( ap[(*nP)-1].m1 > ap[0].m1 )
				ap[0].m1 = ap[(*nP)-1].m1;

			*nP -= 1;
		}
	}


	return( 1 );

}


#ifdef _AA_
void
cseg_print( cseg_type *as, int nA )
{
	int	i;

	for( i = 0 ; i < nA ; i++ ){
		cseg_type *s = &as[i];
		fprintf( stdout, "%3d %3d\t\t%4f(%d)\n", s->i0, s->i1, s->u, s->iu, s->su );
	}
	
}
#endif



static int	pt2fA_average( pt2dA_type *apt, int i0, int i1, vec2f_type *bv, float aT, vl2f_type *vl );

static int
pt2fA_straight_app(  pt2dA_type *apt, int i0, int i1, int nT, vl2f_type *bvl )
{

	int	n;
	vec2f_type	bv;

	eigen2d_type	e;
	pt2dA_eigen2d( apt, i0, i1, &e );
	bvl->v = e.v1;
	bvl->p = e.p;
	bvl->d = 20;


	float aT = cos( ANGLE_D2R(7.5) );
	bv = bvl->v;
	n = pt2fA_average( apt, i0, i1, &bv, aT, bvl );
	if( n < nT )	return( -1 );
	VL2F_DUMP( bvl, "PVL", 2, "D" );




	aT = cos( ANGLE_D2R(5) );
	bv = bvl->v;
	n = pt2fA_average( apt, i0, i1, &bv, aT, bvl );
	if( n < nT )	return( -1 );
	VL2F_DUMP( bvl, "PVL", 3, "D" );


	//aT = cos( ANGLE_D2R(3) );
	//bv = bvl->v;
	//n = pt2fA_average( apt, i0, i1, &bv, aT, bvl );
	//if( n < nT )	return( -1 );
	//VL2F_DUMP( bvl, "PVL", 2, "D" );



	return( n );
}



static int	pt2fA_average_d( pt2dA_type *apt, int i0, int i1,float aT, vl2f_type *vl );


static int
pt2fA_average( pt2dA_type *apt, int i0, int i1, vec2f_type *bv, float aT, vl2f_type *vl )
{

	int	i;

	int	n;

	vl->v.x = vl->v.y = 0;
	vl->p.x = vl->p.y = 0;
	
	for( i = i0, n = 0 ; i < i1 ; i++ ){
		pt2d_type *pt = &apt->a[i];
		float t = VEC2D_INNER( pt->n, *bv );

		if( ABS(t) < aT )	continue;

		if( t > 0 ){
			vl->v.x += pt->n.x;
			vl->v.y += pt->n.y;
		}
		else { 
			vl->v.x -= pt->n.x;
			vl->v.y -= pt->n.y;
		}


		vl->p.x += pt->p.x;
		vl->p.y += pt->p.y;

		n++;
	}

	if( n < 1 )
		return( -1 );

	vl->v.x /= n;
	vl->v.y /= n;

	vl->p.x /= n;
	vl->p.y /= n;

	n = pt2fA_average_d( apt, i0, i1, aT, vl );

	return( n );
}



static int
pt2fA_average_d( pt2dA_type *apt, int i0, int i1,float aT, vl2f_type *vl )
{

	int	i;

	int	n;

	float	t0,	t1;

	for( i = i0, n = 0 ; i < i1 ; i++ ){
		pt2d_type *pt = &apt->a[i];
		float t = VEC2D_INNER( pt->n, vl->v );

		if( ABS(t) < aT )	continue;


		vec2f_type	dp;
		dp.x = pt->p.x - vl->p.x;
		dp.y = pt->p.y - vl->p.y;
		t = VEC2D_INNER( vl->v, dp );

		if( n > 0 ){
			if( t < t0 )	t0 = t;
			else	if( t > t1 )	t1 = t;
		}
		else	t0 = t1 = t;

		n++;
	}

	if( n == 0 )
		return( -1 );

	float t = 0.5*(t1+t0);
	vl->p.x += t*vl->v.x;
	vl->p.y += t*vl->v.y;

	vl->d = 0.5*(t1 - t0);

	return( n );
}