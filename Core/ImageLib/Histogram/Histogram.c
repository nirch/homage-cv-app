/********************************
 ***   Histogram.c   ***
 ********************************/

#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"

#include "Histogram.h"



int
histogram_range( int h[], int nH, float p0, float p1, float *r0, float *r1 )
{
int	i;
int	sum;
float	T;
int	nPixel;


	for( i = 0, nPixel = 0 ; i < nH ; i++ )
		nPixel += h[i];



	T = nPixel * p0/100;
	for( i = 0, sum = 0 ; i < nH ; i++ ){
		sum += h[i];
		if( sum > T )	break;
	}
	*r0 = ( i > 0 )? i -1 : 0;



	T = nPixel * p0/100;
	for( i = nH-1, sum = 0 ; i > 0 ; i-- ){
		sum += h[i];
		if( sum > T )	break;
	}

	*r1 = ( i < nH-1 )? i : nH-1;

	return( nPixel );
}






void
histogram_write( int h[], int nH, char *file )
{
	FILE	*fp;
	int		i;


	if( (fp = fopen( file, "wb")) == NULL )
		return;

	for( i = 0; i < nH ; i++ )
		fprintf( fp, "%d		%d\n", i, h[i] );

	fclose( fp );
}



float
histogram_median( int h[], int nH )
{
	int	i;
	int	sum;
	float	T,	t,	t0;
	int	nPixel;


	for( i = 0, nPixel = 0 ; i < nH ; i++ )
		nPixel += h[i];



	T = nPixel * 50/100;
	for( i = 0, sum = 0 ; i < nH ; i++ ){
		sum += h[i];
		if( sum > T )	break;
	}

	t0 = sum - h[i];

	t = i +  ( T - t0 )/( sum - t0 );



	return( t );
}