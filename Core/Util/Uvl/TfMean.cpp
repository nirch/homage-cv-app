/******************
 ***   TfMean.cpp   ***
 ******************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"
#include	"TfType.h"



float	tfA_mean_1( tfA_type *tfA, float a0, float T,  float *av, float *var, int aI[]  );


int	tfA_mean_1( tfA_type *tfA, tf_type *tf, float T, float *av, float *var, int aI[] );




int
tfA_mean( tfA_type *tfA, float T, float *av, float *var, int aI[] )
{
	float cav,	cvar;
	int	i,	n,	nMax;
	int	cI[100];

	nMax = -1;
	for( i = 0 ; i < tfA->nT ; i++ ){

		n = tfA_mean_1( tfA, tfA->t[i],  T,  &cav, &cvar, cI );

		if( nMax < 0 || n > nMax || n == nMax && cvar < *var ){
			nMax = n;
			*av = cav;
			*var = cvar;

			//			memcpy( aI, cI, tfA->nT *sizeof(int) );

			for( int k = 0 ; k < tfA->nT ; k++ )
				aI[k] = cI[k];
		}	
	}

	return( nMax );

}



int
tfA_mean_1( tfA_type *tfA, tf_type *tf, float T, float *av, float *var, int aI[] )
{
	float cav,	cvar;
	int	i,	n,	nMax;
	int	cI[100];

	nMax = -1;
	for( i = 0 ; i < tf->nA ; i++ ){

		if( tf->a[i] >= 1000 )
			continue;

		n = tfA_mean_1( tfA, tf->a[i],  T,  &cav, &cvar, cI );

		if( nMax < 0 || n > nMax || n == nMax && cvar < *var ){
			nMax = n;
			*av = cav;
			*var = cvar;

			//			memcpy( aI, cI, tfA->nT *sizeof(int) );
			for( int k = 0 ; k < tfA->nT ; k++ )
				aI[k] = cI[k];

		}	
	}

	return( nMax );

}

float
tfA_mean_1( tfA_type *tfA, float a0, float T,  float *av, float *var, int aI[] )
{
	int	i,	j,	n;

	n = 0;
	*av = *var = 0;
	for( i = 0 ; i < tfA->nT ; i++ ){

		tf_type *tf = tfA->t[i];


		j = tfA_nearest( tf, a0, T );
		aI[i] = j;

		if( j < 0 )	continue;

		*av += tf->a[j];
		*var += tf->a[j] * tf->a[j];
		n++;
	}


	if( n < 0 )	return( -1 );

	*av /= n;
	*var = *var / n - *av * *av;

	return( n );
}
