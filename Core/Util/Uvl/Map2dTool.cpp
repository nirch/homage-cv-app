/*****************
	RegAgpLinec
******************/

#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"
#include	"Ucamera/Pt2dType.h"
#include	"Umath/Matrix4Type.h"
#include	"Umath/Matrix3Type.h"


#include "Map2dType.h"


map2dA_type *
map2dA_alloc( int n )
{
	map2dA_type	*am;


	am = (map2dA_type *)malloc( sizeof( map2dA_type) );

	am->NA = n;
	am->a = ( map2d_type *)malloc( am->NA*sizeof(map2d_type) );

	am->nA = 0;



	return( am );
}


map2dA_type *
map2dA_realloc( map2dA_type *am, int n )
{
	if( am == NULL ){
		am = map2dA_alloc(n );
		return( am );
	}


	if( am->NA < n ){
		am->a = ( map2d_type *)realloc( am->a, n*sizeof(map2d_type) );
		am->NA = n;
	}

	return( am );
}



void
map2dA_destroy( map2dA_type *am )
{
	free( am->a );

	free( am );
}



void
map2d_set_lt2(map2d_type *m, lt2_type *lt, double x, double y )
{
	m->sp.x = x;
	m->sp.y = y;

	m->tp.x = LT2_F1( *lt, m->sp.x, m->sp.y );
	m->tp.y = LT2_F2( *lt, m->sp.x, m->sp.y );
	m->use = 1;
}


pt2dA_type *
map2dA_to_pt2dA( map2dA_type *am )
{
	pt2dA_type	*apt;
	int	i;

	apt = pt2dA_alloc( 2*am->nA );
	apt->axis = PT2D_AXIS_YX;
	apt->type = PT2D_2;

	for( i = 0 ; i < am->nA ; i++ ){
		map2d_type *m = &am->a[i];

		pt2d_type *pt = &apt->p[apt->nP++];
		pt->p.x = m->sp.x;
		pt->p.y = m->sp.y;
		pt->group = 1;

		pt = &apt->p[apt->nP++];
		pt->p.x = m->tp.x;
		pt->p.y = m->tp.y;
		pt->group = 2;
	}

	return( apt );
}


map2dA_type *
	map2dA_from_pt2dA( pt2dA_type *apt )
{
	map2dA_type	*am;
	int	i;


	am = map2dA_alloc( apt->nA/2 );

	for( i = 0 ; i+1 < apt->nA ; i += 2 ){


		map2d_type *m = &am->a[am->nA++];

		pt2d_type *pt = &apt->p[i];

		m->sp.x = pt->p.x;
		m->sp.y = pt->p.y;



		pt = &apt->p[i+1];
		m->tp.x = pt->p.x;
		m->tp.y = pt->p.y;

		m->use = 1;
	}

	return( am );
}




int
map2dA_dump( map2dA_type *am, char *prefix, int index, char *suffix )
{
pt2dA_type	*apt;


	apt = map2dA_to_pt2dA( am );

	PT2DA_DUMP( apt, prefix, index, suffix );

	pt2dA_destroy( apt );

	return( 1 );

}


int
map2dA_similarity_lt2( map2dA_type *am, lt2_type *lt )
{
	int	i;

	matrix4_type m;
	double	D[4],	X[4];


	matrix4_zero( &m );
	D[0] = D[1] = D[2] = D[3] = 0;

	for( i = 0 ; i < am->nA ; i++ ){
		map2d_type *map = &am->a[i];
		if( map->use == 0 )	continue;

		m.a00 += map->sp.x * map->sp.x  + map->sp.y * map->sp.y;
		m.a01 += map->sp.y * map->sp.x  -  map->sp.x * map->sp.y;
		m.a02 +=             map->sp.x;
		m.a03 +=									   map->sp.y;
		D[0]  += map->tp.x * map->sp.x  +  map->tp.y * map->sp.y;



		m.a10 += map->sp.x * map->sp.y  -  map->sp.y * map->sp.x;
		m.a11 += map->sp.y * map->sp.y  +  map->sp.x * map->sp.x;
		m.a12 +=             map->sp.y;
		m.a13 +=									  - map->sp.x;
		D[1]  += map->tp.x * map->sp.y  -  map->tp.y * map->sp.x;


		m.a20 += map->sp.x;
		m.a21 += map->sp.y;
		m.a22 += 1;        
		m.a23 += 0;		
		D[2]  += map->tp.x;

		m.a30 += map->sp.y;
		m.a31 += -map->sp.x;
		m.a32 += 0;        
		m.a33 += 1;		
		D[3]  += map->tp.y;
	}


	matrix4_solve( &m, D, X );

	lt->a0 = X[0];
	lt->b0 = X[1];
	lt->a1 = -X[1];
	lt->b1 = X[0];

	lt->c0 = X[2];
	lt->c1 = X[3];



	return( 1 );
}



