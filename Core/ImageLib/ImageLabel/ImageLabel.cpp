/*************************
 ***   Image1Label.c   ***
 *************************/
#include <math.h>
#include "Uigp/igp.h"
#include "ImageType/ImageType.h"

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Umath/Matrix2Type.h"
#include "Umath/EigenType.h"

#include "ImageLabel/ImageLabel.h"

#include "ImageDump/ImageDump.h"



imageLabel_type *
imageLabel_alloc()
{
	imageLabel_type *abw;

	abw = ( imageLabel_type *)malloc( sizeof(imageLabel_type) );

	abw->mim = NULL;

	abw->im = NULL;
	abw->a = NULL;
	abw->nA = 0;
	abw->margin = 0;

	return( abw );
}

void
imageLabel_destroy( imageLabel_type *abw )
{
	if( abw->a != NULL )
		free( abw->a );


	if( abw->im != NULL )
		image_destroy( abw->im, 1 );


	if( abw->mim != NULL )
		image_destroy( abw->mim, 1 );


	free( abw );
}


int
imageLabel_no( imageLabel_type *abw )
{
	int	i,	no;

	for( i = 0, no = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		if( bw->id != i )	continue;

		no++;
	}

	return( no );
}


int
imageLabel_bigest( imageLabel_type *abw, int color )
{
	int	i,	no,	iMax,	 nMax;

	nMax = iMax = 0;

	for( i = 0, no = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		if( bw->id != i )	continue;

		if( bw->color != color )
			continue;

		if( iMax < 0 || bw->no > nMax ){
			iMax = i;
			nMax = bw->no;
		}

		no++;
	}

	return( iMax );
}



int
imageLabel_print( imageLabel_type *abw, FILE *fp )
{
	int	i,	no;

	fprintf( fp, "Id\tColor\tno\n" );
	for( i = 0, no = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		if( bw->id != i )	continue;

		no++;
//		fprintf( fp, "%d  -   color: %d   no: %d\n", (int)bw->id, (int)bw->color, bw->no );
		fprintf( fp, "%d\t%d\t%d\t%d\t%d\n", (int)bw->id, (int)bw->color, (int)bw->av, (int)bw->var, bw->no );
	}

	return( no );
}


int
bwLabel_printA( bwLabel_type *bw, int nBw, FILE *fp )
{
	int	i,	no;

	for( i = 0, no = 0 ; i < nBw ; i++ ){
		if( bw[i].id != i )	continue;

		fprintf( fp, "%d  -   color: %d   no: %d\n", (int)bw[i].id, (int)bw[i].color, bw[i].no );
	}

	return( no );
}






void
bwLabel_merge( bwLabel_type *abw, int nBw, int id1, int id )
{
	int	i;

	abw[id].no += abw[id1 ].no;

	for( i = id1 ; i < nBw ; i++ ){
		if( abw[i].id == id1 )
			abw[i].id = id;
	}
}


int
bwLabel_no( bwLabel_type *bw, int nBw, int T )
{
	int	i,	no;

	for( i = 0, no = 0 ; i < nBw ; i++ ){
		if( bw[i].id != i )	continue;
		if( T <= 0 || bw[i].no > T )	no++;
	}

	return( no );
}


void
bwLabel_order( bwLabel_type *aBw, int nBw, int NI, int aI[], int *nI )
{
	int	i,	j;
	bwLabel_type	*bw;

	*nI = 0;
	for( i = 0 ; i < nBw ; i++ ){
		bw = &aBw[i];
		if( bw->id != i )	continue;



		for( j = *nI ; j > 0 ; j-- ){
			if( aBw[aI[j-1]].no > bw->no )	break;
			aI[j] = aI[j-1];
		}
		aI[j] = i;

		if( *nI < NI )
			(*nI)++;
	}
}


void
bwLabelA_order( imageLabel_type *abw, int color, int nT,  int NI, int aI[], int *nI )
{
	int	i,	j;
	bwLabel_type	*bw;

	*nI = 0;
	for( i = 0 ; i < abw->nA ; i++ ){
		bw = &abw->a[i];
		if( bw->id != i )	continue;

		if( nT > 0 && bw->no < nT )	continue;

		if( bw->color != color )
			continue;

		for( j = *nI ; j > 0 ; j-- ){
			if( abw->a[aI[j-1]].no > bw->no )	break;
			aI[j] = aI[j-1];
		}
		aI[j] = i;

		if( *nI < NI )
			(*nI)++;
	}
}