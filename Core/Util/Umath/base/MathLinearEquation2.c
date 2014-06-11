/********************************
 ***   MathLinerExquation.c   ***
 ********************************/
#include	<math.h>

#include	"Uigp/igp.h"
#include	"Uvec/Vec2d.h"

#include	"../umath.h"

#define	MSIZE	2


int
math_linear_equation2( float A[2][2], float D[2], float minDet, float X[2] )
{
float	a,	b,	c,	d;
float	det;

	a = A[0][0];
	b = A[0][1];
	c = A[1][0];
	d = A[1][1];


	det = a*d - b*c;


	if( ABS(det) >  minDet ){


		X[0] = ( d * D[0] - b*D[1] ) /det;
		X[1] = ( c * D[0] + a*D[1] ) /det;

		return( 1 );
	}

	return( -1 );
}



int
math_linearD_equation2( double A[2][2], double D[2], double minDet, double X[2] )
{
	double	a,	b,	c,	d;
	double	det;

	a = A[0][0];
	b = A[0][1];
	c = A[1][0];
	d = A[1][1];


	det = a*d - b*c;


	if( ABS(det) >  minDet ){


		X[0] = ( d * D[0] - b*D[1] ) /det;
		X[1] = ( -c * D[0] + a*D[1] ) /det;

		return( 1 );
	}

	return( -1 );
}



// A[0][1] = A[1][0]
int
math_linear_equation2_symtric( float A[2][2], float D[2], float EigThresh, float X[2] )
{
float	a,	b,	c;
float	e1,	e2;
float	det,	desc,	t;
vec2d	ev;

	a = A[0][0];
	b = A[0][1];
	c = A[1][1];


	desc = hypot( a-c, 2*b );
	e1 = ((a+c) + desc ) * 0.5;
	e2 = ((a+c) - desc ) * 0.5;



//	if( ABS(e2) >  EigThresh && ( ABS(e2) >  1.5*EigThresh) || ABS(e2) > ABS(e1)*EigThresh ){
	if( ABS(e2) >  EigThresh ){
		det = a*c - b*b;
		X[0] = ( c * D[0] - b*D[1] ) /det;
		X[1] = (-b * D[0] + a*D[1] ) /det;

		return( 1 );
	}



	if( ABS(e1) >  EigThresh ){
		ev.x = -b;
		ev.y = a - e1;

		t = hypot( ev.x, ev.y);
		ev.x /= t;
		ev.y /= t;

		t = (D[0] * ev.x + D[1] * ev.y) / e1;
		X[0] = ev.x * t;
		X[1] = ev.y * t;


		return( 2 );
	}


	return( -1 );
}

int
math_linearD_equation2_symtric( double A[2][2], double D[2], double EigThresh, double X[2] )
{
double	a,	b,	c;
double	e1,	e2;
double	det,	desc,	t;
vec2d_type	ev;

	a = A[0][0];
	b = A[0][1];
	c = A[1][1];


	det = a*c - b*b;
	if( ABS(det) >  EigThresh ){
		det = a*c - b*b;
		X[0] = ( c * D[0] - b*D[1] ) /det;
		X[1] = (-b * D[0] + a*D[1] ) /det;

		return( 1 );
	}


	desc = hypot( a-c, 2*b );
	e1 = ((a+c) + desc ) * 0.5;
	e2 = ((a+c) - desc ) * 0.5;



	//
	//if( ABS(e2) >  EigThresh ){
	//	det = a*c - b*b;
	//	X[0] = ( c * D[0] - b*D[1] ) /det;
	//	X[1] = (-b * D[0] + a*D[1] ) /det;

	//	return( 1 );
	//}



	if( ABS(e1) >  EigThresh ){
		ev.x = -b;
		ev.y = a - e1;

		t = hypot( ev.x, ev.y);
		ev.x /= t;
		ev.y /= t;

		t = (D[0] * ev.x + D[1] * ev.y) / e1;
		X[0] = ev.x * t;
		X[1] = ev.y * t;


		return( 2 );
	}


	return( -1 );
}






