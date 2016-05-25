



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



int	pt2dA_seg_r( pt2dA_type *apt, int cyclic, cseg_type ap[], int *nP );

void	cseg_print( cseg_type *as, int nA );
#endif



int
	pln_curvature( pln_type *pl, plnCurvaturePrm_type *prm, float *curvature )
{
	int i;


	int N = 5;

	int cyclic = ( pl->state == PLN_CLOSE )? 1 : 0;

	PLN_DUMP( pl, "AA", 1, "Curvature-N"  );
	pt2dA_type *apt = pln_sampleP( pl, 0, pl->len, 1.0, NULL );




	pt2dA_curvature( apt, cyclic, 5 );//, prm );



	//cseg_type as[1024];
	cseg_type *as = ( cseg_type *)malloc( (apt->nA)*sizeof(cseg_type) );
	int nS;
	pt2dA_seg_r(apt, cyclic, as, &nS );

#ifdef _DUMP
	cseg_print( as, nS );
#endif


	*curvature = 0;
	for( i = 0 ; i < nS ; i++ ){
		cseg_type *s = &as[i];
		if( ABS(s->u) > ABS(*curvature) )
			*curvature = s->u;
	}



	free( as );

	pt2dA_destroy( apt );


	return( 1 );
}




int
pt2dA_curvature( pt2dA_type *apt, int cyclic, int N )  // N = 5;
{
	int i;


//	int N = 5;


	for( i = 0 ; i < apt->nA ; i++ ){


		pt2d_type *pt = &apt->a[i];

		int i0 = i - N;
		if( i0 < 0 )	i0 =  (cyclic == 1 )? apt->nA + i0 : 0;

		int i1 = i + N;
		if( i1 >= apt->nA )	i1 =  (cyclic == 1 )?  i1 - apt->nA :apt->nA-1;


		pt2d_type *pt0 = &apt->a[i0];
		pt2d_type *pt1 = &apt->a[i1];

		vec2f_type v,	u;
		v.x = pt1->p.x - pt0->p.x;
		v.y = pt1->p.y - pt0->p.y;


		float t = hypot( v.x, v.y );
		v.x /= t;
		v.y /= t;

		VEC2D_LEFT( v, u );

		vec2f_type dp;
		dp.x = pt->p.x - pt0->p.x;
		dp.y = pt->p.y - pt0->p.y;

		pt->r = 2*VEC2D_INNER( u, dp ) / t;
	}


	apt->type = PT2D_4V;
	PT2DA_DUMP( apt, "AA", 1, "Curvature-N" );


	return( 1 );
}







int
pt2dA_seg_r( pt2dA_type *apt, int cyclic, cseg_type ap[], int *nP )
{
	int	i;



	int state = 0;

	*nP = 0;

	cseg_type *ps = &ap[(*nP)++];
	ps->state = -1;

	for( i = 1 ; i < apt->nA-1 ; i++ ){

		pt2d_type *pt = &apt->a[i];


		float u = pt->r;


		if( ps->state == -1 ){
			ps->i0 = ps->i1 = i;
			ps->u = u;
			ps->iu = i;
			ps->state = ( u < 0 );
			continue;
		}

		state = (u <= 0 );

		if( ps->state == state ){
			ps->i1 = i;
			if( ABS(ps->u) < ABS(u) ){
				ps->u = u;
				ps->iu = i;
			}

			continue;
		}


		ps = &ap[(*nP)++];
		ps->i0 = ps->i1 = i;
		ps->u = u;
		ps->iu = i;
		ps->state = ( u < 0 );

	}

	if( cyclic == 1 ){
		if( ap[(*nP)-1].state = ap[0].state ){
			ap[0].i0 = ap[(*nP)-1].i0;
			if( ABS(ap[(*nP)-1].u) > ABS(ap[0].u) ){
				ap[0].u = ap[(*nP)-1].u;
				ap[0].iu = ap[(*nP)-1].u;
			}

			*nP -= 1;
		}
	}


	return( 1 );

}


#ifdef _AA_
int	pt2dA_straight_seg_r( pt2dA_type *apt, int cyclic, float T, cseg_type ap[], int *nP );


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
	pt2dA_type *apt = pln_sampleP( pl, 0, pl->len, 1.0, NULL );




	pt2dA_curvature( apt, cyclic, N );//, prm );


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

		if( ABS(s->su ) > prm->slT )
			continue;

		if( avl->nA >= avl->NA )
			vl2fA_realloc( avl, avl->nA + 16 );

		if( pt2d_approximate_line_vl( apt, s->i0, s->i1, &avl->a[avl->nA] ) > 0 ){
			vl2f_swap( &avl->a[avl->nA] );
			avl->nA ++;
		}
	}

	VL2FA_DUMP( avl, "VL", 1, NULL );



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

#endif

void
cseg_print( cseg_type *as, int nA )
{
	int	i;

	for( i = 0 ; i < nA ; i++ ){
		cseg_type *s = &as[i];
		fprintf( stdout, "%3d %3d\t\t%4f(%d)\n", s->i0, s->i1, s->u, s->iu, s->su );
	}
	
}