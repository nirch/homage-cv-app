/*************************
 ***	Mat2fBaseDct   ***
 *************************/

#include	<stdio.h>

#include	<math.h>

#include	"Uigp/igp.h"
#include	"Umat/Mat2fType.h"

static mat2f_type *		mat2f_dct_sample( int n, int m,int N, int M );



void
mat2f_base_dct( mat2f_type *base[], int N, int M ) 
{
int	n,	m,	i;

	i = 0;
	for( n = 0 ; n < N ; n++ )
		for( m = 0 ; m < M ; m++ )
			base[i++] = mat2f_dct_sample( n, m, N, M );

	mat2f_base_ortonormalize( base, N*M );

}


static mat2f_type * 
mat2f_dct_sample( int n, int m,int N, int M )
{
int	i,	j;
float x, y,	cosy,	*f;
mat2f_type *mat;





	mat = mat2f_alloc( N, M);

	f = mat->f;
	for( i=0; i < N ; i++ ){

		y = ((2*i+1)*n* M_PI )/(2.0*N);

		cosy = cos( y );

		for ( j = 0 ; j < M ; j++ ){

			x = ((2*j+1)*m* M_PI )/(2.0*M);
		

			*f++ = cos(x)* cosy;

		}
	}

	return( mat );
}
