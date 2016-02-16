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
plnF_type	*fpl;
int	i;

	fpl = ( plnF_type *)malloc( sizeof( plnF_type) );
	
	fpl->NA = n;
	
	fpl->a = ( plnA_type **)malloc( fpl->NA * sizeof( plnA_type*) );

	fpl->nA = 0;

	fpl->frame0 = 0;

	for( i = 0 ; i <  fpl->NA ; i++ )
		fpl->a[i] = NULL;
	
	return( fpl );
}


void 
plnF_destroy( plnF_type *fpl )
{
int	i;

	for( i = 0 ; i < fpl->nA ; i++ ){
		if( fpl->a[i] != NULL ){
			plnA_destroy( fpl->a[i] );
			fpl->a[i] = NULL;
		}
	}


	free( fpl->a );

	free( fpl );
}

plnA_type *
plnA_from_pl( pln_type *pl )
{
	plnA_type *apl = plnA_alloc( 2 );
	apl->a[apl->nA++] = pl;

	return( apl );
}

plnA_type *
plnF_get( plnF_type *fpl, int iFrame )
{
	iFrame -= fpl->frame0;

	if( iFrame < 0 || iFrame >= fpl->nA )
		return( NULL );

	return( fpl->a[iFrame] );
}



void
plnF_add( plnF_type *fpl, plnA_type *apl, int iFrame )
{
int	i;

	iFrame -= fpl->frame0;

	if( iFrame >= fpl->NA ){
		
		int NA = fpl->NA + iFrame+32;
		fpl->a = ( plnA_type **)realloc( fpl->a, NA*sizeof(plnA_type *) );

		for( i = fpl->NA ; i <  NA ; i++ )
			fpl->a[i] = NULL;

		fpl->NA = NA;
	}


	if( fpl->nA > iFrame && fpl->a[iFrame] != NULL )
		plnA_destroy( fpl->a[iFrame] );


	fpl->a[iFrame] = apl;
	apl->iFrame = iFrame + fpl->frame0;


	if( fpl->nA <= iFrame )
		fpl->nA = iFrame+1;

}


void
	plnF_append( plnF_type *fpl, plnA_type *apl, int iFrame )
{
	int	i;

	iFrame -= fpl->frame0;

	if( iFrame >= fpl->NA ){

		int NA = fpl->NA + iFrame+32;
		fpl->a = ( plnA_type **)realloc( fpl->a, NA*sizeof(plnA_type *) );

		for( i = fpl->NA ; i <  NA ; i++ )
			fpl->a[i] = NULL;

		fpl->NA = NA;
	}

	if( fpl->nA > iFrame && fpl->a[iFrame] != NULL ){
		plnA_append( fpl->a[iFrame], apl );
		//for( i = 0 ; i < apl->nA ; i++ )
		//	plnA_add( fpl->a[iFrame], apl->a[i] );

		//apl->nA = 0;
		//plnA_destroy( apl );
		return;
	}
	

	fpl->a[iFrame] = apl;
	apl->iFrame = iFrame + fpl->frame0;


	if( fpl->nA <= iFrame )
		fpl->nA = iFrame+1;

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
plnF_clear( plnF_type *fpl, int iFrame )
{

	iFrame -= fpl->frame0;

	if( iFrame < fpl->nA && fpl->a[iFrame] != NULL ){
		plnA_destroy( fpl->a[iFrame] );

		fpl->a[iFrame] = NULL;
	}
}