#include	<stdio.h>
#include	<stdlib.h>

#include	"Mat2fType.h"


mat2f_type *
mat2f_alloc( int ni, int nj )
{
mat2f_type	*m;

//int n;
//	n = sizeof( mat2f_type ) + sizeof( float )* ni * nj;

	m = ( mat2f_type *)malloc( sizeof( mat2f_type ) + sizeof( float )* ni * nj );

	m->ni = ni;
	m->nj = nj;

	m->f = (float *)( m + 1 );

	return( m );
}



void
mat2f_free( mat2f_type *mat )
{
//	free( mat->f );

	free( mat );
}

/*
mat2f_type *
mat2f_alloc( int ni, int nj )
{
mat2f_type	*m;

	m = ( mat2f_type *)malloc( sizeof( mat2f_type ) );

	m->ni = ni;
	m->nj = nj;

	m->f = (float *)malloc( sizeof( float )*m->ni*m->nj );

	return( m );
}



void
mat2f_free( mat2f_type *mat )
{
	free( mat->f );

	free( mat );
}
*/



float
mat2f_inner( mat2f_type *m1, mat2f_type *m2 )
{
int	i,	j;
float	*f1,	*f2;
float	a;

	a = 0;

	for( i = 0, f1 = m1->f, f2 = m2->f ; i < m1->ni ; i++ )
		for( j = 0 ; j < m1->nj ; j++, f1++, f2++ )
			a += (*f1) * (*f2);

	return( a );
}


mat2f_type *
mat2f_add( mat2f_type *m1, mat2f_type *m2 )
{
mat2f_type	*m;
int	i;
float	*f,	*f1,	*f2;


	m = mat2f_alloc( m1->ni, m2->nj );

	f1 = m1->f;
	f2 = m2->f;
	f = m->f;
	for( i = 0 ; i < m->ni*m->nj ; i++, f++, f1++, f2++ ){
		*f = (*f1) + (*f2);
	}

	return( m );
}

void
mat2f_accumulate( mat2f_type *m, mat2f_type *m1, float t )
{

int	i;
float	*f,	*f1;


	f1 = m1->f;
	f = m->f;
	for( i = 0 ; i < m->ni*m->nj ; i++, f++, f1++ ){
		*f += t * (*f1);
	}
}



void
mat2f_const( mat2f_type *m, float a )
{
int	i;
float	*f;


	for( i = 0, f = m->f ; i < m->ni*m->nj ; i++, f++ )
		*f = a;
}

void
mat2f_add_const( mat2f_type *m, float a )
{
int	i;
float	*f;


	for( i = 0, f = m->f ; i < m->ni*m->nj ; i++, f++ )
		*f += a;
}


void
mat2f_mult_const( mat2f_type *m, float a )
{
int	i;
float	*f;


	for( i = 0, f = m->f ; i < m->ni*m->nj ; i++, f++ )
		*f *= a;
}
