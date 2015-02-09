/*******************
 ***  PlnTool.c  ***
 *******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"


#include "Uvl/Vl2fType.h"
#include "Uln/PlnType.h"



plnF_type *	
plnF_alloc( int n )
{
plnF_type	*vpl;
int	i;

	vpl = ( plnF_type *)malloc( sizeof( plnF_type) );
	
	vpl->NA = n;
	
	vpl->a = ( plnA_type **)malloc( vpl->NA * sizeof( plnA_type*) );

	vpl->nA = 0;

	for( i = 0 ; i <  vpl->NA ; i++ )
		vpl->a[i] = NULL;
	
	return( vpl );
}


void 
plnF_destroy( plnF_type *vpl )
{
int	i;


	for( i = 0 ; i < vpl->nA ; i++ ){
		if( vpl->a[i] != NULL ){
			plnA_destroy( vpl->a[i] );
			vpl->a[i] = NULL;
		}
	}


	free( vpl->a );

	free( vpl );
}

plnA_type *
plnA_from_pl( pln_type *pl )
{
	plnA_type *apl = plnA_alloc( 2 );
	apl->a[apl->nA++] = pl;

	return( apl );
}

plnA_type *
plnF_get( plnF_type *vpl, int iFrame )
{
	if( iFrame < 0 || iFrame >= vpl->nA )
		return( NULL );

	return( vpl->a[iFrame] );
}



void
plnF_add( plnF_type *vpl, plnA_type *apl, int iFrame )
{
int	i;

	if( iFrame >= vpl->NA ){
		
		int NA = vpl->NA + iFrame+32;
		vpl->a = ( plnA_type **)realloc( vpl->a, NA*sizeof(plnA_type *) );

		for( i = vpl->NA ; i <  NA ; i++ )
			vpl->a[i] = NULL;

		vpl->NA = NA;
	}

	if( vpl->nA > iFrame && vpl->a[iFrame] != NULL )
		plnA_destroy( vpl->a[iFrame] );

	vpl->a[iFrame] = apl;
	apl->iFrame = iFrame;


	if( vpl->nA <= iFrame )
		vpl->nA = iFrame+1;

}



void
plnF_destroy_empty( plnF_type *fpl )
{
int	i;

	for( i = 0 ; i <  fpl->nA ; i++ ){
		if( fpl->a[i] == NULL )	continue;
		if( fpl->a[i]->nA == 0 ){
			plnA_destroy( fpl->a[i] );
			fpl->a[i] = NULL;
		}
	}
}


void
plnF_clear( plnF_type *vpl, int iFrame )
{

	if( iFrame < vpl->nA && vpl->a[iFrame] != NULL ){
		plnA_destroy( vpl->a[iFrame] );

		vpl->a[iFrame] = NULL;
	}
}