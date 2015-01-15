/********************************
 ***   Image3DenaimcRange.c   ***
 ********************************/
#include <math.h>
#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"


#include "Histogram.h"


//typedef struct isdata1_type {
//	int	i0,	i1;
//
//	int	n;
//
//	float	mean;
//	float	var;	
//
//	float	d;
//}isdata1_type;


static void	clustring_isodata_histogram_init( int H[], int nH, int K, isdata1_type c[], int *nC );

static void	clustring_isodata_aux( isdata1_type c[], int nC, int H[], int nH );

static void	clustring_isodata_aux_1( isdata1_type *c, int H[], int nH );

static void	clustring_isodata_remove_small( isdata1_type c[], int *nC, int nT );

static int	clustring_isodata_split( isdata1_type c[], int *nC, int vT );

static int	clustring_isodata_merege( isdata1_type c[], int *nC, int dT );

static void	clustring_isodata_print( FILE *fp, isdata1_type c[], int nC );



void	histogram_clustring_isodataA( int H[], int nH, int K, int nT, float vT, float mT, int Itration, isdata1_type c[], int *nM );


void
histogram_clustring_isodata( int H[], int nH, int K, int nT, float vT, float mT, int Itration, float am[], int *nM )
{
	isdata1_type	*ac;
	int	nC,	i;

	ac = (isdata1_type *)malloc( 2*K*sizeof(isdata1_type));


	histogram_clustring_isodataA( H, nH, K, nT, vT, mT, Itration, ac, &nC );

	for( i = 0 ; i < nC ; i++ ){
		am[i] = ac[i].mean;
	}

	*nM = nC;

	free( ac );
}

void
histogram_clustring_isodataA( int H[], int nH, int K, int nT, float vT, float mT, int Itration, isdata1_type c[], int *nM )
{
//isdata1_type	*c;
int	nC,	i;

//	c = (isdata1_type *)malloc( 2*K*sizeof(isdata1_type));

	clustring_isodata_histogram_init( H, nH, K, c, &nC );



	for( i = 0 ; i < Itration ; i++ ){


		clustring_isodata_aux( c, nC, H, nH  );
//		clustring_isodata_print( stdout, c, nC );

		clustring_isodata_remove_small( c, &nC, nT  );

		clustring_isodata_aux( c, nC, H, nH  );


		if( nC < K ){
			clustring_isodata_split( c, &nC, vT );
			continue;
		}


		if( nC > K ){
			clustring_isodata_merege( c, &nC, mT  );
			continue;
		}
	}
	clustring_isodata_aux( c, nC, H, nH  );
	clustring_isodata_print( stdout, c, nC );

	//for( i = 0 ; i < nC ; i++ ){
	//	am[i] = c[i].mean;
	//	an[i] = c[i].n;
	//}

	*nM = nC;

//	free( c );
}


static int
clustring_isodata_split( isdata1_type c[], int *nC, int vT )
{
	int	i,	i0;
	float	d;

	i0 = 0;
	for( i = 1 ; i < *nC ; i++ ){
		if( c[i].var > c[i0].var )
			i0 = i;
	}

	if( c[i0].var < vT )
		return( -1 );

	for( i = *nC ; i > i0 ; i-- )
		c[i] = c[i-1];

//	nC++;

	d = 0.125*sqrt( c->var);
	c[i0].mean = c[i0].mean - d;
	c[i0+1].mean = c[i0+1].mean;

	*nC += 1;

	return( 1 );
}



static int
clustring_isodata_merege( isdata1_type c[], int *nC, int dT )
{
int	i,	i0;
float	d,	dMin;

	i0 = -1;
	dMin = 0;
	for( i = 0 ; i < *nC-1 ; i++ ){
		d = c[i+1].mean - c[i].mean;
		if( d < dMin || i0 < 0 ){
			dMin = d;
			i0 = i;
		}
	}

	if( dMin > dT )
		return( -1 );


	c[i0].mean = ( c[i0].n * c[i0].mean + c[i0+1].n * c[i0+1].mean)/( c[i0].n + c[i0+1].n );

	*nC -= 1;
	for( i = i0+1 ; i < *nC ; i++ )
		c[i] = c[i+1];

	return( 1 );
}


static void
clustring_isodata_remove_small( isdata1_type c[], int *nC, int nT )
{
int	i,	j;
	for( i = 0, j = 0 ; i < *nC ; i++ ){
		if( c[i].n < nT )	continue;

		c[j++] = c[i];
	}

	*nC = j;
}


static void
clustring_isodata_aux( isdata1_type c[], int nC, int H[], int nH )
{
int	k;
float	r;

	c[0].i0 = 0;
	for( k = 0 ; k < nC-1 ; k++ ){
		r = 0.5*( c[k].mean + c[k+1].mean);
		c[k].i1 = r;
		c[k+1].i0 = r+1;
	}

	c[nC-1].i1 = nH-1;


	for( k = 0; k < nC ; k++ )
		clustring_isodata_aux_1( &c[k], H, nH );

}

static void
clustring_isodata_aux_1( isdata1_type *c, int H[], int nH )
{
int	i;
float	sum,	sum2,	d;
int	n;

		

	sum = sum2 = 0;
	n = 0;

	for( i = c->i0 ; i <= c->i1 ; i++ ){

		sum += H[i]*i;
		sum2 += H[i]*i*i;
		n += H[i];
	}

	if( n == 0 ){
		c->n = 0;
		c->var = c->var = c->d = c->mean = 0;
		return;
	}

	c->n = n;
	c->mean = sum /c->n;

	c->var = sum2/c->n - c->mean*c->mean;

	sum = 0;
	for( i = c->i0 ; i <= c->i1 ; i++ ){
		d = i - c->mean;
		if( d < 0 )	d = -d;

		sum += H[i]*d;
	}

	c->d = sum / c->n;
}



static void	histogram_minmax( int H[], int nH, int *m0, int *m1, int *samples );

static void
clustring_isodata_histogram_init( int H[], int nH, int K, isdata1_type c[], int *nC )
{
int	m0,	m1,	sum;
int	k,	i;
	histogram_minmax( H, nH, &m0, &m1, &sum );


	k = 0.5*(m1 - m0);
	if( K < k )	k = K;

	for( i = 0 ; i < k ; i++ ){
		c[i].mean = (i+0.5) * (m1 - m0)/ k;
	}

	*nC = k;
}


static void
histogram_minmax( int H[], int nH, int *m0, int *m1, int *samples )
{
int	i;
	*m0 = *m1 = -1;
	*samples = 0;

	for( i = 0 ; i < nH ; i++ ){
		if( H[i] ==  0 )	continue;
		if( *m0 < 0 )	*m0 = i;

		*m1 = i;

		*samples += H[i];
	}
}



static void
clustring_isodata_print( FILE *fp, isdata1_type c[], int nC )
{
	int	i;
	for( i = 0 ; i < nC ; i++ ){
		fprintf( fp, "%d   %.2f    %.2f    %d\n", i, c[i].mean, c[i].var, c[i].n );
	}
}