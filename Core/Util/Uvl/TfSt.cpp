/******************
 ***   TfMean.cpp   ***
 ******************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"

#include "Umath/St/StType.h"

#include	"TfType.h"



int tfA_st( tfA_type *atf, int i0, int i1, int j0, int a0, int T, st_type *st )
{
	int	i;


	st_init( st );



	for( i = i0 ; i <= i1 ; i++ ){

		tf_type *tf = tfA_get( atf, i );
		if( tf == NULL )	continue;

		float a = tf->a[j0];

		if( a == 0 )	continue;

		if( a0 != 0 ){
			float t = a - a0;
			if( ABS(t) > T )
				continue;
		}

		st_add( st, a );
	}

	if( st->n < 4 )
		return( -1 );

	st_final( st );

	return( 1 );
}

