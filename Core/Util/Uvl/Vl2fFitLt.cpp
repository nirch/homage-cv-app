
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif


#include	"Uigp/igp.h"
#include	"Uln/PlnType.h"
#include	"Uvl/Vl2fType.h"

#include "Umath/Matrix4Type.h"
#include "Umath/Matrix.h"


int	vl2fA_fit_lt( vl2fA_type *bavl,  vl2fA_type *avl, lt2_type *lt );

int	vl2fA_fit_affine_lt( vl2fA_type *bavl,  vl2fA_type *avl, lt2_type *lt );



static int	vl2fA_fit_lt_1( vl2f_type *bvl,  vl2f_type *vl,  float dt, matrix4_type *m, double D[4] );

//vl2fA_type *	vl2fA_fit_lt_field( vl2fA_type *bavl,  lt2_type *lt, vl2fA_type *avl );


//int	vl2fA_match( vl2fA_type *bavl, vl2fA_type *avl );
//int	vl2fA_match_lt( vl2fA_type *bavl, lt2_type *lt, vl2fA_type *avl );






int
vl2fA_fit_lt_smart( vl2fA_type *bavl,  vl2fA_type *avl, int g, lt2_type *lt )
{
	vl2fA_match( bavl, avl );

	vl2fA_fit_lt( bavl,  avl, lt );


	vl2fA_type *avl1 = vl2fA_fit_lt_field( bavl, lt, NULL );
	VL2FA_DUMP( avl1, "AA", 1, "field" );
	vl2fA_destroy( avl1 );



	vl2fA_match_lt( bavl, lt, 10.0, 32.0, avl );
	if( vl2fA_fit_lt( bavl,  avl, lt ) < 0 )
		return( -1 );


	avl1 = vl2fA_fit_lt_field( bavl, lt, NULL );
	VL2FA_DUMP( avl1, "AA", 1, "field-1" );
	vl2fA_destroy( avl1 );




	vl2fA_match_lt( bavl, lt, 10.0, 16.0, avl );
	if( vl2fA_fit_lt( bavl,  avl, lt ) < 0 )
		return( -1 );


	avl1 = vl2fA_fit_lt_field( bavl, lt, NULL );
	VL2FA_DUMP( avl1, "AA", 1, "field-2" );
	vl2fA_destroy( avl1 );



	int	i,	n;
	for( i = 0 ;  i < avl->nA ; i++ )
		avl->a[i].ng[1-g] = -1;



	for( i = 0, n = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *vl = &bavl->a[i];
		vl->ng[g] = vl->id;

		if( vl->id >= 0 ){
			avl->a[vl->id].ng[1-g] = i;
			n++;
		}
	}

	//fprintf( stdout, "%d  ", n );


	return( n );
}



int
vl2fA_fit_lt( vl2fA_type *bavl,  vl2fA_type *avl, lt2_type *lt )
{
matrix4_type	m;
double	D[4],	X[4];
	int i;


//	vl2fA_match( bavl, avl );


	matrix4_zero( &m );
	D[0] = D[1] = D[2] = D[3] = 0;

	for( i = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];

		if( bvl->id < 0 )	continue;

		vl2fA_fit_lt_1( bvl, &avl->a[bvl->id], 20, &m, D );
	}


	if( matrix4_solve( &m, D, X ) < 0 )
		return( -1 );

	lt->a0 = X[0];
	lt->b0 = X[1];
	lt->c0 = X[2];
	lt->a1 = -X[1];
	lt->b1 = X[0];
	lt->c1 = X[3];



	//vl2fA_type *avl1 = vl2fA_fit_lt_field( bavl, lt, NULL );

	//VL2FA_DUMP( avl1, "AA", 1, "field" );

	//vl2fA_destroy( avl1 );


	return( 1 );
}



