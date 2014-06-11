/*********************
 ***   LT2Tool.h   ***
 *********************/
#include <stdio.h>
#include <math.h>

#include "Uigp/igp.h"

#include "Matrix3Type.h"
#include "Matrix2Type.h"

#include "EigenType.h"

#include "LT2Type.h"


void	
lt2_clear( lt2_type *lt )
{
	LT2_SET_F1( *lt, 0, 0, 0 );
	LT2_SET_F2( *lt, 0, 0, 0 );
}

void	
lt2_unity( lt2_type *lt )
{
	LT2_SET_F1( *lt, 1, 0, 0 );
	LT2_SET_F2( *lt, 0, 1, 0 );
}

void	
lt2_affine_setV( lt2_type *lt, float a, float b, float s, float t )
{
	lt->a0 = s;
	lt->b0 = t;
	lt->c0 = a;

	lt->a1 = -t;
	lt->b1 = s;
	lt->c1 = b;
}

#ifdef _AA_
void	
lt2_similarity_set( lt2_type *lt, float a, float b, float angle, float scale )
{
	lt->a0 = scale*cos(angle);
	lt->b0 = scale*sin(angle);
	lt->c0 = a;

	lt->a1 = -scale*sin(angle);;
	lt->b1 = scale*cos(angle);;
	lt->c1 = b;
}
#endif

void	
lt2_similarity_set( lt2_type *lt, float a, float b, float angle, float scale )
{
float	s,	t;

	s = scale * cos( angle );
	t = scale * sin( angle );

	lt->a0 = s;
	lt->b0 = t;
	lt->c0 = a;

	lt->a1 = -t;
	lt->b1 = s;
	lt->c1 = b;

	//ctr.x = c->ctr.x *  s + c->ctr.y * t + a;
	//ctr.y = c->ctr.x * -t + c->ctr.y * s + b;
}


void	
lt2_similarity_get( lt2_type *lt, float *a, float *b, float *angle, float *scale )
{
	*scale = hypot( lt->a0, lt->b0 );

	*angle = atan2( lt->b0, lt->a0 );

	*a = lt->c0;
	*b = lt->c1;
}


int	
lt2_inverse( lt2_type *t, lt2_type *it )
{
float	det;
float	f1[2],	f2[2];

	det = t->f1[0] * t->f2[1] - t->f1[1] * t->f2[0];

	it->f1[0] = f1[0] = t->f2[1] / det;
	it->f1[1] = f1[1] = -t->f1[1] / det;

	it->f2[0] = f2[0] = -t->f2[0] / det;
	it->f2[1] = f2[1] = t->f1[0] / det;

	it->f1[2] =  (float) -(f1[0] * t->f1[2] + f1[1] * t->f2[2] );
	it->f2[2] =  (float) -(f2[0] * t->f1[2] + f2[1] * t->f2[2] );

	return( 1 );
}


/*
int	
lt2_inverse( lt2_type *t, lt2_type *it )
{
float	det;

	det = t->f1[0] * t->f2[1] - t->f1[1] * t->f2[0];

	it->f1[0] = t->f2[1] / det;
	it->f1[1] = -t->f1[1] / det;

	it->f2[0] = -t->f2[0] / det;
	it->f2[1] = t->f1[0] / det;

	it->f1[2] =  -(it->f1[0] * t->f1[2] + it->f1[1] * t->f2[2] );
	it->f2[2] =  -(it->f2[0] * t->f1[2] + it->f2[1] * t->f2[2] );

	return( 1 );
}
*/


void	
lt2_shift( lt2_type *t, float x0, float y0, lt2_type *it )
{
	LT2_F1_C( *it ) = LT2_F1( *t, x0, y0 );
	LT2_F1_A( *it ) = LT2_F1_A( *t );
	LT2_F1_B( *it ) = LT2_F1_B( *t );


	LT2_F2_C( *it ) = LT2_F2( *t, x0, y0 );
	LT2_F2_A( *it ) = LT2_F2_A( *t );
	LT2_F2_B( *it ) = LT2_F2_B( *t );
}

#ifdef _AA_
void	
lt2_scale( lt2_type *t, float scale )
{
	LT2_F1_A( *t ) *= scale;
	LT2_F1_B( *t ) *= scale;

	LT2_F2_A( *t ) *= scale;
	LT2_F2_B( *t ) *= scale;
}
#endif

void	
lt2_scaleN( lt2_type *t, float scale )
{
	LT2_F1_A( *t ) *= scale;
	LT2_F1_B( *t ) *= scale;
	LT2_F1_C( *t ) *= scale;


	LT2_F2_A( *t ) *= scale;
	LT2_F2_B( *t ) *= scale;
	LT2_F2_C( *t ) *= scale;
}




