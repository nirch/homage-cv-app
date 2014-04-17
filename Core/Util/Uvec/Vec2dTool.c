/**********************
 ***	VecTool.c   ***
 **********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Vec2d.h"

#include "Umath/Matrix2Type.h"

vec2fA_type *
vec2fA_alloc( int n )
{
vec2fA_type	*av;

	av = (vec2fA_type *)malloc( sizeof( vec2fA_type) );

	av->a = ( vec2f_type *)malloc( n*sizeof(vec2f_type) );

	av->NA = n;

	av->nA = 0;

	return( av );
}


void
vec2fA_destroy( vec2fA_type *av )
{
	free( av->a );

	free( av );
}

void
vec2d_reorder( vec2f_type V[], int no )
{
vec2f_type	v,	*v0,	*v1,	*v2;
int	i,	j,	I;
int	flag,	tflag;


	for( i = 1 ; i < no-1 ; i++ ){
		v0 = &V[i-1];
		v1 = &V[i];

		flag = -( v1->x * v0->y + v1->y * -v0->x ) > 0;

		I = i;

		for( j = i+1 ; j < no ; j++ ){
			v2 = &V[j];
			tflag = -( v2->x * v0->y + v2->y * -v0->x ) > 0;
			if( -( v2->x * v1->y + v2->y * -v1->x ) > 0 )
					tflag |= 0x02;

			if( (flag == 1 && tflag == 0x01 ) ||
				(flag == 0 && tflag != 0x02 ) ){

				I = j;
				v1 = v2;
				flag = -( v1->x * v0->y + v1->y * -v0->x ) > 0; 
			}
		}

		if( i == I )    continue;

		v = V[i];
		V[i] = V[I];
		V[I] = v;
	}
}

int
vec2d_select( vec2d V[], int no, vec2d *p )
{
vec2f_type	*v;
int	i,	flag,	tflag;

	v = &V[0];
	flag = ( -p->x * v->y + p->y * v->x ) > 0;


	for( i = 1 ; i < no ; i++ ){
		v = &V[i];
		tflag = ( -p->x * v->y + p->y * v->x ) > 0;

		if( flag == 1 && tflag == 0 )	return( i -1 );

		flag = tflag;
	}

	return( no-1 );
}



float
vec2d_distance( vec2f_type *v0 ,vec2f_type *v1 )
{
vec2f_type	dv;
float	dis;

	dv.x = v0->x - v1->x;
	dv.y = v0->y - v1->y;
	dis = hypot( dv.x, dv.y );

	return( dis );
}


float
vec2d_distance2( vec2f_type *v0 ,vec2f_type *v1 )
{
vec2f_type	dv;
float	dis;

	dv.x = v0->x - v1->x;
	dv.y = v0->y - v1->y;
	dis = dv.x*dv.x + dv.y*dv.y;

	return( dis );
}

float
vec2d_angle( vec2f_type *v )		// angle between v and x-axis
{
	float	a;

	a = atan2( v->y, v->x );

	return( a );
}


//float
//vec2d_angle( vec2d *v0 ,vec2d *v1 )
//{
//float	l0,	l1,	t;
//float	angle;
//
//	l0 = hypot( v0->x, v0->y );
//	l1 = hypot( v1->x, v1->y );
//
//	t = (v0->x*v1->x + v0->y*v1->y )/( l0 * l1 );
//	angle = acos( t );
//
//	return( angle );
//}








void 
vec2d_middle( vec2d *p0, vec2d *p1, vec2d *p)
{
	p->x = ( p0->x + p1->x) * 0.5f;
	p->y = ( p0->y + p1->y) * 0.5f;
}



// return vector that crospond to the two parall motion vector
int
vec2d_joint( vec2d *mv0, vec2d *mv1, vec2d *mv )
{
vec2d	v0,	v1;
float	det;
float	l0,	l1;


	l0 = hypot( mv0->x, mv0->y );
	if( l0 < 0.25 )	return( -1 );

	v0.x = mv0->x / l0;
	v0.y = mv0->y / l0;


	l1 = hypot( mv1->x, mv1->y );
	if( l1 < 0.25 )	return( -1 );

	v1.x = mv1->x / l1;
	v1.y = mv1->y / l1;




	det = v0.x * v1.y -v0.y *v1.x;

	if( ABS(det) < 0.382 ){//0.923 ){	// 22.5 dgree

		return( -1 );
	}


	mv->x = (l0 * v1.y - l0 *v0.y)/ det;
	mv->y = (v0.x * l1 - v1.x *l0)/ det;

	return( 1 );
}




int
vec2fA_write_to_file( vec2fA_type *av, char *file )
{
	return vec2dA_write_to_file( av->a, av->nA, file );
}


int
vec2dA_write_to_file( vec2d av[], int nV, char *file )
{
FILE	*fp;
int	i;

	if( (fp = fopen( file, "wb" ) ) == NULL )
		return( -1 );

	for( i = 0 ; i < nV ; i++ ){
		fprintf( fp, "%f %f %f\n", av[i].x, av[i].y, 0 );
	}


	fclose( fp );

	return( 1 );
}


void
vec2fA_approximate_line( vec2fA_type *av, vec2f_type *ctr, vec2f_type *v, float *len )
{
	vec2f_type	vy,	vx,	v0,	dv,	dv0,	u;
	float	x,	y,	t;
	int	i;
	matrix2_type	m;
	vec2d_type	D,	X;
	float	a,	b;

	v0 = av->a[0];

	vx.x = av->a[av->nA-1].x - av->a[0].x;
	vx.y = av->a[av->nA-1].y - av->a[0].y;

	t = hypot( vx.x, vx.y );
	vx.x = vx.x / t;
	vx.y = vx.y / t;

	vy.x = -vx.y;
	vy.y =  vx.x;



	D.x = D.y = 0;
	matrix2_zero( &m );
	for( i = 0 ; i < av->nA ; i++ ){
		dv.x = av->a[i].x - av->a[0].x;
		dv.y = av->a[i].y - av->a[0].y;

		x = vx.x * dv.x + vx.y * dv.y;
		y = vy.x * dv.x + vy.y * dv.y;

		m.a00 += x*x;
		m.a01 += x;
		D.x += x*y;

		m.a11 += 1;

		D.y += y;
	}

	m.a10 = m.a01;

	matrix2S_solve( &m, &D, &X );

	a = X.x;
	b = X.y;


	dv.x = a;
	dv.y = -1;
	t = hypot( dv.x, dv.y );
	dv.x /= t;
	dv.y /= t;

	dv0.x = -b * dv.x / t;
	dv0.y = -b * dv.y / t;


	u.x = vx.x * dv.x + vy.x * dv.y;
	u.y = vx.y * dv.x + vy.y * dv.y;

	v->x = u.y;
	v->y = -u.x;

	v0.x = vx.x * dv0.x + vy.x * dv0.y + av->a[0].x;
	v0.y = vx.y * dv0.x + vy.y * dv0.y + av->a[0].y;


	t = v->x * ( av->a[0].x - v0.x )  +  v->y * ( av->a[0].y - v0.y );

	ctr->x = v0.x + t * v->x;
	ctr->y = v0.y + t * v->y;

	*len = v->x * ( av->a[av->nA-1].x - v0.x )  +  v->y * ( av->a[av->nA-1].y - v0.y ) - t;

}


int
vec2fA_select( vec2fA_type *av, vec2d *p, float dT )
{
	vec2f_type	dv;
	int	i,	iMin;
	float	dMin,	d;

	iMin = -1;

	for( i = 0 ; i < av->nA ; i++ ){
		dv.x = p->x - av->a[i].x;
		dv.y = p->y - av->a[i].y;

		d = dv.x * dv.x + dv.y*dv.y;

		if( iMin < 0 || d < dMin ){
			dMin = d;
			iMin = i;
		}
	}

	if( dMin < dT*dT )
		return( iMin );

	return( -1 );
}