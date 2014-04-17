/**********************
 ***	VecTool.c   ***
 **********************/
#include	<math.h>

#include	"Uigp/igp.h"
#include	"TriangleType.h"
#include	<math.h>


triangleA_type *
triangleA_alloc( int n )
{
triangleA_type	*at;

	at = (triangleA_type *)malloc( sizeof( triangleA_type) );

	at->t = ( triangle_type *)malloc( n*sizeof(triangle_type) );


	at->NT = n;

	at->nT = 0;

	return( at );
}


void
triangleA_destroy( triangleA_type *at )
{
	free( at->t );

	free( at );
}



void	
triangleA_add( triangleA_type *at, pt2dA_type *apt, int i0, int i1, int i2 )
{
triangle_type	*t;
int	i;

	for( i = 0 ; i < at->nT ; i++ ){
		t = &at->t[i];	

		if( t->i0 == i0 ){
			if( t->i1 == i1 && t->i2 == i2 || t->i1 == i2 && t->i2 == i1 )
				return;
		}

		if( t->i1 == i0 ){
			if( t->i0 == i1 && t->i2 == i2 || t->i0 == i2 && t->i2 == i1 )
				return;
		}

		if( t->i2 == i0 ){
			if( t->i0 == i1 && t->i1 == i2 || t->i0 == i2 && t->i1 == i1 )
				return;
		}
	}

	t = &at->t[at->nT++];

	t->i0 = i0;
	t->i1 = i1;
	t->i2 = i2;

	t->p[0] = apt->p[i0].p;
	t->p[1] = apt->p[i1].p;
	t->p[2] = apt->p[i2].p;

	t->area = triangle_aera( &t->p[0], &t->p[1], &t->p[2] );
}



float
triangle_aera( vec2d *p0, vec2d *p1, vec2d *p2 )
{
	vec2d	v;
	float	t,	h,	a;

	v.x = p1->x - p0->x;
	v.y = p1->y - p0->y;

	t = hypot( v.x, v.y );
	v.x /= t;
	v.y /= t;


	h = v.y*(p2->x - p0->x) - v.x*(p2->y - p0->y);
	if( h < 0 )	 h = -h;

	a = h*t/2;

	return( a );
}


void	
triangleA_measure( triangleA_type *at, float *av, float *var )
{
float	sum,	vsum;
	triangle_type	*t;
	int	i;

	sum = 0;
	vsum = 0;
	for( i = 0 ; i < at->nT ; i++ ){
		t = &at->t[i];	
		sum += t->area;

		vsum += t->area*t->area;
	}

	*av = sum / at->nT;

	*var = sqrt( vsum/at->nT - (*av) * (*av) );
}



void
triangle_bounded_circle( triangle_type	*tr, vec2f_type *ctr, float *r )
{
vec2f_type	v1,	v2,	v3,	v;
vec2f_type	u1,	u3,	u;
float	t;

	v1.x = tr->p[1].x - tr->p[0].x;
	v1.y = tr->p[1].y - tr->p[0].y;

	t = hypot( v1.x, v1.y );
	v1.x /= t;
	v1.y /= t;


	v2.x = tr->p[2].x - tr->p[0].x;
	v2.y = tr->p[2].y - tr->p[0].y;
	t = hypot( v2.x, v2.y );
	v2.x /= t;
	v2.y /= t;



	v3.x = tr->p[2].x - tr->p[1].x;
	v3.y = tr->p[2].y - tr->p[1].y;
	t = hypot( v3.x, v3.y );
	v3.x /= t;
	v3.y /= t;


	u1.x = -v1.y;
	u1.y = v1.x;
	if( (tr->p[2].x - tr->p[0].x)*u1.x +  (tr->p[2].y - tr->p[0].y)*u1.y <  0 ){
		u1.x = -u1.x;
		u1.y = -u1.y;
	}

	u3.x = -v3.y;
	u3.y = v3.x;
	if( (tr->p[0].x - tr->p[1].x)*u3.x +  (tr->p[0].y - tr->p[1].y)*u3.y <  0 ){
		u3.x = -u3.x;
		u3.y = -u3.y;
	}



	v.x = 0.5*(v1.x + v2.x );
	v.y = 0.5*(v1.y + v2.y );


	u.x = u1.x - u3.x;
	u.y = u1.y - u3.y;

	t = (- VEC2D_DOT( tr->p[1], u3 ) + VEC2D_DOT( tr->p[0], u3 ) )/VEC2D_DOT( v, u );

	ctr->x = tr->p[0].x + t* v.x;
	ctr->y = tr->p[0].y + t* v.y;

	*r = t * VEC2D_DOT( v, u1 );

	//r1 = ( ctr->x - tr->p[0].x ) * u1.x + ( ctr->y - tr->p[0].y ) *u1.y;
	//r2 = ( ctr->x - tr->p[1].x ) * u3.x + ( ctr->y - tr->p[1].y ) *u3.y;

}




pt2dA_type *
triangleA_circle( triangleA_type *at, float areaT )
{
pt2dA_type	*apt;
pt2d_type	*pt;
triangle_type	*t;
int	i;

	apt = pt2dA_alloc( at->nT );
	apt->nP = 0;

	for( i = 0 ; i < at->nT ; i++ ){
		t = &at->t[i];	
		if( t->area < areaT )	continue;

		pt = &apt->p[apt->nP++];


		triangle_bounded_circle( t, &pt->p, &pt->r );
		pt->r = sqrt( t->area / M_PI );
	}

	return( apt );
}