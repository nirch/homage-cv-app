/*********************
 ***   Matrix3Tool.c   ***
 ********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/umath.h"
#include	"Matrix3Type.h"


#define		MSIZE	3



void
matrix3_unit( matrix3_type *m )
{
	m->a00 = 1; m->a01 = 0; m->a02 = 0;
	m->a10 = 0; m->a11 = 1; m->a12 = 0;
	m->a20 = 0; m->a21 = 0; m->a22 = 1;

}

void
matrix3_zero( matrix3_type *m )
{
	m->a00 = 0; m->a01 = 0; m->a02 = 0;
	m->a10 = 0; m->a11 = 0; m->a12 = 0;
	m->a20 = 0; m->a21 = 0; m->a22 = 0;
}

void
matrix3_set( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 )
{
	m->a00 = R0->x;
	m->a10 = R0->y;
	m->a20 = R0->z;

	m->a01 = R1->x;
	m->a11 = R1->y;
	m->a21 = R1->z;

	m->a02 = R2->x;
	m->a12 = R2->y;
	m->a22 = R2->z;
}


void
matrix3_get( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 )
{
	R0->x = m->a00;
	R0->y = m->a10;
	R0->z = m->a20;

	R1->x = m->a01;
	R1->y = m->a11;
	R1->z = m->a21;

	R2->x = m->a02;
	R2->y = m->a12;
	R2->z = m->a22;
}

void
matrix3_set_row( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 )
{
	m->a00 = R0->x;
	m->a01 = R0->y;
	m->a02 = R0->z;

	m->a10 = R1->x;
	m->a11 = R1->y;
	m->a12 = R1->z;

	m->a20 = R2->x;
	m->a21 = R2->y;
	m->a22 = R2->z;
}

void
matrix3_plus( matrix3_type *a, matrix3_type *b, matrix3_type *m )
{
	m->a00 = a->a00 + b->a00; 
	m->a01 = a->a01 + b->a01; 
	m->a02 = a->a02 + b->a02;
					
	m->a10 = a->a10 + b->a10;
	m->a11 = a->a11 + b->a11; 
	m->a12 = a->a12 + b->a12;
					
	m->a20 = a->a20 + b->a20; 
	m->a21 = a->a21 + b->a21; 
	m->a22 = a->a22 + b->a22;

}

void
matrix3_mult_scalar( matrix3_type *a, double s, matrix3_type *m )
{
	m->a00 = s * a->a00; 
	m->a01 = s * a->a01; 
	m->a02 = s * a->a02;

	m->a10 = s * a->a10;
	m->a11 = s * a->a11; 
	m->a12 = s * a->a12;

	m->a20 = s * a->a20; 
	m->a21 = s * a->a21; 
	m->a22 = s * a->a22;

}

void
matrix3_transpose( matrix3_type *a, double s, matrix3_type *m )
{
	m->a00 = a->a00; 
	m->a01 = a->a10; 
	m->a02 = a->a20;

	m->a10 = a->a01;
	m->a11 = a->a11; 
	m->a12 = a->a21;

	m->a20 = a->a02; 
	m->a21 = a->a12; 
	m->a22 = a->a22;

}
void
matrix3_mult( matrix3_type *a, matrix3_type *b, matrix3_type *c )
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
matrix3_mult_vector( matrix3_type *a, float *b, float *c )
{
int i, j;

	for( i = 0 ; i < MSIZE ; i++ ){

		c[i] = 0;

		for( j = 0 ; j < MSIZE ; j++)
			c[i] += a->a[i][j] * b[j];
	}
}



void
matrix3_multV( matrix3_type *m, vec3f_type *b, vec3f_type *c )
{
	c->x = m->a00*b->x + m->a01*b->y + m->a02*b->z;
	c->y = m->a10*b->x + m->a11*b->y + m->a12*b->z;
	c->z = m->a20*b->x + m->a21*b->y + m->a22*b->z;
}

void
matrix3_multVD( matrix3_type *m, vec3d_type *b, vec3d_type *c )
{
	c->x = m->a00*b->x + m->a01*b->y + m->a02*b->z;
	c->y = m->a10*b->x + m->a11*b->y + m->a12*b->z;
	c->z = m->a20*b->x + m->a21*b->y + m->a22*b->z;
}

void
matrix3_multVD_T( matrix3_type *m, vec3d_type *b, vec3d_type *c )
{
	c->x = m->a00*b->x + m->a10*b->y + m->a20*b->z;
	c->y = m->a01*b->x + m->a11*b->y + m->a21*b->z;
	c->z = m->a02*b->x + m->a12*b->y + m->a22*b->z;
}

void
matrix3_multV_N( matrix3_type *m, float bx, float by, float bz, float *cx, float *cy, float *cz )
{
	*cx = m->a00*bx + m->a01*by + m->a02*bz;
	*cy = m->a10*bx + m->a11*by + m->a12*bz;
	*cz = m->a20*bx + m->a21*by + m->a22*bz;
}

void  
matrix3_rotationZ( matrix3_type *m, double a )  // rotation around Z axis
{
 
    m->a00 =cos(a);  m->a01 =-sin(a);    m->a02 =0;
    m->a10 =sin(a);  m->a11 = cos(a);    m->a12 =0;
    m->a20 =0;       m->a21 = 0;         m->a22 =1;
}





void 
matrix3_rotationY( matrix3_type *m, double a)
{
    m->a00 =cos(a);  m->a01 =0;       m->a02 =-sin(a);
    m->a10 =0;       m->a11 =1;       m->a12 =0;     
    m->a20 =sin(a); m->a21 =0;       m->a22 =cos(a);
}

void 
matrix3_rotationX( matrix3_type *m, double a)
{
    m->a00 =1;       m->a01 =0;       m->a02 =0;      
    m->a10 =0;       m->a11 =cos(a);  m->a12 =-sin(a);
    m->a20 =0;       m->a21 =sin(a);  m->a22 =cos(a); 
}


void	
matrix3_rotationXYZ( matrix3_type *m, double rx, double ry, double rz )
{
matrix3_type mx,	my,	mz,	tm;

	matrix3_rotationZ( &mz, rz );
	matrix3_rotationX( &mx, rx );
	matrix3_rotationY( &my, ry );

	matrix3_mult( &my, &mz, &tm );
	matrix3_mult( &mx, &tm, m );
}



void	
matrix3_rotationYXZ( matrix3_type *m, double rx, double ry, double rz )
{
matrix3_type mx,	my,	mz,	tm;

	matrix3_rotationZ( &mz, rz );
	matrix3_rotationX( &mx, rx );
	matrix3_rotationY( &my, ry );

	matrix3_mult( &mx, &mz, &tm );
	matrix3_mult( &my, &tm, m );
}

void	
matrix3_rotationYZX( matrix3_type *m, double rx, double ry, double rz )
{
	matrix3_type mx,	my,	mz,	tm;

	matrix3_rotationZ( &mz, rz );
	matrix3_rotationX( &mx, rx );
	matrix3_rotationY( &my, ry );

	matrix3_mult( &mz, &mx, &tm );
	matrix3_mult( &my, &tm, m );
}

void	
matrix3_rotationZYX( matrix3_type *m, double rx, double ry, double rz )
{
	matrix3_type mx,	my,	mz,	tm;

	matrix3_rotationZ( &mz, rz );
	matrix3_rotationX( &mx, rx );
	matrix3_rotationY( &my, ry );

	matrix3_mult( &my, &mx, &tm );
	matrix3_mult( &mz, &tm, m );
}



void	
matrix3_rotationYXZ_inv( matrix3_type *m, float rx, float ry, float rz )
{
matrix3_type mx,	my,	mz,	tm;

	matrix3_rotationZ( &mz, -rz );
	matrix3_rotationX( &mx, -rx );
	matrix3_rotationY( &my, -ry );

	matrix3_mult( &mx, &my, &tm );
	matrix3_mult( &mz, &tm, m );
}



void	
matrix3_rotationYXZ_decompose_1( matrix3_type *m, double *rx, double *ry, double *rz )
{
	*rx = asin( -m->a12 );

	*ry = atan2(  -m->a02, m->a22 );


	*rz = atan2(  m->a10, m->a11 );
}



void	
matrix3_rotationZYX_decompose( matrix3_type *m, double *rx, double *ry, double *rz )
{
	*ry = asin( m->a20 );		// sy

	*rx = atan2(  m->a21,  m->a22);   // sx*cy ,  cx*cy

	*rz = atan2(  m->a10, m->a00 );  // sz*cy,  cz*cy
}


void matrix3_rotation_OPK( matrix3_type *m, double omega, double phi, double kappa )
{
	matrix3_rotationZYX( m, -omega, phi, -kappa );
}


void matrix3_rotation_OPK_decompose( matrix3_type *m, double *omega, double *phi, double *kappa )
{

	matrix3_rotationZYX_decompose( m, omega, phi, kappa );

	*omega = -*omega;
	*phi = *phi;
	*kappa = -*kappa;
}


void
matrix3_inverse( matrix3_type *m, matrix3_type *im )
{	
	math_matrixD_inverse3( m->a, im->a );
}


void
matrix3_solve( matrix3_type *m, vec3d_type *D, vec3d_type *X )
{
matrix3_type	im;

	math_matrixD_inverse3( m->a, im.a );

	matrix3_multVD( &im, D, X );
}


void
matrix3S_solve( matrix3_type *m, vec3d_type *D, vec3d_type *X )
{
	matrix3_type	im;

	math_matrixD_inverse3( m->a, im.a );

	matrix3_multVD( &im, D, X );
}







void
matrix3_column( matrix3_type *m, vec3d_type *R0, vec3d_type *R1, vec3d_type *R2 )
{
	R0->x = m->a00;
	R0->y = m->a10;
	R0->z = m->a20;

	R1->x = m->a01;
	R1->y = m->a11;
	R1->z = m->a21;

	R2->x = m->a02;
	R2->y = m->a12;
	R2->z = m->a22;

}

void
matrix3_column_i( matrix3_type *m, int j, vec3d_type *c )
{
	c->x = m->a[0][j];
	c->y = m->a[1][j];
	c->z = m->a[2][j];
}


void
matrix3_max_diff( matrix3_type *m0, matrix3_type *m1, matrix3_type *md )
{
float	t;
int	i;

	for( i = 0 ; i < 9 ; i++ ){
		t = m0->A[i] - m1->A[i];
		if( t < 0 )	t = -t;
		if( t > md->A[i] )	md->A[i] = t;
	}
}


int
matrix3_write_to_file( matrix3_type *m, char *file )
{
FILE *fp;

	if( (fp = fopen( file, "wb" )) == NULL )
		return( -1 );


	matrix3_write( m, fp );

	fclose( fp );

	return( 1 );
}

void
matrix3_write( matrix3_type *m, FILE *fp )
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


int	
matrix3_dump( matrix3_type *m, char *prefix, int index, char *suffix )
{
	FILE	*fp;
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".txt", file ) < 0 )
		return( -1 );

	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );

	matrix3_write( m, fp );

	fclose( fp );

	return( 1 );
}


int
matrix3_to_quaternion( matrix3_type *m, double q[] ) // w  x  y   z
{
	double	T,	S;

	T = 1 + m->a00 + m->a11 + m->a22;


	if( T > 1e-6 ){
		q[0] = sqrt(T) / 2;
		q[1] = (m->a21 - m->a12) / (4*q[0]);
		q[2] = (m->a02 - m->a20) / (4*q[0]);
		q[3] = (m->a10 - m->a01) / (4*q[0]);

		return( 4 );
	}



	if( m->a00 > m->a11 && m->a00 > m->a22 ){
		S = sqrt( 1.0 + m->a00 - m->a11 - m->a22 ) * 2; 
		q[1] = 0.25 * S;
		q[2] = (m->a01 + m->a10 ) / S; 
		q[3] = (m->a02 + m->a20 ) / S; 
		q[0] = (m->a12 - m->a21 ) / S;

		return( 4 );
	}

	if( m->a11 > m->a22 ){
		S = sqrt( 1.0 + m->a11 - m->a00 - m->a22 ) * 2; 
		q[1] = ( m->a01 + m->a10 ) / S; 
		q[2] = 0.25 * S;
		q[3] = (m->a12 + m->a21 ) / S; 
		q[0] = (m->a02 - m->a20 ) / S;

		return( 4 );
	}


	S = sqrt( 1.0 + m->a22 - m->a00 - m->a11 ) * 2; 
	q[1] = (m->a02 + m->a20  ) / S; 
	q[2] = (m->a12 + m->a21  ) / S; 
	q[3] = 0.25 * S;
	q[0] = (m->a01 - m->a10 ) / S;

	return( 4 );
}


void
matrix3_from_quaternion( matrix3_type *m, double q[] ) // w x  y   z
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
}




double
matrix3_L2( matrix3_type *m1, matrix3_type *m2 )
{
	int	i,	j;
	double	sum,	t;

	sum = 0;
	for( i = 0 ; i < 3; i++ ){
		for( j = 0; j < 3 ; j++ ){
			t = m1->a[i][j] - m2->a[i][j];

			sum += t*t;
		}
	}

	sum /= 9;

	return( sum );
}


double
matrix3_dmax( matrix3_type *m1, matrix3_type *m2 )
{
	int	i,	j;
	double	max,	t;

	max = 0;
	for( i = 0 ; i < 3; i++ ){
		for( j = 0; j < 3 ; j++ ){
			t = m1->a[i][j] - m2->a[i][j];

			if( t < 0 )	t = -t;
			if( t > max )
				max = t;
		}
	}


	return( max );
}



int
matrix3_test( matrix3_type *m )
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


	// a*X = y;
void
matrix3_linaer_equation_add( matrix3_type *m, double D[], double X[], double y )
{
	int	i,	j;

	for( i = 0 ; i < 3 ; i++  ){

		for( j = 0 ; j < 3 ; j++ ){
			m->a[i][j] += X[j] *X[i];
		}

		D[i] += y*X[i];
	}
}