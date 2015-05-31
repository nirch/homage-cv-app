
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include	"Uigp/igp.h"
#include	"Umath/LT2Type.h"

#include	"Uvl/Vl2fType.h"




//int	vl2fA_match( vl2fA_type *bavl, vl2fA_type *avl );
//int	vl2fA_match_lt( vl2fA_type *bavl, lt2_type *lt, vl2fA_type *avl );




int	vl2fA_match_1( vl2f_type *bvl, vl2fA_type *avl );


static int vl2fA_match_lt_1( vl2f_type *bvl, lt2_type *lt, float aT, float dT, vl2fA_type *avl );






int
vl2fA_match( vl2fA_type *bavl, vl2fA_type *avl )
{
int	i,	n;

	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];

		bvl->id = vl2fA_match_1( bvl, avl );

		if( bvl->id >= 0 )	n++;
	}

	return( n );
}


int
	vl2fA_match_1( vl2f_type *bvl, vl2fA_type *avl )
{
	int	i,	iMin;
	float tMin;
	

	float aT = cos( ANGLE_D2R(5) );
	float dT = 50;

	vec2f_type	N;

	VEC2D_RIGHT( bvl->v, N );

	iMin = -1;
	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];

		float a = VEC2D_INNER( bvl->v, vl->v );

		if( ABS(a) < aT )	continue;

		vec2f_type	dp;
		dp.x = vl->p.x - bvl->p.x;
		dp.y = vl->p.y - bvl->p.y;

		float t0 = VEC2D_INNER( bvl->v, dp );
		float t1 = -VEC2D_INNER( vl->v, dp );

		if( (t0 < -bvl->d + 10 || t0 > bvl->d - 10) && (t1 < -vl->d + 10 || t1 > vl->d - 10) )
			continue;

		vec2f_type	v;
		v.x = vl->p.x + t1*vl->v.x - bvl->p.x;
		v.y = vl->p.y + t1*vl->v.y - bvl->p.x;
		
		float t = VEC2D_INNER( N, dp );
		

		if( ABS(t) > dT )	continue;

		if( iMin < 0 || ABS(t) < ABS(tMin) ){
			iMin = i;
			tMin = t;
		}
	}


	return( iMin );
}





int
vl2fA_match_lt( vl2fA_type *bavl, lt2_type *lt, float aT, float dT, vl2fA_type *avl )
{
	int	i,	n;

	
	aT = cos( ANGLE_D2R(10) );


	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];

		bvl->id = vl2fA_match_lt_1( bvl, lt, aT, dT, avl );

		if( bvl->id >= 0 )	n++;
	}

	return( n );
}



static int
vl2fA_match_lt_1( vl2f_type *bvl, lt2_type *lt, float aT, float dT, vl2fA_type *avl )
{
	int	i,	iMin;
	float tMin;


	//float aT = cos( ANGLE_D2R(10) );
	//float dT = 50;

	vec2f_type	p,	v,	bp;
	LT2_F( *lt, bvl->p, p );


	bvl->ev.x = p.x - bvl->p.x;
	bvl->ev.y = p.y - bvl->p.y;

	bp.x = p.x - bvl->p.x;
	bp.y = p.y - bvl->p.y;
	float bt = hypot( bp.x, bp.y );
	if( bt > 0.25 ){
		bp.x /= bt;
		bp.y /= bt;
	}
	else	bt = 0;


	v.x = lt->a0 * bvl->v.x + lt->b0 * bvl->v.x;
	v.y = lt->a1 * bvl->v.y + lt->b1 * bvl->v.y;

	VEC2D_NORMALIZE( v );

	vec2f_type	N;
	VEC2D_RIGHT( v, N );

	iMin = -1;
	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];


		float a = VEC2D_INNER( v, vl->v );

		if( ABS(a) < aT )	continue;


		vec2f_type	dp;
		dp.x = vl->p.x - p.x;
		dp.y = vl->p.y - p.y;

		float t0 = VEC2D_INNER( v, dp );
		float t1 = -VEC2D_INNER( vl->v, dp );

		if( (t0 < -bvl->d + 10 || t0 > bvl->d - 10) && (t1 < -vl->d + 10 || t1 > vl->d - 10) )
			continue;

		vec2f_type	dv;
		dv.x = vl->p.x + t1*vl->v.x - bvl->p.x;
		dv.y = vl->p.y + t1*vl->v.y - bvl->p.y;


		if( bt > 0 ){
			float t = VEC2D_INNER( bp, dv );
			if( t < 0 )	continue;
		}



		float t = VEC2D_INNER( N, dp );
		if( ABS(t) > dT )	continue;

		if( iMin < 0 || ABS(t) < ABS(tMin) ){
			iMin = i;
			tMin = t;
		}
	}


	return( iMin );
}




