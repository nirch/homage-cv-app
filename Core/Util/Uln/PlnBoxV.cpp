// WndInner.cpp : implementation file
//

#include <stdio.h>
#include <math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"


#include "PlnType.h"

static int	plnA_boxV_update( pln_type *pl, float dt, vec2f_type *p0, vec2f_type *v, box2f_type *b );




int
plnA_boxV( plnA_type *apl, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{
int	i;

	
	b->x0 = b->x1 = b->y0 = b->y1 = 0;

	for( i = 0; i < apl->nA ; i++ )
		plnA_boxV_update( apl->a[i], 1.0, p0, v, b );

	
		

	return( 1 );
}



int
pln_boxV( pln_type *pl, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{

	b->x0 = b->x1 = b->y0 = b->y1 = 0;

	plnA_boxV_update( pl, 1.0, p0, v, b );

	return( 1 );
}


static int
plnA_boxV_update( pln_type *pl, float dt, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{

	vec2f_type	p;
	int	n;
	float	t;




	vec2f_type	bu;
	VEC2D_LEFT( *v, bu );
	

	for( t = 0, n = 0 ; t < pl->len ; t += dt ){

		pln_gt2p( pl, t, &p );

		vec2f_type	dp;
		dp.x  = p.x - p0->x;
		dp.y  = p.y - p0->y;

		float x = VEC2D_INNER( *v, dp );
		float y = VEC2D_INNER( bu, dp );

		if( ABS(x) > 25 )	y = 0;


		BOX2D_UPDATE( *b, x, y );
	

		n++;
	}




	return( n );
}





static int	plnA_boxV_updateV( pln_type *pl, float dt, vec2f_type *p0, vec2f_type *v, box2f_type *b );

int
	plnA_boxVV( plnA_type *apl, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{
	int	i;


	b->x0 = b->x1 = b->y0 = b->y1 = 0;

	vec2f_type dp;
	dp.x = apl->a[0]->ctr.x - p0->x;
	dp.y = apl->a[0]->ctr.y - p0->y;


	vec2f_type	bu;
	VEC2D_LEFT( *v, bu );

	float x = VEC2D_INNER( *v, dp );
	float y = VEC2D_INNER( bu, dp );
	b->x0 = b->x1 = x;
	b->y0 = b->y1 = y;


	for( i = 0; i < apl->nA ; i++ )
		plnA_boxV_updateV( apl->a[i], 1.0, p0, v, b );




	return( 1 );
}



int
	pln_boxVV( pln_type *pl, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{

	b->x0 = b->x1 = b->y0 = b->y1 = 0;

	plnA_boxV_updateV( pl, 1.0, p0, v, b );

	return( 1 );
}


static int
	plnA_boxV_updateV( pln_type *pl, float dt, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{

	vec2f_type	p;
	int	n;
	float	t;




	vec2f_type	bu;
	VEC2D_LEFT( *v, bu );


	for( t = 0, n = 0 ; t < pl->len ; t += dt ){

		pln_gt2p( pl, t, &p );

		vec2f_type	dp;
		dp.x  = p.x - p0->x;
		dp.y  = p.y - p0->y;

		float x = VEC2D_INNER( *v, dp );
		float y = VEC2D_INNER( bu, dp );


		BOX2D_UPDATE( *b, x, y );


		n++;
	}




	return( n );
}



static int	plnA_boxVx_in( pln_type *pl, vl2f_type *vl, float dy );


#ifdef _AA_
int
plnA_copy_boxV( plnA_type *apl, float d, vec2f_type *p0, vec2f_type *v, float dx, float dy, int fCopy, plnA_type *tapl )
{
	int	i;


	vl2f_type vl;
	vl.p = *p0;
	vl.v = *v;
	vl.d = dx;

	box2f_type box,	b;

	vl2f_boxE( &vl, -dy, dy, &b );

	for( i = 0; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];

		pln_box( pl, &box );

		if( box2f_distance( &box, &b ) > 0 )
			continue;



		if( plnA_boxVx_in( pl, &vl, dy ) <= 0 )
			continue;

		if( fCopy == 1 )
			pl = pln_copy( pl );

		tapl->a[tapl->nA++] = pl;
	}




	return( 1 );
}
#endif



int
	plnA_copy_boxV( plnA_type *apl, float d, vl2f_type *vl, float dy, int fCopy, plnA_type *tapl )
{
	int	i;

	box2f_type box,	b;

	vl2f_boxE( vl, -dy, dy, &b );

	for( i = 0; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];

		pln_box( pl, &box );

		if( box2f_distance( &box, &b ) > 0 )
			continue;



		if( plnA_boxVx_in( pl, vl, dy ) <= 0 )
			continue;

		if( fCopy == 1 )
			pl = pln_copy( pl );

		tapl->a[tapl->nA++] = pl;
	}




	return( 1 );
}



static int
plnA_boxVx_in( pln_type *pl, vl2f_type *vl, float dy )
{

	vec2f_type	p;
	int	n;
	float	t;




	vec2f_type	bu;
	VEC2D_LEFT( vl->v, bu );


	for( t = 0, n = 0 ; t < pl->len ; t += 1.0 ){

		pln_gt2p( pl, t, &p );

		vec2f_type	dp;
		dp.x  = p.x - vl->p.x;
		dp.y  = p.y - vl->p.y;

		float x = VEC2D_INNER( vl->v, dp );

		if( ABS(x) > vl->d )
			continue;


		float y = VEC2D_INNER( bu, dp );

		if(  ABS(y) - dy > 2 )	continue;


		n++;
	}




	return( n );
}





static int	plnA_boxVx_update( pln_type *pl, float d, vec2f_type *p0, vec2f_type *v, box2f_type *b );


int
	plnA_boxVx( plnA_type *apl, float d, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{
	int	i;



	b->x0 = b->x1 = b->y0 = b->y1 = 0;

	for( i = 0; i < apl->nA ; i++ ){
		plnA_boxVx_update( apl->a[i], d, p0, v, b );

	}




	return( 1 );
}

static int
	plnA_boxVx_update( pln_type *pl, float d, vec2f_type *p0, vec2f_type *v, box2f_type *b )
{

	vec2f_type	p;
	int	n;
	float	t;




	vec2f_type	bu;
	VEC2D_LEFT( *v, bu );


	for( t = 0, n = 0 ; t < pl->len ; t += 1.0 ){

		pln_gt2p( pl, t, &p );

		vec2f_type	dp;
		dp.x  = p.x - p0->x;
		dp.y  = p.y - p0->y;

		float x = VEC2D_INNER( *v, dp );
		float y = VEC2D_INNER( bu, dp );

		float dy = ABS(y) - d;
		if( dy > 2 )	continue;


		BOX2D_UPDATE( *b, x, y );


		n++;
	}




	return( n );
}