#define	A(t)	LT2_F1_A(t)
#define	B(t)	LT2_F1_B(t)
#define	C(t)	LT2_F1_C(t)


#define	D(t)	LT2_F2_A(t)
#define	E(t)	LT2_F2_B(t)
#define	F(t)	LT2_F2_C(t)


void	
lt2_compose( lt2_type *t, lt2_type *s, lt2_type *ct )
{
	A( *ct ) = A(*s) * A(*t) + D(*s) * B(*t);
	D( *ct ) = A(*s) * D(*t) + D(*s) * E(*t);

	B( *ct ) = B(*s) * A(*t) + E(*s) * B(*t);
	E( *ct ) = B(*s) * D(*t) + E(*s) * E(*t);

	C( *ct ) = C(*s) * A(*t) + F(*s) * B(*t) + C(*t);
	F( *ct ) = C(*s) * D(*t) + F(*s) * E(*t) + F(*t);
}


void
lt2RS_inverse( lt2_type *lt, lt2_type *it )
{
float	det;

	det = lt->a0 * lt->b1 - lt->a0 * lt->b0;


	it->a0 =  lt->b1 / det;
	it->b0 = -lt->b0 / det;

	it->a1 = -lt->a1 / det;
	it->b1 =  lt->a0 / det;


	it->b0 = - ( it->a0 * lt->c0 + it->b0 * it->c1 );

	it->b1 = - ( it->a1 * lt->c0 + it->b1 * it->c1 );

}






void	
lt2_subtruct( lt2_type *lt0, lt2_type *lt1, lt2_type *lt )
{
int	i;

	for( i = 0 ; i < 3 ; i++ )
		lt->f1[i] =  lt1->f1[i] - lt0->f1[i];

	for( i = 0 ; i < 3 ; i++ )
		lt->f2[i] =  lt1->f2[i] - lt0->f2[i];
}

float	
lt2_dis( lt2_type *lt0, lt2_type *lt1 )
{
float	d,	md;
int	i;
	md = 0;

	for( i = 0 ; i < 3 ; i++ ){
		if( (d = lt1->f1[i] - lt0->f1[i]) < 0 )	d = -d;
		if( d > md )	md = d;
	}

	for( i = 0 ; i < 3 ; i++ ){
		if( (d = lt1->f2[i] - lt0->f2[i]) < 0 )	d = -d;
		if( d > md )	md = d;
	}

	return( d );
}




void 
lt2_max_diff( lt2_type *lt1, lt2_type *lt2, lt2_type *LT )
{
int	i;
float	a;

	for( i = 0 ; i < 3 ; i++ ){
		if( (a = lt1->f1[i] - lt2->f1[i]) < 0 )	a = -a;
		if( a > LT->f1[i] )	LT->f1[i] = a;

		if( (a = lt1->f2[i] - lt2->f2[i]) < 0 )	a = -a;
		if( a > LT->f2[i] )	LT->f2[i] = a;
	}
}


void
lt2_print( FILE *fp, lt2_type *lt )
{
	fprintf( stderr, "%.4f   %.4f   %.4f\n", lt->a0, lt->b0, lt->c0 );
	fprintf( stderr, "%.4f   %.4f   %.4f\n", lt->a1, lt->b1, lt->c1 );


}









lt2A_type *
lt2A_alloc( int ni, int nj )
{
lt2A_type *ltA;

	ltA = ( lt2A_type *)malloc( sizeof(lt2A_type ) );

	ltA->Ni = ni;
	ltA->Nj = nj;

	ltA->Nij = ltA->Ni *ltA->Nj;


	ltA->lt = ( lt2_type *)malloc( ltA->Nij * sizeof(lt2_type) );

	return( ltA );
}




void
lt2A_destroy( lt2A_type *ltA )
{
	if( ltA->lt != NULL )
		free( ltA->lt );

	free( ltA );
}


void
lt2A_set( lt2A_type *ltA, lt2_type *glt )
{
int	i,	j;
lt2_type *lt;
float	x,	y;


	lt = ltA->lt;
	for( i = 0, y = ltA->y0 ; i < ltA->Ni ; i++, y += ltA->dy )
		for( j = 0, x = ltA->x0 ; j < ltA->Nj ; j++, x += ltA->dx, lt++ ){

			lt2_shift( glt, x, y, lt );
	}
}

//void
//lt2A_set( lt2A_type *ltA, lt2_type *glt, float x0, float y0, float dx, float dy )
//{
//	int	i,	j;
//	lt2_type *lt;
//	float	x,	y;
//
//	ltA->x0 = x0;
//	ltA->y0 = y0;
//	ltA->dx = dx;
//	ltA->dy = dy;
//
//	lt = ltA->lt;
//	for( i = 0, y = y0 ; i < ltA->Ni ; i++, y += dy )
//		for( j = 0, x = x0 ; j < ltA->Nj ; j++, x += dx, lt++ ){
//
//			lt2_shift( glt, x, y, lt );
//		}
//}

