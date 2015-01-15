//  Defines the entry point for the console application.
//
#include <stdio.h>
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"
#include "Umath/Kmean/fKmean.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageMorphology/ImageMorphology.h"
#include "Imagelabel/BwLabel.h"

#define DIFF_THRESH 16// 12


#include	"UnBackground.h"

#include	"BnImage/BnImage.h"

#include "ImageLabel/ImageLabel.h"
#include "Histogram/Histogram.h"


#include "bImage.h"


int	CUnBackground::ProcessBlobB()
{
	int	i;
	int H[256];

	for( i = 0 ; i < 256 ; i++ )
		H[i]  = 0;

	int aI[16],	nI;
	bwLabelA_order( m_abw, 0, 50, 16, aI, &nI );
	for( i = 0 ; i < nI ; i++ ){
		bwLabel_type *bw = &m_abw->a[aI[i]];
		H[(int)bw->av] = bw->no;

		fprintf( stdout, "%d %d %d\n", aI[i], (int)bw->av, bw->no );
	}


	isdata1_type ac[16];
	int	nC;
	histogram_clustring_isodataA( H, 256, 4, 100, 400, 64, 10, ac, &nC );






	int i0 = 0;
	for( i = 1 ; i < nC ; i++ )
		if( ac[i].n > ac[i0].n )
			i0 = i;




	fprintf( stdout, "AAA %d %d\n", (int)ac[i0].mean, ac[i0].n );


	for( i = 0 ; i < m_abw->nA ; i++ ){
		bwLabel_type *bw = &m_abw->a[i];

		if( bw->id != i )	continue;
		if( bw->color != 0 )	continue;

		float dt = bw->av - ac[i0].mean;
	
		if( ABS(dt) > 64 || bw->no < 10 )
			bw->color = 2;
	}



	return( 1 );
}



int	CUnBackground::ProcessSilhouette()
{
int	i;



	for( i = 0 ; i < m_nM ; i++  )
		m_sWeight[i] = TestSilhouette( &m_asB[i], m_abw );
	return( 1 );
}





static  int	imageLabelUI_nColor( imageLabel_type *abw, int i0, int j0, int j1, int color );

int	CUnBackground::TestSilhouette( sbA_type *as, imageLabel_type *abw)
{
int	i,		nW,	n;

	nW = n = 0;

	for( i = 0 ; i < as->nA ; i++ ){
		sb_type *s = &as->a[i];

		if( s->j0 < 0 )	continue;

		nW += imageLabelUI_nColor( abw, i, s->j0-8, s->j0, 0 );
		n += 8;

		nW += imageLabelUI_nColor( abw, i, s->j1, s->j1+8, 0 );
		n += 8;

	}

	fprintf( stdout, "TestSilhouette: %d( %d %.2f )\n", nW, n, nW*100.0/n );


	return( nW*100.0/n );

}

static  int
imageLabelUI_nColor( imageLabel_type *abw, int i0, int j0, int j1, int color )
{
	int	j,	n;
	
	u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i0, j0 );

	for( j = j0, n = 0 ; j < j1 ; j++, sp++ ){
		if( abw->a[*sp].color == color )
			n++;
	}



	return( n );
}
	


