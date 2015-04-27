/**********************
 ***   fKmean.cpp   ***
 **********************/
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"

#include "fKmean.h"







static float	fKmean_update( float a[], int nA, fKmean_type ak[], int nK );

static float	fKmean_histogram_update( int a[], int nA, fKmean_type ak[], int nK );



int 
fKmean( float a[], int nA, fKmean_type ak[], int nK, float d )
{
int	k,	i;
float	m;


	floatA_order( a, nA );

	for( k = 0, m = a[0]+ 0.5 *d ; m < a[nA-1] ; k++, m += d ){
		ak[k].m = m ;
	}

	nK = k;


	for( i = 0; i < 10 ; i++ ){
		m = fKmean_update( a, nA, ak,  nK );
		if( ABS(m) < 0.1 )	break;
	}

	return( nK );
}


static float
fKmean_update( float a[], int nA, fKmean_type ak[], int nK )
{
	int	i,	k;
	float	T,	m;

	i = 0;
	for( k = 0 ; k < nK-1 ; k++ ){
		T = 0.5*(ak[k].m + ak[k+1].m);

		fKmean_type *fk = &ak[k];
		fk->s = 0;
		fk->n = 0;
		for( ; i < nA && a[i] < T ; i++ ){
			fk->s += a[i];
			fk->n ++;
		}
		if( fk->n > 0 ){
			m = fk->s/fk->n;
			fk->dm = m - fk->m;
			fk->m = m;
		}
	}

	fKmean_type *fk = &ak[k];
	fk->s = fk->dm = 0;
	fk->n = 0;
	for( ; i < nA ; i++ ){
		fk->s += a[i];
		fk->n ++;
	}

	if( fk->n > 0 ){
		m = fk->s/fk->n;
		fk->dm = m - fk->m;
		fk->m = m;
	}



	m = ak[0].dm;
	for( k = 1 ; k < nK ; k++ ){
		if( ABS(ak[k].dm) > ABS(m) )
			m = ak[k].dm;
	}

	return( m );
}





int 
fKmean_histogram( int a[], int nA, fKmean_type ak[], int nK, float d )
{
	int	k,	i;
	float	m;



	for( k = 0, m = 0+ 0.5 *d ; m < nA-1 ; k++, m += d ){
		ak[k].m = m ;
	}

	nK = k;


	for( i = 0; i < 10 ; i++ ){
		m = fKmean_histogram_update( a, nA, ak,  nK );
		if( ABS(m) < 0.1 )	break;
	}

	return( nK );
}



static float
fKmean_histogram_update( int a[], int nA, fKmean_type ak[], int nK )
{
int	i,	k;
float	T,	m;

	i = 0;
	for( k = 0 ; k < nK-1 ; k++ ){
		T = 0.5*(ak[k].m + ak[k+1].m);

		fKmean_type *fk = &ak[k];
		fk->s = 0;
		fk->n = 0;
		for( ; i < nA && i < T ; i++ ){
			fk->s += a[i]*i;
			fk->n += a[i];
		}
		if( fk->n > 0 ){
			m = fk->s/fk->n;
			fk->dm = m - fk->m;
			fk->m = m;
		}
		else fk->dm = 0;
	}

	fKmean_type *fk = &ak[k];
	fk->s = fk->dm = 0;
	fk->n = 0;
	for( ; i < nA ; i++ ){
		fk->s += a[i]*i;
		fk->n += a[i];
	}

	if( fk->n > 0 ){
		m = fk->s/fk->n;
		fk->dm = m - fk->m;
		fk->m = m;
	}




	m = ak[0].dm;
	for( k = 1 ; k < nK ; k++ ){
		if( ABS(ak[k].dm) > ABS(m) )
			m = ak[k].dm;
	}

	return( m );
}





void
floatA_order( float a[], int nA )
{
	int	i,	j,	tmp;

	for( i = 1; i < nA ; i++ ){
		tmp = a[i];

		for( j = i ; j > 0 ; j-- ){
			if( tmp > a[j-1] )	break;

			a[j] = a[j-1];
		}

		a[j] = tmp;
	}
}




int 
fKmean_cyclic( fKmean_type ak[], int nK )
{
int	i;


int	i0,	i1;

	i1 = 0;
	for( i = 1; i < nK ; i++ ){
		if( ak[i].n > ak[i1].n )
			i1 = i;
	}

	i0 = 0;
	for( i = 1; i < i1 ; i++ ){
		if( ak[i].n > ak[i0].n )
			i0 = i;
	}

	if( ak[i0].n > 0.5 * ak[i1].n ){
		float m = ak[i1].m - 2*ak[i0].m;
		if( ABS(m) < 10  )
			return( i0 );
	}

	return( i1 );
}



