/*********************
 ***   Matrix4.c   ***
 ********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Uvec/Vec3d.h"
#include	"umath.h"

#include	"Matrix4Type.h"


#define		MSIZE	4

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
matrix4H_mult_vector( matrix4_type *m, vec3d *b, vec3d *c )
{

	c->x = m->a00 * b->x + m->a01 * b->y + m->a02 * b->z + m->a03;
	
	c->y = m->a10 * b->x + m->a11 * b->y + m->a12 * b->z + m->a13;

	c->z = m->a20 * b->x + m->a21 * b->y + m->a22 * b->z + m->a23;
}

void
matrix4H_mult_V( matrix4_type *m, vec3d *b, vec3d *c )
{

	c->x = m->a00 * b->x + m->a01 * b->y + m->a02 * b->z;

	c->y = m->a10 * b->x + m->a11 * b->y + m->a12 * b->z;

	c->z = m->a20 * b->x + m->a21 * b->y + m->a22 * b->z;
}


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

void
matrix4_read( matrix4_type *m, FILE *fp )
{
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a00, &m->a01, &m->a02, &m->a03 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a10, &m->a11, &m->a12, &m->a13 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a20, &m->a21, &m->a22, &m->a23 );
	fscanf( fp, "%lf %lf  %lf  %lf", &m->a30, &m->a31, &m->a32, &m->a33 );
}


void
matrix4_write( matrix4_type *m, FILE *fp )
{
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a00, m->a01, m->a02, m->a03 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a10, m->a11, m->a12, m->a13 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a20, m->a21, m->a22, m->a23 );
	fprintf( fp, "%lf  %lf  %lf  %lf\n", m->a30, m->a31, m->a32, m->a33 );
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


void
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

		return;
	}



	if( m->a00 > m->a11 && m->a00 > m->a22 ){
		S = sqrt( 1.0 + m->a00 - m->a11 - m->a22 ) * 2; 
		q[1] = 0.25 * S;
		q[2] = (m->a01 + m->a10 ) / S; 
		q[3] = (m->a02 + m->a20 ) / S; 
		q[0] = (m->a12 - m->a21 ) / S;

		return;
	}

	if( m->a11 > m->a22 ){
		S = sqrt( 1.0 + m->a11 - m->a00 - m->a22 ) * 2; 
		q[1] = ( m->a01 + m->a10 ) / S; 
		q[2] = 0.25 * S;
		q[3] = (m->a12 + m->a21 ) / S; 
		q[0] = (m->a02 - m->a20 ) / S;

		return;
	}


	S = sqrt( 1.0 + m->a22 - m->a00 - m->a11 ) * 2; 
	q[1] = (m->a02 + m->a20  ) / S; 
	q[2] = (m->a12 + m->a21  ) / S; 
	q[3] = 0.25 * S;
	q[0] = (m->a01 - m->a10 ) / S;
}


void
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
}




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
matrix4H_setV( matrix4_type *m, vec3f_type *R1, vec3f_type *R2, vec3f_type *R3, vec3f_type *T )
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