// A[0][1] = A[1][0]
int
math_linear_equation2_symtricN( float A[2][2], float D[2], float EigThresh, float *ee, float X[2] )
{
float	a,	b,	c;
float	e1,	e2;
float	det,	desc,	t;
vec2d	ev;
float	d0,	d1,	t0,	t1;

	a = A[0][0];
	b = A[0][1];
	c = A[1][1];


	desc = hypot( a-c, 2*b );
	e1 = ((a+c) + desc ) * 0.5;
	e2 = ((a+c) - desc ) * 0.5;



	*ee = e2;


//	if( ABS(e2) >  EigThresh && ( ABS(e2) >  1.5*EigThresh) || ABS(e2) > ABS(e1)*EigThresh ){
	if( ABS(e2) > EigThresh ){
		det = a*c - b*b;
		X[0] = ( c * D[0] - b*D[1] ) /det;
		X[1] = (-b * D[0] + a*D[1] ) /det;

		return( 1 );
	}



	ev.x = -b;
	ev.y = a - e1;

	t = hypot( ev.x, ev.y);
	ev.x /= t;
	ev.y /= t;

	d0 = ev.x * D[0] + ev.y * D[1];
	d1 = ev.y * D[0] - ev.x * D[1];


	if( ABS(e2) > 0.5*EigThresh  && ABS(d1) > 2* ABS(e2)){
		t0 = d0 / e1;
		t1 = d1 / e2;

		X[0] = ev.x * t0 + ev.y * t1;
		X[1] = ev.y * t0 - ev.x * t1;

		return( 5 );
	}


	t0 = d0 / e1;
	X[0] = ev.x * t0;
	X[1] = ev.y * t0;

	return( 2 );
}



int
math_linear_equation2_symtric_eigenvalue( float xx, float xy, float yy,
										 float *e1, vec2d *v1, float *e2 )
{
	float	a,	b,	c;
	float	desc,	t;


	a = xx;
	b = xy;
	c = yy;


	desc = hypot( a-c, 2*b );
	*e1 = ((a+c) + desc ) * 0.5;
	*e2 = ((a+c) - desc ) * 0.5;

	v1->x = -b;
	v1->y = a - *e1;

	t = hypot( v1->x, v1->y);
	v1->x /= t;
	v1->y /= t;

	return( 1 );
}



int
math_matrixD_inverse2( double DA[MSIZE][MSIZE], double B[MSIZE][MSIZE] )
{
	double	A[MSIZE][MSIZE];
	double	a;
	int	i,	j,	k,	iMax;
	double	max;

	for( i = 0 ; i < MSIZE ; i++ )
		for( j = 0 ; j < MSIZE ; j++ ){
			B[i][j] = ( i != j )? 0 : 1;
			A[i][j] = DA[i][j];
		}


		for( i = 0 ; i < MSIZE ; i++ ){

			max = A[i][i];
			iMax = i;
			for( k = i+1 ; k < MSIZE ; k++ )
				if( ABS(A[i][k]) > ABS(max) ){
					iMax = k;
					max = A[i][k];
				}

				if( i != iMax ){
					vectorD_swap( A[i], A[iMax], MSIZE );
					vectorD_swap( B[i], B[iMax], MSIZE );
				}

				a = 1.0 / A[i][i];
				vectorD_multiply( A[i], a, MSIZE );
				vectorD_multiply( B[i], a, MSIZE );

				for( j = 0 ; j < MSIZE ; j++ ){
					if( j != i ){
						a = -A[j][i];
						vectorD_acumalute( A[j], A[i], a, MSIZE );
						vectorD_acumalute( B[j], B[i], a, MSIZE ); 
					}
				}
		}

		return( 1 );
}