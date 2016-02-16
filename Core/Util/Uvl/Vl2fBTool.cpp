/*****************
	RegAgpLinec
******************/

#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include "Ulog/Log.h"

#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"

#include "Ucamera/Pt2dType.h"

#include "Uln/PlnType.h"



#include "Vl2fType.h"


vl2fB_type *
vl2fB_alloc( int n )
{
	vl2fB_type	*avl;


	avl = (vl2fB_type *)malloc( sizeof( vl2fB_type) );

	avl->NA = n;
	avl->a = ( vl2fA_type **)malloc( avl->NA*sizeof(vl2fA_type*) );

	avl->nA = 0;

	GPMEMORY_LEAK_ALLOC( avl );

	return( avl );
}


vl2fB_type *
vl2fB_realloc( vl2fB_type *avl, int n )
{
	if( avl == NULL ){
		avl = vl2fB_alloc(n );
		return( avl );
	}


	if( avl->NA < n ){
		avl->a = ( vl2fA_type **)realloc( avl->a, n*sizeof(vl2fA_type *) );
		avl->NA = n;
	}

	return( avl );
}





void
vl2fB_destroy( vl2fB_type *bvl )
{
int	i;
	GPMEMORY_LEAK_DESTROY( bvl );

	for( i = 0 ; i < bvl->nA ; i++ )
		vl2fA_destroy( bvl->a[i] );

	free( bvl->a );

	free( bvl );
}



int
vl2fB_add( vl2fB_type *bvl, vl2fA_type *avl )
{
	if( bvl->nA >= bvl->NA ){
		bvl->NA += 100;
		bvl->a = ( vl2fA_type **)realloc( bvl->a, bvl->NA*sizeof(vl2fA_type *) );
	}

	bvl->a[bvl->nA++] = avl;

	return( bvl->nA-1);
}


void
vl2fB_cycle( vl2fB_type *fvl )
{
	int	i;

	if( fvl->nA == 0 )
		return;



	vl2fA_type *tmp = fvl->a[ fvl->NA-1 ];

	for( i = fvl->NA-1 ; i > 0  ; i-- )
		fvl->a[i] = fvl->a[i-1];

	fvl->a[0] = tmp;

	if( fvl->nA < fvl->NA )
		fvl->nA++;
}