void
lt2A_scale( lt2A_type *ltA, float f )
{
int	i,	j;
lt2_type *lt;

	ltA->x0 *= f;
	ltA->y0 *= f;
	ltA->dx *= f;
	ltA->dy *= f;


	lt = ltA->lt;
	for( i = 0 ; i < ltA->Ni ; i++ )
		for( j = 0 ; j < ltA->Nj ; j++, lt++ ){
			lt->c0 *= f;
			lt->c1 *= f;
		}
}


void
lt2A_unity( lt2A_type *ltA )
{
int	i,	j;
lt2_type *lt;
float	x,y	;

	lt = ltA->lt;
	for( i = 0, y = ltA->y0 ; i < ltA->Ni ; i++, y += ltA->dy )
		for( j = 0, x = ltA->x0 ; j < ltA->Nj ; j++, x += ltA->dy, lt++ ){
			lt2_unity( lt );
			lt->c0 = x;
			lt->c1 = y;
		}
}


void 
lt2A_value( lt2A_type *alt, float x, float y, float *u, float *v )
{
lt2_type	*lt;
int	i,	j;


	i = ( y - alt->y0 ) / alt->dy;
	j = ( x - alt->x0 ) / alt->dx;

	if( i < 0 )	i = 0;
	if( i >= alt->Ni ) i = alt->Ni -1;

	if( j < 0 )	j = 0;
	if( j >= alt->Nj ) j = alt->Nj -1;




	x = x - ( alt->x0 + j* alt->dx );
	y = y - ( alt->y0 + i* alt->dy );


	lt = alt->lt + i* alt->Nj + j;


	*u = LT2_F1( *lt, x, y );
	*v = LT2_F2( *lt, x, y );
}




static float	whight( float t, float dt, int n );


void 
lt2A_valueW( lt2A_type *alt, float x, float y, float *u, float *v )
{
lt2_type	*lt;
int	i,	j,	i0,	j0;
float	x0,	y0,	sw,	w;
int	ni,	nj;

	y0 = ( y - (alt->y0+0.5*alt->dy) ) / alt->dy;
	x0 = ( x - (alt->x0+0.5*alt->dx) ) / alt->dx;

	//i0 = ROUND( y0 );
	//j0 = ROUND( x0 );



	//ni = 2;
	//if( i0 < 0 ){
	//	i0 = 0;
	//	ni = 1;
	//}
	//else	
	//	if( i0 >= alt->Ni-1 ){
	//		i0 = alt->Ni-1;
	//		ni = 1;
	//	}

	//nj = 2;
	//if( j0 < 0 ){
	//	j0 = 0;
	//	nj = 1;
	//}
	//else	
	//	if( j0 >= alt->Nj-1 ){
	//		j0 = alt->Nj-1;
	//		nj = 1;
	//	}



	if( y0 < 0 ){
		i0 = 0;
		ni = 1;
	}
	else	
		if( y0 >= alt->Ni-1 ){
			i0 = alt->Ni-1;
			ni = 1;
		}
		else	{
			i0 = y0;
			ni = 2;
		}




	
	if( x0 < 0 ){
		j0 = 0;
		nj = 1;
	}
	else	
		if( x0 >= alt->Nj-1 ){
			j0 = alt->Nj-1;
			nj = 1;
		}
		else {
			j0 = x0;
			nj = 2;
		}


	*u = *v = sw = 0;
	for( i = i0 ; i < i0 + ni ; i++ ){
		for( j = j0 ; j < j0 + nj ; j++ ){

			lt = alt->lt + i* alt->Nj + j;

			x0 = x - ( alt->x0 + j* alt->dx );
			y0 = y - ( alt->y0 + i* alt->dy );

			w = whight( x0, alt->dx, nj ) * whight( y0, alt->dy, ni );

			*u += LT2_F1( *lt, x0, y0 ) * w;
			*v += LT2_F2( *lt, x0, y0 ) * w;

			sw += w;
		}
	}

	*u /= sw;
	*v /= sw;
}


static float
whight( float t, float dt, int n )
{
	float	h;
	if( n <= 1 )	return( 1 );

	h = dt / 2;

	t = t -h;
	if( t < 0 )	t = -t;


	t -= 0.5*h;
	if( t < 0 )	return( 1 );

	if( t > h )	return( 0 );

	return( (h-t)/h );

}



