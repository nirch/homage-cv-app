/******************
 ***  ptLine.c  ***
 ******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#include "Pt2dType.h"



pt2dA_type *
pt2dA_get_pt( pt2dA_type *apt, int i0, pt2dA_type *capt )
{
pt2d_type	*pt;//,	*npt;
int	i,	prev,	tmp;

	if( capt == NULL )
		capt = pt2dA_alloc( apt->nP );



	prev = pt2dA_get_prev(apt, i0 );


	capt->nP = 0;
	for( i = i0 ; ; i ){
		pt = &apt->p[i];

		if( pt->id != 2 && pt->id != 3 )
			break;

		capt->p[capt->nP] = *pt;
		capt->p[capt->nP].id = i;
		capt->nP++;

		pt->group = 1;

		if( pt->id == 3 && pt != &apt->p[i0] )
			break;


		tmp = i;
		i = ( pt->i[0] == prev )? pt->i[1] : pt->i[0];
		prev = tmp;

	}

	return( capt );
}




int
pt2dA_get_prev( pt2dA_type *apt, int i0 )
{
pt2d_type	*pt,	*npt;
int	i;

	pt = &apt->p[i0];


	if( (i = pt->i[0]) < 0 )
		return( pt->i[0] );

	npt = &apt->p[i];
	if( npt->id == 2 && (npt->i[0] == i0 || npt->i[1] == i0) )
		return( pt->i[1] );

	return( pt->i[0] );
}



int
pt2dA_get_next( pt2dA_type *apt, int i0 )
{
	pt2d_type	*pt,	*pt0;
	int	i;


	pt0 = &apt->p[i0];
	if( pt0->id != 3 )
		return(0);


	for( i = 0 ; i < 2 ; i++ ){
		int si = pt0->i[i];
		if( si < 0 )	continue;

		pt = &apt->p[si];
		if( (pt->i[0] != i0  &&  pt->i[1] != i0) || pt->id != 2 )
			return( i );
	}


	return( -1 );
}



int
pt2dA_get_nextN( pt2dA_type *apt, int i0, int prev )
{
	pt2d_type	*pt0;

	pt0 = &apt->p[i0];


	if( pt0->i[0] == prev )	return( pt0->i[1] );
	if( pt0->i[1] = prev )	return( pt0->i[0] );

	return( -2 );
}





int
pt2dA_get_ptN( pt2dA_type *apt, int i0, int i1, int nP, pt2dA_type *capt )
{
	pt2d_type	*pt;
	int	i,	prev,	tmp;


	capt->nP = 0;
	capt->p[capt->nP] = apt->p[i0];
	capt->p[capt->nP].id = i0;
	capt->nP++;


	prev = i0;

	for( i = i1 ; capt->nP < nP ; i ){
		pt = &apt->p[i];

		if( pt->id != 2 && pt->id != 3 )
			break;

		capt->p[capt->nP] = *pt;
		capt->p[capt->nP].id = i;
		capt->nP++;

		pt->group = 1;

		if( pt->id == 3 && pt != &apt->p[i1] )
			break;


		tmp = i;
		i = ( pt->i[0] == prev )? pt->i[1] : pt->i[0];
		prev = tmp;

	}

	return( capt->nP );
}