int
vl2fA_match_no( vl2fA_type *avl, int ng )
{
	int	i,	n;

	for( i = 0, n = 0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];

		if( vl->ng[ng] > 0 )
			n++;
	}


	return( n );
}


int
vl2fA_match_set( vl2fA_type *bavl, vl2fA_type *avl, int ng )
{
	int	i,	n;

	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *vl = &bavl->a[i];

		if( vl->ng[ng] >= 0 ){
			vl->ng[ng] = avl->a[vl->ng[ng] ].ng[ng];

			if( vl->ng[ng] >= 0 )
				n++;
		}
	}


	return( n );
}

int
	vl2fA_match_connect( vl2fA_type *bavl, vl2fA_type *avl, int ng )
{
	int	i,	n;

	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *vl = &bavl->a[i];

		if( vl->ng[ng] < 0 )	continue;

		int k = vl->ng[ng];

		vl->ng[ng] = avl->a[k ].ng[ng];

		if( vl->ng[ng] < 0 )	continue;

		vl->ev.x += avl->a[k ].ev.x;
		vl->ev.y += avl->a[k ].ev.y;

		
		n++;
	}

	return( n );
}

int
	vl2fA_match_set_inv( vl2fA_type *bavl, vl2fA_type *avl, int ng )
{
	int	i,	n;

	int ng1 = 1 - ng;

	for( i = 0 ;i < avl->nA ; i++ )
		avl->a[i].ng[ng1] = -1;


	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *vl = &bavl->a[i];

		if( vl->ng[ng] >= 0 ){
			avl->a[vl->ng[ng] ].ng[ng1] = i ;

			n++;
		}
	}


	return( n );
}





int
vl2fA_set_ev_lt( vl2fA_type *avl, lt2_type *lt )
{
	int	i,	n;

	for( i = 0, n = 0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];

		vec2f_type	p;
		LT2_F( *lt, vl->p, p );

		vl->ev.x = vl->p.x - p.x;
		vl->ev.y = vl->p.y - p.y;

		VEC2D_NORM2( vl->ev );
	}

	return( n );
}


vl2fA_type *
	vl2fA_match_field( vl2fA_type *bavl, vl2fA_type *avl1, int ng, vl2fA_type *avl )
{
	int	i,	n;

	avl = vl2fA_realloc( avl, bavl->nA );
	avl->nA = 0;

	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];
		if( bvl->ng[ng] < 0 )	continue;


		vl2f_type *vl = &avl1->a[ bvl->ng[ng] ];

		vl2f_set( &avl->a[avl->nA++], &bvl->p, &vl->p );
	}


	return( avl );
}


vl2fA_type *
	vl2fA_match_field_ev( vl2fA_type *bavl, int ng, vl2fA_type *avl )
{
	int	i,	n;

	avl = vl2fA_realloc( avl, bavl->nA );
	avl->nA = 0;

	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];
		if( bvl->ng[ng] < 0 )	continue;


		vec2f_type	p;
		p.x = bvl->p.x + bvl->ev.x;
		p.y = bvl->p.y + bvl->ev.y;

		vl2f_set( &avl->a[avl->nA++], &bvl->p, &p );
	}


	return( avl );
}