int
map2dA_affine_lt2( map2dA_type *am, lt2_type *lt )
{
	matrix3_type m;
	vec3d_type		Dx,	Dy, X;
	int	i;


	matrix3_zero( &m );


	Dx.x = Dx.y = Dx.z = 0;
	Dy.x = Dy.y = Dy.z = 0;


	for( i = 0 ; i < am->nA ; i++ ){
		map2d_type *map = &am->a[i];
		if( map->use == 0 )	continue;

		m.a00 += map->sp.x * map->sp.x;
		m.a01 += map->sp.y * map->sp.x;
		m.a02 +=             map->sp.x;
		Dx.x   += map->tp.x * map->sp.x;

		m.a10 += map->sp.x * map->sp.y;
		m.a11 += map->sp.y * map->sp.y;
		m.a12 +=             map->sp.y;
		Dx.y   += map->tp.x * map->sp.y;

		m.a20 += map->sp.x;
		m.a21 += map->sp.y;
		m.a22 += 1;
		Dx.z   += map->tp.x;




		Dy.x   += map->tp.y * map->sp.x;
		Dy.y   += map->tp.y * map->sp.y;
		Dy.z   += map->tp.y;


	}


	matrix3_solve( &m, &Dx, &X );

	lt->a0 = X.x;
	lt->b0 = X.y;
	lt->c0 = X.z;
	

	matrix3_solve( &m, &Dy, &X );

	lt->a1 = X.x;
	lt->b1 = X.y;
	lt->c1 = X.z;


	return( 1 );
}


double
map2dA_measure_lt2( map2dA_type *am, lt2_type *lt )
{
	int	i;
	vec2d_type	p;

	double sum = 0;
	int	n = 0;
	for( i = 0 ; i < am->nA ; i++ ){
		map2d_type *map = &am->a[i];
		if( map->use == 0 )	continue;

		p.x = LT2_F1( *lt, map->sp.x, map->sp.y );
		p.y = LT2_F2( *lt, map->sp.x, map->sp.y );

		p.x -= map->tp.x;
		p.y -= map->tp.y;

		double t = p.x*p.x + p.y*p.y;

		sum += t;
		n++;
	}

	sum /= n;

	return( sum );
}








#ifdef _AA_
int
map2dA_read( map2dA_type **am, char *file )
{
	pt2dA_type	*apt;


	if( pt2dA_read_from_file( &apt, file ) < 0 )
		return( -1 );

	*am = map2dA_from_pt2dA( apt );

	pt2dA_destroy( apt );

	return( 1 );
}


int
map2dA_write( map2dA_type *am, char *file )
{
	pt2dA_type	*apt;


	apt = map2dA_to_pt2dA( am );

	pt2dA_write_to_file( apt, file );


	pt2dA_destroy( apt );

	return( 1 );
}





void
map2dA_reorder( map2dA_type *am, pt2dA_type *apt, float dis )
{
	int i,	k;
	vec2f_type p;
	map2d_type tmp;

	if( apt == NULL )
		return;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		p = pt->p;
		if( apt->axis == PT2D_AXIS_XY )
			VEC2D_SWAP( p );

		k = map2dA_select( am, &p, dis );
		if( k < 0 )	continue;

		tmp = am->a[i];
		am->a[i] = am->a[k];
		am->a[k] = tmp;
	}
}



int	map2d_middle( map2d_type *vl0, map2d_type *vl1, map2d_type *vl )
{
	float	t,	d,	d0,	d1;
	vec2f_type	u,	u1;
	u.x  = -vl0->v.y;
	u.y =  vl0->v.x;

	u1.x  = -vl1->v.y;
	u1.y =  vl1->v.x;



	d = ( vl1->p.x - vl0->p.x )*u.x + (vl1->p.y - vl0->p.y) *u.y;
	t = ( vl1->p.x - vl0->p.x )*vl0->v.x + (vl1->p.y - vl0->p.y) *vl0->v.y;

	vl->v.x = vl0->v.x - vl1->v.x;
	vl->v.y = vl0->v.y - vl1->v.y;
	VEC2D_NORMALIZE( vl->v );

	d0 = t - vl1->d;
	if( d0 > -vl0->d )	
		d0 = -vl0->d;

	d1 = t + vl1->d;
	if( d1 < vl0->d )
		d1 = vl0->d;


	d = ( ( vl0->p.x - vl1->p.x )*u1.x + (vl0->p.y - vl1->p.y)*u1.y )/ ( 1 - u.x*u1.x - u.y*u1.y );
	vl->p.x = vl0->p.x + d*u.x;
	vl->p.y = vl0->p.y + d*u.y;


	vl->d = 0.5*(d1 - d0 );
	t = 0.5*(d1 + d0 );

	vl->p.x += t*vl->v.x;
	vl->p.y += t*vl->v.y;

	return( 1 );
}



void
map2d_cutpoint( map2d_type *vl0, map2d_type *vl1, vec2f_type *p )
{
	vec2f_type	n1,	dp;
	float	t;

	dp.x = vl1->p.x - vl0->p.x;
	dp.y = vl1->p.y - vl0->p.y;

	n1.x = vl1->v.y;
	n1.y = -vl1->v.x;

	t = VEC2D_INNER( dp, n1 ) / VEC2D_INNER( vl0->v, n1 );

	p->x = vl0->p.x + t *vl0->v.x;
	p->y = vl0->p.y + t *vl0->v.y;


#define _TEST
#ifdef _TEST

	t = map2d_distance( vl1, p );
#endif
}

#endif