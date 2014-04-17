/****************************
 ***   HistogramOrder.c   ***
 ****************************/
#include <math.h>
#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"


#include "Histogram.h"


int 
histogram_order( int H[], int nH, int a[] )
{
int	i,	j,	nA;

	nA = 0;
	for( i = 0 ; i < nH ; i++ ){
		if( H[i] == 0 )	continue;

		for( j = nA ; j > 0 ; j-- ){
			if( H[ a[j-1] ] < H[i] )	break;
			a[j] = a[j-1];
		}

		a[j] = i;
		nA++;
	}

	return( nA );
}