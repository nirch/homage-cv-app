/*********************
 ***   Matrix4.c   ***
 ********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Uvec/Vec3d.h"
#include	"umath.h"

#include	"Matrix4Type.h"


#define		MSIZE	4


matrix4_type *
matrix4_alloc()
{
matrix4_type	*m;

	m = (matrix4_type *)malloc( sizeof( matrix4_type) );
	
	return( m );
}


void
matrix4_destroy( matrix4_type *m )
{
	free( m );
}


void  
matrix4_zero( matrix4_type *m )
{
    m->a00 = 0;
	m->a01 = 0;    
	m->a02 = 0;   
	m->a03 = 0;

    m->a10 = 0;
	m->a11 = 0;
	m->a12 = 0;
	m->a13 = 0;

    m->a20 = 0;
	m->a21 = 0;
	m->a22 = 0;
	m->a23 = 0;

    m->a30 = 0;
	m->a31 = 0;
	m->a32 = 0;
	m->a33 = 0;
}


void  
matrix4_unity( matrix4_type *m )
{
    m->a00 = 1;
	m->a01 = 0;    
	m->a02 = 0;   
	m->a03 = 0;

    m->a10 = 0;
	m->a11 = 1;
	m->a12 = 0;
	m->a13 = 0;

    m->a20 = 0;
	m->a21 = 0;
	m->a22 = 1;
	m->a23 = 0;

    m->a30 = 0;
	m->a31 = 0;
	m->a32 = 0;
	m->a33 = 1;
}

void
matrix4_set( matrix4_type *m, float *a )
{
int i, j;
float	*sp;
	
	sp = a;
    for( i = 0 ; i < 4 ; i++ )
		for( j =0  ; j < 4 ; j++ )
			m->a[i][j] = *sp++;
}

void
matrix4_setD( matrix4_type *m, double *a )
{
	int i, j;
	double	*sp;

	sp = a;
	for( i = 0 ; i < 4 ; i++ )
		for( j =0  ; j < 4 ; j++ )
			m->a[i][j] = *sp++;
}

void
matrix4_multiply_scalar( matrix4_type *m, float a, matrix4_type *tm )
{
	int i, j;


	for( i = 0 ; i < 4 ; i++ )
		for( j =0  ; j < 4 ; j++ )
			tm->a[i][j] = a * m->a[i][j];
}

void
matrix4H_multiply_scalar( matrix4_type *m, float a, matrix4_type *tm )
{
	int i, j;


	for( i = 0 ; i < 3 ; i++ )
		for( j =0  ; j < 4 ; j++ )
			tm->a[i][j] = a * m->a[i][j];
}


void
matrix4_mult( matrix4_type *a, matrix4_type *b, matrix4_type *c )
{
int i, j, k;

    for( i = 0 ; i < 4 ; i++ )
		for( j =0  ; j < 4 ; j++ ){

			c->a[i][j]=0;

			for( k = 0 ; k < 4 ; k++)
				c->a[i][j] += a->a[i][k] * b->a[k][j];
		}
}


void
matrix4_mult_vector( matrix4_type *a, float *b, float *c )
{
int i, j;

	for( i = 0 ; i < MSIZE ; i++ ){

		c[i] = 0;

		for( j = 0 ; j < MSIZE ; j++)
			c[i] += a->a[i][j] * b[j];
	}
}


void
matrix4_multVD( matrix4_type *a, double *b, double *c )
{
	int i, j;

	for( i = 0 ; i < MSIZE ; i++ ){

		c[i] = 0;

		for( j = 0 ; j < MSIZE ; j++)
			c[i] += a->a[i][j] * b[j];
	}
}
void
matrix4H_mult_vector( matrix4_type *m, vec3f_type *b, vec3f_type *c )
{

	c->x = m->a00 * b->x + m->a01 * b->y + m->a02 * b->z + m->a03;
	
	c->y = m->a10 * b->x + m->a11 * b->y + m->a12 * b->z + m->a13;

	c->z = m->a20 * b->x + m->a21 * b->y + m->a22 * b->z + m->a23;
}


void
matrix4H_mult_vectorD( matrix4_type *m, vec3d_type *b, vec3d_type *c )
{

	c->x = m->a00 * b->x + m->a01 * b->y + m->a02 * b->z + m->a03;

	c->y = m->a10 * b->x + m->a11 * b->y + m->a12 * b->z + m->a13;

	c->z = m->a20 * b->x + m->a21 * b->y + m->a22 * b->z + m->a23;
}
//void
//matrix4H_mult_V( matrix4_type *m, vec3f_type *b, vec3f_type *c )
//{
//
//	c->x = m->a00 * b->x + m->a01 * b->y + m->a02 * b->z;
//
//	c->y = m->a10 * b->x + m->a11 * b->y + m->a12 * b->z;
//
//	c->z = m->a20 * b->x + m->a21 * b->y + m->a22 * b->z;
//}


void  
matrix4_rotationZ( matrix4_type *m, float a )  // rotation around Z axis
{
    m->a00 =cos(a);  m->a01 =-sin(a);    m->a02 =0;   m->a03 =0;
    m->a10 =sin(a);  m->a11 = cos(a);    m->a12 =0;   m->a13 =0;
    m->a20 =0;      m->a21 = 0;         m->a22 =1;   m->a23 =0;
    m->a30 =0;      m->a31 = 0;         m->a32 =0;   m->a33 =1;
}





void 
matrix4_rotationY( matrix4_type *m, float a)
{
    m->a00 =cos(a);  m->a01 =0;       m->a02 =sin(a); m->a03 =0;
    m->a10 =0;       m->a11 =1;       m->a12 =0;      m->a13 =0;
    m->a20 =-sin(a); m->a21 =0;       m->a22 =cos(a); m->a23 =0;
    m->a30 =0;       m->a31 =0;       m->a32 =0;      m->a33 =1;
}

void 
matrix4_rotationX( matrix4_type *m, float a)
{
    m->a00 =1;       m->a01 =0;       m->a02 =0;       m->a03 =0;
    m->a10 =0;       m->a11 =cos(a);  m->a12 =-sin(a); m->a13 =0;
    m->a20 =0;       m->a21 =sin(a);  m->a22 =cos(a);  m->a23 =0;
    m->a30 =0;       m->a31 =0;       m->a32 =0;       m->a33 =1;
}



void	
matrix4_rotationYXZ( matrix4_type *m, float rx, float ry, float rz )
{
matrix4_type mx,	my,	mz,	tm;

	matrix4_rotationZ( &mz, rz );
	matrix4_rotationX( &mx, rx );
	matrix4_rotationY( &my, ry );

	matrix4_mult( &mx, &mz, &tm );
	matrix4_mult( &my, &tm, m );
}


void
matrix4_inverse( matrix4_type *m, matrix4_type *im )
{	
	math_matrixD_inverse4( m->a, im->a );
}


int
matrix4_solve( matrix4_type *m, double D[], double X[] )
{
	if( math_linear_equation4D( m->a, D, X ) < 0 )
		return( -1 );

	return( 1 );
}


void
matrix4_read( matrix4_type *m, FILE *fp )
{
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a00, &m->a01, &m->a02, &m->a03 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a10, &m->a11, &m->a12, &m->a13 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a20, &m->a21, &m->a22, &m->a23 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a30, &m->a31, &m->a32, &m->a33 );
}



int	
matrix4_read_file( matrix4_type *m, char *file )
{
	FILE	*fp;

	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );

	matrix4_read( m, fp );

	fclose( fp );

	return( 1 );
}


void
matrix4_write( matrix4_type *m, FILE *fp )
{
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a00, m->a01, m->a02, m->a03 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a10, m->a11, m->a12, m->a13 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a20, m->a21, m->a22, m->a23 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a30, m->a31, m->a32, m->a33 );
}


void
matrix4H_read( matrix4_type *m, FILE *fp )
{
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a00, &m->a01, &m->a02, &m->a03 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a10, &m->a11, &m->a12, &m->a13 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a20, &m->a21, &m->a22, &m->a23 );

	m->a30 = 0;
	m->a31 = 0;
	m->a32 = 0;
	m->a33 = 1.0;
}

void
matrix4H_write( matrix4_type *m, FILE *fp )
{
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a00, m->a01, m->a02, m->a03 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a10, m->a11, m->a12, m->a13 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a20, m->a21, m->a22, m->a23 );
}




int	
matrix4_write_file( matrix4_type *m, char *file )
{
	FILE	*fp;

	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );

	matrix4_write( m, fp );

	fclose( fp );

	return( 1 );
}

int	
matrix4_dump( matrix4_type *m, char *prefix, int index, char *suffix )
{
FILE	*fp;
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".txt", file ) < 0 )
		return( -1 );

	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );

	matrix4_write( m, fp );

	fclose( fp );

	return( 1 );
}



int
matrix4_to_quaternion( matrix4_type *m, double q[] ) // w  x  y   z
{
	double	T,	S;

	q[4] = m->a03;
	q[5] = m->a13;
	q[6] = m->a23;

	T = 1 + m->a00 + m->a11 + m->a22;


	if( T > 1e-6 ){
		q[0] = sqrt(T) / 2;
		q[1] = (m->a21 - m->a12) / (4*q[0]);
		q[2] = (m->a02 - m->a20) / (4*q[0]);
		q[3] = (m->a10 - m->a01) / (4*q[0]);

		return( 7 );
	}



	if( m->a00 > m->a11 && m->a00 > m->a22 ){
		S = sqrt( 1.0 + m->a00 - m->a11 - m->a22 ) * 2; 
		q[1] = 0.25 * S;
		q[2] = (m->a01 + m->a10 ) / S; 
		q[3] = (m->a02 + m->a20 ) / S; 
		q[0] = (m->a12 - m->a21 ) / S;

		return(7);
	}

	if( m->a11 > m->a22 ){
		S = sqrt( 1.0 + m->a11 - m->a00 - m->a22 ) * 2; 
		q[1] = ( m->a01 + m->a10 ) / S; 
		q[2] = 0.25 * S;
		q[3] = (m->a12 + m->a21 ) / S; 
		q[0] = (m->a02 - m->a20 ) / S;

		return(7);
	}


	S = sqrt( 1.0 + m->a22 - m->a00 - m->a11 ) * 2; 
	q[1] = (m->a02 + m->a20  ) / S; 
	q[2] = (m->a12 + m->a21  ) / S; 
	q[3] = 0.25 * S;
	q[0] = (m->a01 - m->a10 ) / S;


	return( 7 );
}


int
matrix4_from_quaternion( matrix4_type *m, double q[7] ) // w x  y   z
{
double	x,	y,	z,	w,	t;

	w = q[0];	
	x = q[1];
	y = q[2];
	z = q[3];

	t = 1.0 /sqrt(w*w + x*x + y*y + z*z);

	w *= t;
	x *= t;
	y *= t;
	z *= t;




	m->a00 = 1 - 2*y*y - 2*z*z;
	m->a01 = 2*x*y - 2*z*w;
	m->a02 = 2*x*z + 2*y*w;

	m->a10 = 2*x*y + 2*z*w;
	m->a11 = 1-2*x*x - 2*z*z;
	m->a12 = 2*y*z - 2*x*w;

	m->a20 = 2*x*z - 2*y*w;
	m->a21 = 2*y*z + 2*x*w;
	m->a22 = 1-2*x*x - 2*y*y; 

	m->a03 = q[4];
	m->a13 = q[5];
	m->a23 = q[6];


	m->a30 = 0;
	m->a31 = 0;
	m->a32 = 0;
	m->a33 = 1;

	return( 7 );
}




//void
//matrix4H_set( matrix4_type *m, double T[3][4] )
//{
//	int	i,	j;
//
//	for( i = 0 ; i < 3 ; i++ )
//		for( j = 0 ; j < 4 ; j++ )
//			m->a[i][j] = T[i][j];
//
//	m->a[3][0] = m->a[3][1] = m->a[3][2] = 0;
//	m->a[3][3] = 1.0;
//}


void
matrix4H_set( matrix4_type *m, double T[3][4] )
{
	int	i,	j;

	for( i = 0 ; i < 3 ; i++ )
		for( j = 0 ; j < 4 ; j++ )
			m->a[i][j] = T[i][j];

	m->a[3][0] = m->a[3][1] = m->a[3][2] = 0;
	m->a[3][3] = 1.0;
}



void
matrix4H_setV( matrix4_type *m, vec3d_type *R1, vec3d_type *R2, vec3d_type *R3, vec3d_type *T )
{
	m->a00 = R1->x;
	m->a10 = R1->y;
	m->a20 = R1->z;
	m->a30 = 0;

	m->a01 = R2->x;
	m->a11 = R2->y;
	m->a21 = R2->z;
	m->a31 = 0;

	m->a02 = R3->x;
	m->a12 = R3->y;
	m->a22 = R3->z;
	m->a32 = 0;

	m->a03 = T->x;
	m->a13 = T->y;
	m->a23 = T->z;
	m->a33 = 1;
}


void
matrix4H_set_RT( matrix4_type *m, matrix3_type *R, vec3d_type *T )
{
	m->a00 = R->a00;
	m->a10 = R->a10;
	m->a20 = R->a20;
	m->a30 = 0;

	m->a01 = R->a01;
	m->a11 = R->a11;
	m->a21 = R->a21;
	m->a31 = 0;

	m->a02 = R->a02;
	m->a12 = R->a12;
	m->a22 = R->a22;
	m->a32 = 0;

	m->a03 = T->x;
	m->a13 = T->y;
	m->a23 = T->z;
	m->a33 = 1;
}



void
matrix4H_get_RT( matrix4_type *m, matrix3_type *R, vec3d_type *T )
{
	R->a00 = m->a00;
	R->a10 = m->a10;
	R->a20 = m->a20;
	

	R->a01 = m->a01;
	R->a11 = m->a11;
	R->a21 = m->a21;


	R->a02 = m->a02;
	R->a12 = m->a12;
	R->a22 = m->a22;


	T->x = m->a03;
	T->y = m->a13;
	T->z = m->a23;

}

void
matrix4H_get_T( matrix4_type *m, vec3d_type *T )
{
	T->x = m->a03;
	T->y = m->a13;
	T->z = m->a23;
}



void
matrix4H_RT2CP( matrix4_type *m, vec3d_type *cp, vec3d_type *N )
{
	matrix3_type R,	RI;
	vec3d_type	T;

	matrix4H_get_RT( m, &R, &T );
	matrix3_inverse( &R, &RI );
	matrix3_multVD( &RI,&T, cp );

	cp->x = -cp->x;
	cp->y = -cp->y;
	cp->z = -cp->z;

	N->x = m->a20;
	N->y = m->a21;
	N->z = m->a22;
}



void
matrix4H_compose( matrix4_type *m, double x, double y, double z, double omega, double phi, double kappa )
{
	matrix3_type R;
	vec3d_type T;

	matrix3_rotationZYX( &R, -omega, phi, -kappa );

	T.x = x;
	T.y = y;
	T.z = z;

	matrix4H_set_RT( m, &R, &T );
}

void
matrix4H_decompose( matrix4_type *m, double *x, double *y, double *z, double *omega, double *phi, double *kappa )
{
matrix3_type R;
vec3d_type T;

	matrix4H_get_RT( m, &R, &T );

	*x = T.x;
	*y = T.y;
	*z = T.z;

	matrix3_rotationZYX_decompose( &R, omega, phi, kappa );
	*omega = -*omega;
	*phi = *phi;
	*kappa = -*kappa;
}


void
matrix4H_decomposeS( matrix4_type *m, double *x, double *y, double *z, double *omega, double *phi, double *kappa, double *scale )
{
	matrix4_type	sm;
	double	t;
	t = m->a00 * m->a00 + m->a01 * m->a01 +  m->a02 * m->a02;
	*scale = sqrt( t );

	t = 1.0 / *scale;

	matrix4H_multiply_scalar( m, t, &sm );

	matrix4H_decompose( &sm, x, y, z, omega, phi, kappa );
}


void
matrix4H_getV( matrix4_type *m, vec3f_type *R1, vec3f_type *R2, vec3f_type *R3, vec3f_type *T )
{
	R1->x = m->a00;
	R1->y = m->a10;
	R1->z = m->a20;

	R2->x = m->a01;
	R2->y = m->a11;
	R2->z = m->a21;

	R3->x = m->a02;
	R3->y = m->a12;
	R3->z = m->a22;
	

	T->x = m->a03;
	T->y = m->a13;
	T->z = m->a23;

}


double
matrix4H_L2( matrix4_type *m1, matrix4_type *m2 )
{
	int	i,	j;
	double	sum,	t;

	sum = 0;
	for( i = 0 ; i < 3; i++ ){
		for( j = 0; j < 4 ; j++ ){
			t = m1->a[i][j] - m2->a[i][j];

			sum += t*t;
		}
	}

	sum /= 12;

	return( sum );
}


double
matrix4_dmax( matrix4_type *m1, matrix4_type *m2 )
{
	int	i,	j;
	double	max,	t;

	max = 0;
	for( i = 0 ; i < 4; i++ ){
		for( j = 0; j < 4 ; j++ ){
			t = m1->a[i][j] - m2->a[i][j];

			if( t < 0 )	t = -t;
			if( t > max )
				max = t;
		}
	}


	return( max );
}


int
matrix4H_test( matrix4_type	*m )
{
	float t0,	t1,	t2;
	float u0,	u1,	u2;


	t0 = m->a00 * m->a10 + m->a01 * m->a11 +  m->a02 * m->a12;

	t1 = m->a00 * m->a20 + m->a01 * m->a21 +  m->a02 * m->a22;

	t2 = m->a10 * m->a20 + m->a11 * m->a21 +  m->a12 * m->a22;



	

	u0 = m->a00 * m->a00 + m->a01 * m->a01 +  m->a02 * m->a02;
	u1 = m->a10 * m->a10 + m->a11 * m->a11 +  m->a12 * m->a12;
	u2 = m->a20 * m->a20 + m->a21 * m->a21 +  m->a22 * m->a22;

//	fprintf( stdout, "matrix4H_test  %f   %f  %f      %f  %f  %f\n",  t0, t1, t2, u0, u1, u2 );


	if( ABS(t0) > 0.001 ||  ABS(t1) > 0.001 || ABS(t2) > 0.001 )
		return( -1 );

	u0 -= 1.0;
	u1 -= 1.0;
	u2 -= 1.0;
	if( ABS(u0) > 0.001 ||  ABS(u2) > 0.001 || ABS(u2) > 0.001 )
		return( -1 );



	return( 1 );
}


matrix4A_type *
matrix4A_alloc( int n )
{
	matrix4A_type	*am;


	am = (matrix4A_type *)malloc( sizeof( matrix4A_type) );

	am->NA = n;
	am->a = ( matrix4_type *)malloc( am->NA*sizeof(matrix4_type) );

	am->nA = 0;



	return( am );
}


matrix4A_type *
matrix4A_realloc( matrix4A_type *am, int n )
{
	if( am == NULL ){
		am = matrix4A_alloc(n );
		return( am );
	}


	if( am->NA < n ){
		am->a = ( matrix4_type *)realloc( am->a, n*sizeof(matrix4_type) );
		am->NA = n;
	}

	return( am );
}



void
matrix4A_destroy( matrix4A_type *am )
{
	free( am->a );

	free( am );
}


matrix4A_type *
matrix4A_transform( matrix4A_type *oM, matrix4_type *T, matrix4A_type *tM )
{
	int	i;

	tM = matrix4A_realloc( tM, oM->nA );

	for( i = 0; i < oM->nA ; i++ ){

		matrix4_mult( &oM->a[i], T, &tM->a[i] );
	}

	tM->nA = oM->nA;

	return( tM );
}