void 
lt2A_value_lt( lt2A_type *alt, float x, float y, lt2_type *tlt)
{
	lt2_type	*lt;
	int	i,	j;


	i = ( y - alt->y0 ) / alt->dy;
	j = ( x - alt->x0 ) / alt->dx;

	if( i < 0 )	i = 0;
	if( i >= alt->Ni ) i = alt->Ni -1;

	if( j < 0 )	j = 0;
	if( j >= alt->Nj ) j = alt->Nj -1;




	x = x - ( alt->x0 + j* alt->dx );
	y = y - ( alt->y0 + i* alt->dy );


	lt = alt->lt + i* alt->Nj + j;


	*tlt = *lt;
	tlt->c0 = LT2_F1( *lt, x, y );
	tlt->c1 = LT2_F2( *lt, x, y );
}


//    t = lt( s )
int
	lt2_affine( lt2_type *lt, vec2f_type s[3], vec2f_type t[3] )
{
	matrix3_type	m;
	vec3d_type		D,	X;

	m.a00 = s[0].x;
	m.a01 = s[0].y;
	m.a02 = 1;

	m.a10 = s[1].x;
	m.a11 = s[1].y;
	m.a12 = 1;


	m.a20 = s[2].x;
	m.a21 = s[2].y;
	m.a22 = 1;



	D.x = t[0].x;
	D.y = t[1].x;
	D.z = t[2].x;

	matrix3_solve( &m, &D, &X );
	lt->a0 = X.x;
	lt->b0 = X.y;
	lt->c0 = X.z;



	D.x = t[0].y;
	D.y = t[1].y;
	D.z = t[2].y;

	matrix3_solve( &m, &D, &X );
	lt->a1 = X.x;
	lt->b1 = X.y;
	lt->c1 = X.z;

//#define _TEST
#ifdef _TEST 
	float	x,	y;
	int	i;
	for( i = 0 ; i < 3 ; i++ ){
		x = LT2_F1( *lt, s[i].x, s[i].y );
		y = LT2_F2( *lt, s[i].x, s[i].y );

		x -= t[i].x;
		y -= t[i].y;
	}

#endif

	return( 1 );
}



//    t = lt( s )
int
lt2_similarityA( lt2_type *lt, vec2f_type ap[2], vec2f_type aq[2] )
{
	vec2f_type	p,	q;
	vec2f_type	pu,	qu;
	vec2f_type	w;
	matrix2_type	m,	m1,	m2;
	float	t;

	p.x = ap[1].x - ap[0].x;
	p.y = ap[1].y - ap[0].y;


	q.x = aq[1].x - aq[0].x;
	q.y = aq[1].y - aq[0].y;


	VEC2D_LEFT( p, pu );
	VEC2D_LEFT( q, qu );

	


	t = p.x*p.x + p.y*p.y;
	m1.a00 = p.x / t;
	m1.a01 = p.y / t;
	m1.a10 = pu.x / t;
	m1.a11 = pu.y / t;

	m2.a00 = q.x;
	m2.a01 = qu.x;
	m2.a10 = q.y;
	m2.a11 = qu.y;

	matrix2_mult( &m2, &m1, &m );

	lt->a0 = m.a00;
	lt->b0 = m.a01;
	lt->a1 = m.a10;
	lt->b1 = m.a11;


	matrix2_multV( &m, &ap[0], &w );
	lt->c0 = aq[0].x - w.x;
	lt->c1 = aq[0].y - w.y;


#ifdef _TEST_
	w.x = LT2_F1( *lt, ap[0].x, ap[0].y );
	w.y = LT2_F2( *lt, ap[0].x, ap[0].y );

	w.x -= aq[0].x;
	w.y -= aq[0].y;

	w.x = LT2_F1( *lt, ap[1].x, ap[1].y );
	w.y = LT2_F2( *lt, ap[1].x, ap[1].y );

	w.x -= aq[1].x;
	w.y -= aq[1].y;
#endif


	return( 1 );
}


//    t = lt( s )
int
lt2_asimilarity_eigen2d( lt2_type *lt,  eigen2d_type *ep,  eigen2d_type *eq )
{
int	ret;
float	t;
	vec2f_type ap[2],	aq[2];

	ap[0] = ep->p;
	ap[1].x = ep->p.x + ep->v1.x;
	ap[1].y = ep->p.y + ep->v1.y;

	t = sqrt(eq->e1 / ep->e1);
	aq[0] = eq->p;
	aq[1].x = eq->p.x + t*eq->v1.x;
	aq[1].y = eq->p.y + t*eq->v1.y;

	ret = lt2_similarityA( lt, ap, aq );

	return( ret );
}



void
lt2_swap( lt2_type *lt, lt2_type *sw )
{
	sw->a0 = lt->b1;
	sw->b0 = lt->a1;
	sw->c0 = lt->c1;

	sw->a1 = lt->b0;
	sw->b1 = lt->a0;
	sw->c1 = lt->c0;

}

