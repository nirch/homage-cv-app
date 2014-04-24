/*********************
 ***   Matrix2Tool.c   ***
 ********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/umath.h"
#include	"Matrix2Type.h"


#define		MSIZE	2



void
matrix2_unit( matrix2_type *m )
{
	m->a00 = 1; m->a01 = 0;
	m->a10 = 0; m->a11 = 1;
}

void
matrix2_zero( matrix2_type *m )
{
	m->a00 = 0; m->a01 = 0;
	m->a10 = 0; m->a11 = 0;
}

void
matrix2f_zero( matrix2f_type *m )
{
	m->a00 = 0; m->a01 = 0;
	m->a10 = 0; m->a11 = 0;
}
void
matrix2_mult( matrix2_type *a, matrix2_type *b, matrix2_type *c )
{
int i, j, k;

    for( i = 0 ; i < MSIZE ; i++ )
		for( j =0  ; j < MSIZE ; j++ ){

			c->a[i][j]=0;

			for( k = 0 ; k < MSIZE ; k++)
				c->a[i][j] += a->a[i][k] * b->a[k][j];
		}
}


void
matrix2_mult_vector( matrix2_type *a, float *b, float *c )
{
int i, j;

	for( i = 0 ; i < MSIZE ; i++ ){

		c[i] = 0;

		for( j = 0 ; j < MSIZE ; j++)
			c[i] += a->a[i][j] * b[j];
	}
}


void
matrix2_multV( matrix2_type *m, vec2f_type *b, vec2f_type *c )
{
	c->x = m->a00*b->x + m->a01*b->y;
	c->y = m->a10*b->x + m->a11*b->y;
}

void
matrix2_multVD( matrix2_type *m, vec2d_type *b, vec2d_type *c )
{
	c->x = m->a00*b->x + m->a01*b->y;
	c->y = m->a10*b->x + m->a11*b->y;
}


void
matrix2_mult_vectorN( matrix2_type *m, float x, float y, float *mx, float *my )
{
	*mx = m->a00 * x + m->a01 * y ;

	*my = m->a10 * x + m->a11 * y ;
}








//void
//matrix2_inverse( matrix2_type *m, matrix2_type *im )
//{	
//	math_matrixD_inverse3( m->a, im->a );
//}


int
matrix2_solve( matrix2_type *m, vec2d_type *D, vec2d_type *X )
{
	if( math_linearD_equation2( m->a, (double*)D, 0.02, (double*)X ) != 1 )
		return( -1 );

	return( 1 );
}

int
matrix2S_solve( matrix2_type *m, vec2d_type *D, vec2d_type *X )
{
	if( math_linearD_equation2_symtric( m->a, (double*)D, 0.002, (double*)X ) != 1 )
		return( -1 );

	return( 1 );
}

int
matrix2fS_solve( matrix2f_type *m, vec2f_type *D, vec2f_type *X )
{
	if( math_linear_equation2_symtric( m->a, (float*)D, 0.02, (float*)X ) != 1 )
		return( -1 );

	return( 1 );
}


int
matrix2S_eigen( matrix2_type *m, float *e1, vec2f_type *v1, float *e2 )
{
float	desc,	t;

	if( m->a01 == 0 ){
		if( ABS(m->a00) > ABS(m->a11) ){
			*e1 = m->a00;
			*e2 = m->a11;
			v1->x = 1.0;
			v1->y = 0;
			return( 1 );
		}
		*e1 = m->a11;
		*e2 = m->a00;
		v1->x = 0;
		v1->y = 1;
		return( 1 );
	}

	desc = hypot( m->a00-m->a11, 2*m->a10 );
	*e1 = ((m->a00+m->a11) + desc ) * 0.5;
	*e2 = ((m->a00+m->a11) - desc ) * 0.5;


	v1->x = -m->a01;
	v1->y = m->a00 - *e1;

	t = hypot( v1->x, v1->y );
	v1->x /= t;
	v1->y /= t;

	return( 1 );
}


int
matrix2_eigen( matrix2_type *m, double *e1, vec2d_type *v1, double *e2, vec2d_type *v2 )
{
	double	B,	C;
	double	desc,	t;


	B = m->a00 + m->a11;
	C = m->a00 * m->a11 - m->a01 * m->a10;
	desc = B*B - 4 *C;
	if( desc < 0 )	return( -1 );

	desc = sqrt( desc );
	*e1 = (B + desc ) * 0.5;
	*e2 = (B - desc ) * 0.5;


	v1->x = -m->a01;
	v1->y = m->a00 - *e1;

	t = hypot( v1->x, v1->y );
	v1->x /= t;
	v1->y /= t;

	v2->x = -v1->y;
	v2->y = v1->x;

	return( 1 );
}


void
matrix2_max_diff( matrix2_type *m0, matrix2_type *m1, matrix2_type *md )
{
float	t;
int	i;

	for( i = 0 ; i < MSIZE*MSIZE ; i++ ){
		t = m0->A[i] - m1->A[i];
		if( t < 0 )	t = -t;
		if( t > md->A[i] )	md->A[i] = t;
	}
}


int
matrix2_write_to_file( matrix2_type *m, char *file )
{
FILE *fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	matrix2_write( m, fp );

	fclose( fp );

	return( 1 );
}

void
matrix2_write( matrix2_type *m, FILE *fp )
{
int	i,	j;

	for( i = 0 ; i < MSIZE ; i++ ){
		for( j = 0 ; j < MSIZE ; j++ ){
			fprintf( fp, "%f   ", (float)m->a[i][j] );
	}
		fprintf( fp, "\n" );
	}

	fprintf( fp, "\n" );
}


