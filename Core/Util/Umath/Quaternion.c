/************************
 ***   Quaternion.c   ***
 ************************/
#include	<math.h>
#include	"Uigp/igp.h"

#include	"umath.h"
#include	"Matrix3Type.h"


void
quaternion_from_rotation( matrix3_type *m, double q[] ) // qw  qx  qy   qz
{
double	T,	S;

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
quaternion_to_rotation( double q[4], matrix3_type *m ) // qw qx  qy   qz
{
double	qx,	qy,	qz,	qw;
double	t;

	qw = q[0];	
	qx = q[1];
	qy = q[2];
	qz = q[3];

	t = 1.0 /sqrt(qw*qw + qx*qx + qy*qy + qz*qz);

	qw *= t;
	qx *= t;
	qy *= t;
	qz *= t;




	m->a00 = 1-2*qy*qy - 2*qz*qz;
	m->a01 = 2*qx*qy - 2*qz*qw;
	m->a02 = 2*qx*qz + 2*qy*qw; 

    m->a10 = 2*qx*qy + 2*qz*qw;
	m->a11 = 1-2*qx*qx - 2*qz*qz;
	m->a12 = 2*qy*qz - 2*qx*qw;

    m->a20 = 2*qx*qz - 2*qy*qw;
	m->a21 = 2*qy*qz + 2*qx*qw;
	m->a22 = 1-2*qx*qx - 2*qy*qy; 
}

/*
void
quaternion_to_rotation( double qx, double qy, double qz, double qw, matrix3_type *m ) // qx  qy   qz   qw
{

	m->a00 = 1-2*qy*qy - 2*qz*qz;
	m->a01 = 2*qx*qy - 2*qz*qw;
	m->a02 = 2*qx*qz + 2*qy*qw; 

    m->a10 = 2*qx*qy + 2*qz*qw;
	m->a11 = 1-2*qx*qx - 2*qz*qz;
	m->a12 = 2*qy*qz - 2*qx*qw;

    m->a20 = 2*qx*qz - 2*qy*qw;
	m->a21 = 2*qy*qz + 2*qx*qw;
	m->a22 = 1-2*qx*qx - 2*qy*qy; 
}
*/