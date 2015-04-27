/*****************
	RegAgpLinec
******************/

#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include "Ucamera/Pt2dType.h"

#include "Uln/PlnType.h"



#include "Vl2fType.h"


vl2fA_type *
vl2fA_alloc( int n )
{
	vl2fA_type	*avl;


	avl = (vl2fA_type *)malloc( sizeof( vl2fA_type) );

	avl->NA = n;
	avl->a = ( vl2f_type *)malloc( avl->NA*sizeof(vl2f_type) );

	avl->nA = 0;



	return( avl );
}


vl2fA_type *
vl2fA_realloc( vl2fA_type *avl, int n )
{
	if( avl == NULL ){
		avl = vl2fA_alloc(n );
		return( avl );
	}


	if( avl->NA < n ){
		avl->a = ( vl2f_type *)realloc( avl->a, n*sizeof(vl2f_type) );
		avl->NA = n;
	}

	return( avl );
}



void
vl2fA_destroy( vl2fA_type *avl )
{
	free( avl->a );

	free( avl );
}

void
vl2f_swap( vl2f_type *vl )
{
	VEC2D_SWAP( vl->p );
	VEC2D_SWAP( vl->v );
}



void 
vl2f_set( vl2f_type *vl, vec2f_type *p0, vec2f_type *p1 )
{

	vl->p.x = 0.5* (p1->x + p0->x );
	vl->p.y = 0.5* (p1->y + p0->y );

	vl->v.x = p1->x - p0->x;
	vl->v.y = p1->y - p0->y;

	vl->d = hypot( vl->v.x, vl->v.y );
	vl->v.x /= vl->d;
	vl->v.y /= vl->d;

	vl->d *= 0.5;
}



void 
vl2f_points( vl2f_type *vl, vec2f_type *p0, vec2f_type *p1 )
{

	p0->x = vl->p.x - vl->d*vl->v.x;
	p0->y = vl->p.y - vl->d*vl->v.y;

	p1->x = vl->p.x + vl->d*vl->v.x;
	p1->y = vl->p.y + vl->d*vl->v.y;
}


float
vl2f_distance( vl2f_type *vl, vec2f_type *p )
{
	float	t;
	t = ( p->x - vl->p.x )* -vl->v.y + ( p->y - vl->p.y )* vl->v.x;

	return( t );
}


float
vl2f_distance2( vl2f_type *vl, vec2f_type *p )
{
	float	t,	u;
	u = ( p->x - vl->p.x )* -vl->v.y + ( p->y - vl->p.y )* vl->v.x;
	if( u < 0 )	u = -u;

	t = ( p->x - vl->p.x )* vl->v.x + ( p->y - vl->p.y )* vl->v.y;

	if( t < -vl->d ){
		u+= -vl->d -t;
	}
	if( t > vl->d ){
		u+= t -vl->d;
	}



	return( u );
}



void
vl2fA_set_group( vl2fA_type *avl, int iGroup )
{
	int	i;

	for( i = 0; i < avl->nA ; i++ )
		avl->a[i].group = -1;
}


int
vl2fA_select( vl2fA_type *avl, vec2f_type *p, float D )
{
	int	i;
	float	d,	dMin;
	int	iMin;

	dMin = 2*D;
	iMin = -1;
	for( i = 0; i < avl->nA ; i++ ){
		d = vl2f_distance2( &avl->a[i], p );

		if( d < dMin ){
			iMin = i;
			dMin = d;
		}

	}

	if( dMin > D )	
		return( -1 );

	return( iMin );
}


pt2dA_type *
vl2fA_to_pt2dA( vl2fA_type *avl )
{
	pt2dA_type	*apt;
	int	i;

	apt = pt2dA_alloc( avl->nA );
	apt->axis = PT2D_AXIS_YX;
	apt->type = PT2D_4V;

	for( i = 0 ; i < avl->nA ; i++ ){
		vl2f_type *vl = &avl->a[i];
		pt2d_type *pt = &apt->p[apt->nP++];

		pt->p = vl->p;
		pt->n = vl->v;
		pt->r = vl->d;

		pt->group = vl->group;
	}

	return( apt );
}


vl2fA_type *
vl2fA_from_pt2dA( pt2dA_type *apt )
{
vl2fA_type	*avl;
//vl2f_type	*vl;

	int	i;


	avl = vl2fA_alloc( apt->nP );


	
	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		vl2f_type	*vl = &avl->a[avl->nA++];

		vl->p = pt->p;
		vl->v = pt->n;
		vl->d = pt->r;

		VEC2D_NORMALIZE( vl->v);

	}

	return( avl );
}



int
vl2fA_dump( vl2fA_type *avl, char *prefix, int index, char *suffix )
{
plnA_type *apl;

	if( avl->nA == 0 )
		return( -1 );

	apl = plnA_from_vlA( avl, NULL );

	PLNA_DUMP( apl, prefix, index, suffix );

	plnA_destroy( apl );

	return( 1 );
}



int
vl2fA_read( vl2fA_type **avl, char *file )
{
	pt2dA_type	*apt;


	if( pt2dA_read_from_file( &apt, file ) < 0 )
		return( -1 );

	*avl = vl2fA_from_pt2dA( apt );

	pt2dA_destroy( apt );

	return( 1 );
}


int
vl2fA_write( vl2fA_type *avl, char *file )
{
	pt2dA_type	*apt;


	apt = vl2fA_to_pt2dA( avl );

	pt2dA_write_to_file( apt, file );


	pt2dA_destroy( apt );

	return( 1 );
}





void
vl2fA_reorder( vl2fA_type *avl, pt2dA_type *apt, float dis )
{
	int i,	k;
	vec2f_type p;
	vl2f_type tmp;

	if( apt == NULL )
		return;


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		p = pt->p;
		if( apt->axis == PT2D_AXIS_XY )
			VEC2D_SWAP( p );

		k = vl2fA_select( avl, &p, dis );
		if( k < 0 )	continue;

		tmp = avl->a[i];
		avl->a[i] = avl->a[k];
		avl->a[k] = tmp;
	}
}



int	vl2f_middle( vl2f_type *vl0, vl2f_type *vl1, vl2f_type *vl )
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
vl2f_cutpoint( vl2f_type *vl0, vl2f_type *vl1, vec2f_type *p )
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

	t = vl2f_distance( vl1, p );
#endif
}



void
vl2fA_inverse_direction( vl2fA_type *avl )
{
	int i;

	for( i = 0 ; i < avl->nA ; i++ )
		VEC2D_MINUS( avl->a[i].v );
}
