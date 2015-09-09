/*************************
 ***	GeoAppLine.c   ***
 *************************/

#include	<stdio.h>
#include	<math.h>

#include	"Uigp/igp.h" 
#include	"Uvec/Vec2D.h"
#include	 "Umath/Matrix2Type.h"


static int	app_l1( vec2d v[], int no, float *a, float *b );

static float	app_l1_test( vec2d v[], int no, float a, float b );




int
geo_approximate_line( vec2d v[], int no, float max_err, vec2d *p0, vec2d *p1 )
{
vec2d	vec[4096];
vec2d	dv;
float	t,	x,	y,	a,	b,	err;
int	i;



	dv.x = v[no-1].x - v[0].x;
	dv.y = v[no-1].y - v[0].y;

	t = hypot( dv.x, dv.y );

	dv.x = dv.x / t;
	dv.y = dv.y / t;


	for( i = 0 ; i < no ; i++ ){
		vec[i].x =  (v[i].x - v[0].x) *dv.x  +  (v[i].y - v[0].y)*dv.y;
		vec[i].y = -(v[i].x - v[0].x) *dv.y  +  (v[i].y - v[0].y)*dv.x;
	}



	if( app_l1( vec, no, &a, &b ) < 0 )
		return( -1 );

	err = app_l1_test( vec, no, a, b );
	if( err > max_err )	return( -1 );


	x = 0;
	y = b;

	p0->x = v[0].x + x*dv.x - y * dv.y;
	p0->y = v[0].y + x*dv.y + y * dv.x;


	x = vec[no-1].x;
	y = a*x + b;

	p1->x = v[0].x + x*dv.x - y * dv.y;
	p1->y = v[0].y + x*dv.y + y * dv.x;


	return ( 1 );
}

static int
app_l1( vec2d v[], int no, float *a, float *b )
{
matrix2_type	m;
vec2d_type		D,	X;

	int	i;


	matrix2_zero( &m );
	D.x = D.y = 0;

	for( i = 0 ; i < no ; i++ ){
		m.a00 += v[i].x*v[i].x;
		m.a01 += v[i].x;

		m.a11 += 1;

		D.x += v[i].x * v[i].y;
		D.y += v[i].y;

	}

	m.a10 = m.a01;

	if( matrix2_solve( &m, &D, &X ) < 0 )
		return( -1 );


	*a = X.x;
	*b = X.y;

	return( 1 );
}







static float
app_l1_test( vec2d v[], int no, float a, float b )
{
float	y,	d,	max;
int	i;


	max = 0;

	for( i = 0 ; i < no ; i++ ){

		y = a* v[i].x + b;
		d = y - v[i].y;
		if( d < 0 )	d = -d;

		if( d > max )	max = d;
	}

	return( max );
}