static int
vl2fA_fit_lt_1( vl2f_type *bvl,  vl2f_type *vl,  float dt, matrix4_type *m, double D[4] )
{
float t;

	int n = bvl->d / dt;


	vec2f_type N;
	VEC2D_RIGHT( vl->v, N );
	float d = VEC2D_INNER( vl->p, N );

	for( t = -n*dt ; t < bvl->d ; t += dt ){
		vec2f_type	p;
		p.x = bvl->p.x + t*bvl->v.x;
		p.y = bvl->p.y + t*bvl->v.y;

		double	X[4];
		X[0] = p.x*N.x + p.y * N.y;
		X[1] = p.y*N.x - p.x * N.y;
		X[2] = N.x;
		X[3] = N.y;


		matrix4_linaer_equation_add( m, D, X, d );
	}

	return( 1 );

}




int
vl2fA_fit_affine_lt( vl2fA_type *bavl,  vl2fA_type *avl, int g, lt2_type *lt )
{
	vl2fA_match( bavl, avl );

	vl2fA_fit_affine_lt( bavl,  avl, lt );


	vl2fA_type *avl1 = vl2fA_fit_lt_field( bavl, lt, NULL );
	VL2FA_DUMP( avl1, "AA", 1, "field" );
	vl2fA_destroy( avl1 );



	vl2fA_match_lt( bavl, lt, 10, 5.0, avl );
	if( vl2fA_fit_affine_lt( bavl,  avl, lt ) < 0 )
		return( -1 );


	avl1 = vl2fA_fit_lt_field( bavl, lt, NULL );
	VL2FA_DUMP( avl1, "AA", 1, "field-1" );
	vl2fA_destroy( avl1 );




	int	i;
	for( i = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *vl = &bavl->a[i];
		vl->ng[g] = vl->id;
	}


	return( 1 );
}





static int	vl2fA_fit_affine_lt_1( vl2f_type *bvl,  vl2f_type *vl,  float dt, matrix_type *m, matrix_type *D );

int
vl2fA_fit_affine_lt( vl2fA_type *bavl,  vl2fA_type *avl, lt2_type *lt )
{
	matrix_type	*m,	*D;
	double	X[6];
	int i;


	//	vl2fA_match( bavl, avl );

	m = matrixd_alloc( 6, 6 );
	matrix_zero( m );

	D = matrixd_alloc( 6, 1 );
	matrix_zero( D );

	

	for( i = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];

		if( bvl->id < 0 )	continue;

		vl2fA_fit_affine_lt_1( bvl, &avl->a[bvl->id], 20, m, D );
	}


	if( matrix_solve( m, D->d, X ) < 0 )
		return( -1 );

	lt->a0 = X[0];
	lt->b0 = X[1];
	lt->c0 = X[2];

	lt->a1 = X[3];
	lt->b1 = X[4];
	lt->c1 = X[5];



	return( 1 );
}



static int
vl2fA_fit_affine_lt_1( vl2f_type *bvl, vl2f_type *vl, float dt, matrix_type *m, matrix_type *D )
{
	float t;

	int n = bvl->d / dt;


	vec2f_type N;
	VEC2D_RIGHT( vl->v, N );
	float d = VEC2D_INNER( vl->p, N );

	for( t = -n*dt ; t < bvl->d ; t += dt ){
		vec2f_type	p;
		p.x = bvl->p.x + t*bvl->v.x;
		p.y = bvl->p.y + t*bvl->v.y;

		double	X[6];
		X[0] = p.x*N.x;
		X[1] = p.y*N.x;
		X[2] = N.x;

		X[3] = p.x * N.y;
		X[4] = p.y * N.y;
		X[5] = N.y;


		matrix_linaer_equation_add( m, D, X, d );
	}

	return( 1 );
}




vl2fA_type *
vl2fA_fit_lt_field( vl2fA_type *bavl,  lt2_type *lt, vl2fA_type *avl )
{
	int i;
	
	avl = vl2fA_alloc( bavl->nA );

	for( i = 0 ; i < bavl->nA ; i++ ){
		vl2f_type *bvl = &bavl->a[i];

		if( bvl->id < 0 )	continue;

		vec2f_type	p;
		p.x = LT2_F1( *lt, bvl->p.x, bvl->p.y );
		p.y = LT2_F2( *lt, bvl->p.x, bvl->p.y );

		vl2f_set( &avl->a[avl->nA++], &bvl->p, &p );
	}

	//VL2FA_DUMP( avl, "AA", 1, "field" );

	return( avl );
}





