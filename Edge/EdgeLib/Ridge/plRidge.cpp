/*************************
 ***   DtVehicle.cpp   ***
 *************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>




//#include "Wutil/UWgpw/GpwType.h"



#include "PlRidgeType.h"









plRidge_type * plRidge_alloc()
{
	plRidge_type *r =( plRidge_type *)malloc( sizeof(plRidge_type));

	r->pl = NULL;

	return( r );
}


void plRidge_destroy( plRidge_type *r )
{

	if( r->pl != NULL )
		pln_destroy( r->pl );

	free( r );
}


void plRidge_draw( gpw_type *gpw, plRidge_type *r )
{
//	int	color;
//
//	color = ( r->lm > 0 )? BLUE : GREEN;
//
//
//
//	if( r->pl != NULL )
////		pln_draw( gpw, r->pl, color, color, 0x01);
//		gpw_pln( gpw, r->pl, color, color, 0x01);

}


void
plRidge_translate( plRidge_type *c, vec2f_type *ctr )
{
	c->p.x += ctr->x;
	c->p.y += ctr->y;

	c->pl->ctr.x += ctr->y;
	c->pl->ctr.y += ctr->x;
}

void
plRidgeA_translate( plRidgeA_type *ar, float x, float y )
{
	for( int i = 0 ; i < ar->nR ; i++ ){

		plRidge_type *r = ar->r[i];
		r->pl->ctr.x += x;
		r->pl->ctr.y += y;
	}
}


plRidgeA_type * plRidgeA_alloc( int n )
{
	plRidgeA_type *ar =( plRidgeA_type *)malloc( sizeof(plRidgeA_type));

	ar->NR = n;

	ar->r =( plRidge_type **)malloc( ar->NR * sizeof(plRidge_type *));

	ar->nR = 0;

	return( ar );
}

plRidgeA_type * plRidgeA_realloc( plRidgeA_type *ar, int n )
{
	if( ar == NULL ){
		ar = plRidgeA_alloc( n );
		return( ar );
	}

	if( ar->NR >= n )
		return( ar );


	ar->NR = n;

	ar->r =( plRidge_type **)realloc( ar->r, ar->NR * sizeof(plRidge_type *));

	return( ar );
}


void
plRidgeA_add( plRidgeA_type *ar, plRidge_type *pl )
{
	if( ar->nR >= ar->NR ){
		ar->NR += 100;
		ar->r = ( plRidge_type **)realloc( ar->r, ar->NR*sizeof(plRidge_type *) );
	}

	ar->r[ar->nR++] = pl;
}

void plRidgeA_destroy( plRidgeA_type *ar )
{
int	i;

	for( i = 0 ; i < ar->nR ; i++ )
		plRidge_destroy( ar->r[i] );

	free( ar->r );

	free( ar );
}


void
	plRidgeA_decrease( plRidgeA_type *ar )
{
	int	i,	j;

	for( i = 0, j = 0 ; i < ar->nR ; i++ ){
		plRidge_type	*r = ar->r[i];
		if( r->pl == NULL ){
			plRidge_destroy( r );
			continue;
		}

		ar->r[j++] = r;
	}

	ar->nR = j;
}


int
plRidgeA_select( plRidgeA_type *ar, vec2f_type *p, float D )
{
int	i;
float	uMin;
dPln_type	d;
int	iMin;

	iMin = -1;
	uMin = 2*D;
	for( i = 0 ; i < ar->nR ; i++ ){
		plRidge_type	*r = ar->r[i];

		pln_distance( r->pl, p, &d );
		if( d.gt < 0 || d.gt > r->pl->len )
			continue;

		if( ABS(d.u) < ABS(uMin) ){
			uMin = d.u;
			iMin = i;
		}
	}

	return( iMin );
}


plnA_type *
plRidgeA_to_plnA( plRidgeA_type *ar, float Len, int fCopy )
{
	plnA_type *apl;
	int	i;

	apl = plnA_alloc( ar->nR );

	for( i = 0 ; i < ar->nR ; i++ ){
		pln_type *pl =  ar->r[i]->pl;
		if( Len > 0 && pl->len < Len ) 
			continue;

		if( fCopy == 1 )
			pl = pln_copy( pl );

		apl->a[apl->nA++] = pl;
	}

	return( apl );
}
