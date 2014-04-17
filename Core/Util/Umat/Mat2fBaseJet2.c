/*************************
 ***	Mat2fBaseDct   ***
 *************************/

#include	<stdio.h>

#include	<math.h>

#include	"Uigp/igp.h"
#include	"Umat/Mat2fType.h"



void
mat2f_base_jet2( mat2f_type *base[], int N, int M ) 
{
int	i,	j;
float	*f00,	*f10,	*f01,	*f20,	*f11,	*f02;
float	x,	y;

	base[0] = mat2f_alloc( N, M);
	base[1] = mat2f_alloc( N, M);
	base[2] = mat2f_alloc( N, M);
	base[3] = mat2f_alloc( N, M);
	base[4] = mat2f_alloc( N, M);
	base[5] = mat2f_alloc( N, M);

	f00 = base[0]->f;
	f10 = base[1]->f;
	f01 = base[2]->f;
	f20 = base[3]->f;
	f11 = base[4]->f;
	f02 = base[5]->f;


	for( i = 0 ; i < N ; i++ ){
//		y = ( -(2*N+1) +1 + 2*i)/( 2.0 * N+1 );
		y = ( -(N) +1 + 2*i)/(float)( N );
		for( j = 0 ; j < M ; j++ ){
			x = ( -(M) +1 + 2*j)/(float)( M );

			*f00++ = 1;

			*f10++ = x;
			*f01++ = y;

			*f20++ = x*x;
			*f11++ = x*y;
			*f02++ = y*y;
		}
	}
}


