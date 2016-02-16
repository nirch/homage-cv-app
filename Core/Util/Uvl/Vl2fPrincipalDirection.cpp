
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"
#include "Uvl/FloatType.h"


#include "Vl2fType.h"	





static int	vl2fA_principal_direction_1( vl2fA_type *avl, float aT, vl2f_type *bvl, vl2f_type *rvl, float *cover );
static int	vl2fA_principal_direction_1_v( vl2fA_type *avl, float aT, vl2f_type *bvl, vec2f_type *v );


int
vl2fA_principal_direction( vl2fA_type *avl, vl2f_type *rvl )
{
int	i;

	float	aT = cos( ANGLE_D2R(12.0) );
	//float	aT1 = cos( ANGLE_D2R(2.50) );

	int iMax = -1;
	float cMax;
	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_type	tvl;
		float cover;

		vl2fA_principal_direction_1( avl, aT, &avl->a[i], &tvl, &cover );

		if( iMax < 0 || cMax < cover ){ //rvl->d < tvl.d ){
			iMax = i;
			*rvl = tvl;
			cMax = cover;
		}
	}


	vec2f_type	v;
	vl2fA_principal_direction_1_v( avl, aT, &avl->a[iMax], &v );

	rvl->v = v;

	vl2f_type	tvl;
	float cover;
	vl2fA_principal_direction_1( avl, aT, rvl, &tvl, &cover );
	*rvl = tvl;
	

	return( iMax );
}



static int
vl2fA_principal_direction_1( vl2fA_type *avl, float aT, vl2f_type *bvl, vl2f_type *rvl, float *cover )
{
	int	i,	n;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );


	*cover = 0;

	float	t0,	t1,	u0,	u1;
	for( i = 0, n =  0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( bvl->v, vl->v );
		if( ABS(a) < aT )	continue;

		n++;

		vec2f_type	p;
		p.x = vl->p.x - bvl->p.x;
		p.y = vl->p.y - bvl->p.y;

		float t = VEC2D_INNER( bvl->v, p );
		float u = VEC2D_INNER( bu, p );

		if( n == 1 ){
			t1 = t + vl->d;
			t0 = t - vl->d;

			u0 = u1 = u;

			*cover += vl->d;

			continue;
		}

		if( t-vl->d < t0 )	t0 = t-vl->d;
		if( t+vl->d > t1 )	t1 = t+vl->d;
		if( u < u0 )	u0 = u;
		if( u > u1 )	u1 = u;


		*cover += vl->d;


	}

	float t = (t1 + t0)/2;
	float u = (u1 + u0)/2;




	*rvl = *bvl;
	rvl->p.x += t*rvl->v.x + u*bu.x;
	rvl->p.y += t*rvl->v.y + u*bu.y;

	rvl->d = (t1 - t0 )/2;
	rvl->e = ( u1 - u0 )/2;

	return( 1 );
}
			



static int
vl2fA_principal_direction_1_v( vl2fA_type *avl, float aT, vl2f_type *bvl, vec2f_type *v )
{
	int	i,	n;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );



	v->x = v->y = 0;

	for( i = 0, n =  0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( bvl->v, vl->v );
		if( ABS(a) < aT )	continue;

		n++;

		if( a < 0 ){
			v->x -= vl->d*vl->v.x;
			v->y -= vl->d*vl->v.y;
		}
		else {
			v->x += vl->d*vl->v.x;
			v->y += vl->d*vl->v.y;
		}

	}


	VEC2D_NORMALIZE( *v );


	return( 1 );
}



