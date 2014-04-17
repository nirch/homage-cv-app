#include	<string.h>
#include	<stdio.h>
#include	<math.h>

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "clapackD.lib" )
#else
#pragma comment( lib, "clapack.lib" )
#endif




#include	"Uigp/igp.h"
#include	"umath.h"
#include	"Matrix.h"

#ifdef WIN32
#pragma warning(disable :4005)
#endif


static matrix_type *	matrix_mds_SI( matrix_type *S, int rank );

static void	matrix_mds_test( matrix_type *PG );


int
matrix_mds( matrix_type *D, int rank, matrix_type **P )
{
matrix_type *C,	*T;
matrix_type	*M,	*U,	*V,	*S;
matrix_type *SI,	*PG;


	C = matrix_alloc( D->m, D->m, MT_DOUBLE );
	matrix_centering( C );

	//matrix_print( "D", D, stdout );
	//matrix_print( "C", C, stdout );

	M = matrix_alloc( D->m, D->m, MT_DOUBLE );
	T = matrix_alloc( D->m, D->m, MT_DOUBLE );
	matrix_multiply( C, D, T );
	matrix_multiply( T, C, M );
	matrix_destroy( T );


	matrix_multiply_scalar( M, -0.5, M );
//	matrix_print( "M", M, stdout );


	if( matrix_svd( M, &S, &U, &V  ) < 0 )
		return( -1 );

//	matrix_print( "S", S, stdout );
//	matrix_svd_test( M, S, U, V );

	SI = matrix_mds_SI( S, rank );



	T = matrix_alloc( SI->n, SI->n, MT_DOUBLE );
	matrix_diagonal( T, SI->d, SI->n );

	PG = matrix_alloc( U->m, T->n, MT_DOUBLE );
	matrix_multiply( U, T, PG );
	matrix_destroy( T );


	*P = matrix_crop( PG, PG->m, rank, NULL );
	matrix_destroy( PG );


	matrix_destroy( S );
	matrix_destroy( U );
	matrix_destroy( V );
	matrix_destroy( SI );



	return( 1 );

}


static matrix_type *
matrix_mds_SI( matrix_type *S, int rank )
{
	matrix_type *SI;
	double	*sp,	*tp;
	int	i;

	SI = matrix_alloc( S->m, S->n, MT_DOUBLE );

	sp = S->d;
	tp = SI->d;

	for( i = 0 ; i < S->n ; i++, sp++, tp++ ){
		if( *sp < 0 || i > rank )	
			*tp = 0;
		else	*tp = sqrt( *sp );
	}

	return( SI );
}



matrix_type *
matrix_mds_P2D( matrix_type *P )
{
	matrix_type *D;
	int	i,	j;

	D = matrix_alloc( P->m, P->m, MT_DOUBLE );
	for( i = 0 ; i < P->m ; i++ )
		for( j = 0 ; j < P->m ; j++ )
			MATRIX_ELM( D, i, j ) = vectorD_distance2( MATRIX_ROW( P, i),  MATRIX_ROW( P, j), P->n );

	return( D );
}


static void
matrix_mds_test( matrix_type *PG )
{
matrix_type *T,	*PT;


	T = matrix_alloc( PG->n, PG->m, MT_DOUBLE );
	PT = matrix_alloc( PG->n, PG->m, MT_DOUBLE );

	matrix_transpose( PG, PT );
//	matrix_print( "pT", PT, stdout );
	matrix_multiply( PG, PT, T );

	matrix_print( "PG2", T, stdout );

	matrix_destroy( T );
}



void
matrix_mds_test2( matrix_type *P )
{
matrix_type *C,	*T,	*DD;

	DD = matrix_mds_P2D( P );
	matrix_print( "D", DD, stdout );
	matrix_destroy( DD );

	C = matrix_alloc( P->m, P->m, MT_DOUBLE );
	matrix_centering( C );


	T = matrix_alloc( P->m, P->n, MT_DOUBLE );
	matrix_multiply( C, P, T );

	DD = matrix_mds_P2D( T );
	matrix_print( "DD", DD, stdout );

	matrix_destroy( T );
	matrix_destroy( C );
	matrix_destroy( DD );
}