/*************************
 ***   DtVehicle.cpp   ***
 *************************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>

#include "pHunchType.h"


pHunch_type *
pHunch_alloc()
{
	pHunch_type	*h;

	h = (pHunch_type *)malloc( sizeof( pHunch_type) );

	return( h );
}


void
pHunch_destroy( pHunch_type *h )
{
	free( h );
}

pHunchA_type *
pHunchA_alloc( int nH )
{
pHunchA_type	*ah;

	ah = (pHunchA_type *)malloc( sizeof( pHunchA_type) );

	ah->NA = nH;

	ah->a = ( pHunch_type **)malloc( ah->NA*sizeof(pHunch_type*) );

	ah->nA = 0;

	return( ah );
}



void
pHunchA_destroy( pHunchA_type *ah )
{
int	i;

	for( i = 0 ; i < ah->nA ; i++ )
		free( ah->a[i] );

	free( ah->a );

	free( ah );
}


void
pHunchA_clear( pHunchA_type *ah )
{
	int	i;

	for( i = 0 ; i < ah->nA ; i++ )
		free( ah->a[i] );

	ah->nA = 0;
}


int
pHunchA_translate( pHunchA_type *ah, float x, float y )
{
	int	i;

	for( i = 0 ; i < ah->nA ; i++ ){
		pHunch_type	*h = ah->a[i];

		h->p.x = x + h->p.x;
		h->p.y = y + h->p.y;
	}
	return( 1 );
}


int
pHunchA_swap( pHunchA_type *ah )
{
	int	i;

	for( i = 0 ; i < ah->nA ; i++ ){
		pHunch_type	*h = ah->a[i];

		VEC2D_SWAP( h->p );
	}
	return( 1 );
}



int
pHunchA_select( pHunchA_type *ah, vec2f_type *p, float D )
{
	int	i;
	vec2f_type	dv;
	float	u,	uMin;
	int	iMin;

	iMin = -1;
	uMin = D*D;
	for( i = 0 ; i < ah->nA ; i++ ){
		pHunch_type	*h = ah->a[i];


		dv.x = h->p.x - p->x;
		dv.y = h->p.y - p->y;

		u = dv.x*dv.x + dv.y*dv.y;

		if( ABS(u) < ABS(uMin) ){
			uMin = u;
			iMin = i;
		}
	}


	return( iMin );
}