int
vl2fA_principal_direction_side( vl2fA_type *avl, float aT, float uT, vl2f_type *bvl, float al[2], float ar[2] )
{
	int	i,	n;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );

	al[0] = al[1] = 0;
	ar[0] = ar[1] = 0;

	int	nr,	nl;
	nr = nl = 0;
	for( i = 0, n =  0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( bvl->v, vl->v );
		if( ABS(a) < aT )	continue;

		n++;

		vec2f_type	p;
		p.x = vl->p.x - bvl->p.x;
		p.y = vl->p.y - bvl->p.y;

		float t = VEC2D_INNER( bvl->v, p );
		float u = VEC2D_INNER( bu, p );


		if( u < 0 ){

			float du = u + bvl->e;
			if( ABS( du ) > uT )	continue;

			nl++;
			if( nl == 1 ){
				al[0] =  t - vl->d;
				al[1] =  t + vl->d;
				continue;
			}

			float t0 = t - vl->d;
			if( al[0] > t0 )	al[0] = t0;

			t0 = t + vl->d;
			if( al[1] < t0 )	al[1] =  t0;

			continue;
		}


		float du = u - bvl->e;
		if( ABS( du ) > uT )	continue;

		nr++;

		if( nr == 1 ){
			ar[0] =  t - vl->d;
			ar[1] =  t + vl->d;
			continue;
		}

		float t0 = t - vl->d;
		if( ar[0] > t0 )	ar[0] = t0;

		t0 = t + vl->d;
		if( ar[1] < t0 )	ar[1] =  t0;

	}



	return( 1 );
}


// check if there are line vertical to the principal line 
int
vl2fA_principal_direction_vertical( vl2fA_type *avl, float aT, vl2f_type *bvl )
{
	int	i,	n;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );


	
	float t0 = -bvl->d;
	float t1 = bvl->d;

	
	for( i = 0, n =  0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( vl->v, bu );
		if( ABS(a) < aT )	continue;


		vec2f_type	p;
		p.x = vl->p.x - bvl->p.x;
		p.y = vl->p.y - bvl->p.y;

		float t = VEC2D_INNER( bvl->v, p );
		float u = VEC2D_INNER( bu, p );

		if( u < -vl->d || u > vl->d )
			continue;

		n++;


		if( t < t0 )	t0 = t;
		if( t > t1 )	t1 = t;
	}

	if( n > 0 ){
		bvl->d = 0.5*(t1-t0);

		float t = 0.5*(t1+t0);
		bvl->p.x += t*bvl->v.x;
		bvl->p.y += t*bvl->v.y;
	}

	return( n );
}




#ifdef _AA_
int
vl2fA_principal_direction_vertical( vl2fA_type *avl, float aT, vl2f_type *bvl )
{
	int	i,	k;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );


	floatA_type *a = floatA_alloc( avl->nA );


	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];

		float a = VEC2D_INNER( bvl->v, vl->v );
		if( ABS(a) < aT )	continue;

		vec2f_type	p;
		p.x = vl->p.x - bvl->p.x;
		p.y = vl->p.y - bvl->p.y;

		float t = VEC2D_INNER( bu, p );

		for( k = a->nA ; k > 0 ; k-- ){
			if( t > a->a[k-1] )	break;
			a->a[k]= a->a[k-1];
		}

		a->a[k] = t;

		a->nA++;
	}

		
	return( 1 );
}



typedef struct side_type {

	float u;
	
	int n;

	float t0,	t1;
	float d;

} side_type;


int
vl2fA_principal_direction_side( vl2fA_type *avl, vl2f_type *bvl, float aT, float uT, float u0, side_type *s )
{
	int	i,	n;

	vec2f_type bu;
	VEC2D_RIGHT( bvl->v, bu );

	s->u = 0;
	s->d = 0;
	s->n = 0;

	float	su = 0;

	int	nr,	nl;
	nr = nl = 0;
	for( i = 0, n =  0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( bvl->v, vl->v );
		if( ABS(a) < aT )	continue;

		n++;

		vec2f_type	p;
		p.x = vl->p.x - bvl->p.x;
		p.y = vl->p.y - bvl->p.y;

		float t = VEC2D_INNER( bvl->v, p );
		float u = VEC2D_INNER( bu, p );



		float du = u - u0;
		if( ABS( du ) > uT )	continue;


		s->n++;

		s->u += u;
		s->d += vl->d;

		if( nr == 1 ){
			s->t0 =  t - vl->d;
			s->t1 =  t + vl->d;
			continue;
		}

		float t0 = t - vl->d;
		if( s->t0 > t0 )	s->t0 = t0;

		t0 = t + vl->d;
		if( s->t1 < t0 )	s->t1 =  t0;

	}

	s->u /= s->n;
 


	return( 1 );
}
#endif