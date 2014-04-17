
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>



#ifdef _DEBUG
//#define _DUMP
#endif


#include	"Uigp/igp.h"
#include	"Ulog/Log.h"



#include "Uln/PlnType.h"
#include "Uln/CrPlnType.h"


#include "Umath/Kmean/fKmean.h"






static int	plnA_parallel_distance_1( plnA_type *apl, int i0, float r0, float r1, float au[]  );



float
plnA_parallel_distance(  plnA_type *apl, float r0, float r1 )
{
int	i,	n;
float	*au;

	au = ( float *)malloc( apl->nA*apl->nA*sizeof(float) );

	//for( i = 0 ; i < 512 ; i++ )
	//	au[i] = 0;


	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		pln_type *pl = apl->a[i];


		n += plnA_parallel_distance_1( apl, i, r0, r1, &au[n] );

	}

//	float u = aa( au, 512, 10 );
	fKmean_type ak[100];

	int nK = fKmean( au, n, ak, 10, 20 );

	i = fKmean_cyclic( ak, nK );

	float u = ak[i].m;

	free( au );
	return( u );
}





int
plnA_parallel_distance_1( plnA_type *apl, int i0, float r0, float r1, float au[]  )
{
	pln_type	*bpl;
	int	i,	n;
	float	u;


	float	aT = cos( ANGLE_D2R(15.0) );

	bpl = apl->a[i0];
	for( i = i0+1, n = 0 ; i < apl->nA ; i++ ){
		if( i == i0 )	continue;


		if( pln_parallel_distance( bpl, apl->a[i], 20.0, aT, r0 , r1, &u ) < 0 )
			continue;


//		if( (k = u) < 0 )	k = -k;
//		au[k]++;
		au[n] = ABS(u);
		n++;

	}


	return( n );
}



