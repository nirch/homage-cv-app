
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#include	"Uigp/igp.h"
#include "Umath/Matrix.h"
#include "PcaType.h"

//
//typedef struct pca_type {
//	int	n;
//	int	i;
//
//	matrix_type	*m;
//
//	matrix_type	*a;
//
//
//	matrix_type *em;
//	matrix_type *ev;
//} pca_type;



pca_type *
pca_alloc( int n )
{
pca_type	*pca;

	pca = ( pca_type *)malloc( sizeof(pca_type ) );

	pca->n = n;
	pca->i = 0;

	pca->m = matrix_alloc( n, n, MT_DOUBLE );
	matrix_zero( pca->m );
	pca->a = matrix_alloc( 1, n, MT_DOUBLE );
	matrix_zero( pca->a );

	pca->em = matrix_alloc( n, n, MT_DOUBLE );
	pca->ev = matrix_alloc( 1, n, MT_DOUBLE );

	return( pca );
}

void
pca_destory( pca_type *pca )
{

	matrix_destroy( pca->m );
	matrix_destroy( pca->a );

	free( pca );
}

void
pca_add( pca_type *pca, double f[] )
{
int	i,	j;
double	*a,	*m;

	for( i = 0 ; i < pca->n ; i++ ){
		m = MATRIX_ELM_P( pca->m, i, i );
		for( j = i ; j < pca->n ; j++, m++ ){
			*m += f[i]*f[j];
		}

		a = MATRIX_ELM_P( pca->a, 0, i );
		*a += f[i];
	}

	pca->i++;
}


void
pca_final( pca_type *pca )
{
	int	i,	j;
	double	*m;
	for( i = 0 ; i < pca->n ; i++ )
		pca->a->d[i] /= pca->i;



	for( i = 0 ; i < pca->n ; i++ ){
		m = MATRIX_ELM_P( pca->m, i, i );

		for( j = i ; j < pca->n ; j++, m++ ){
			*m = *m / pca->i - pca->a->d[i] * pca->a->d[j];
		}
	}


	matrixD_symmetric( pca->m );

//	matrix_print( "co", pca->m, stdout );


	matrixSD_eigen( pca->m, pca->em, pca->ev->d );
}






