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


#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "Imagelabel/BwLabel.h"
#include "ImageLabel/ImageLabel.h"
#include "Histogram/Histogram.h"


#include	"UnBackground.h"

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
	for( i = 1 ; i < nC ; i++ ){
		if( ac[i].n > ac[i0].n || ac[i].n > 400 && ac[i].mean > 2*ac[i0].mean )
			i0 = i;
	}

	fprintf( stdout, "AAA %d %d\n", (int)ac[i0].mean, ac[i0].n );




	int	iB = -1;
	int	R,	G,	B,	n;
	R = G = B = n = 0;
	for( i = 0 ; i < nI ; i++ ){
		bwLabel_type *bw = &m_abw->a[aI[i]];
		int av = bw->av;
		if( av < ac[i0].i0 || av > ac[i0].i1 )
			continue;

		if( iB == -1 || bw->no > m_abw->a[iB].no )
			iB = aI[i];

		R += bw->R;
		G += bw->G;
		B += bw->B;
		n++;
	}

	if( n == 0 )
		return( 1 );

	R /= n;
	G /= n;
	B /= n;



	for( i = 0 ; i < nI ; i++ ){
		bwLabel_type *bw = &m_abw->a[aI[i]];
		int av = bw->av;
		if( av >= ac[i0].i0 && av <= ac[i0].i1 )
			continue;



		if( ProcessBlobB_TestBlob(  iB, aI[i] ) < 0 )
			bw->color = 2;
	}









	for( i = 0 ; i < m_abw->nA ; i++ ){
		bwLabel_type *bw = &m_abw->a[i];

		if( bw->id != i )	continue;
		if( bw->color != 0 )	continue;

		int	k;
		for( k = 0 ; k < nI ; k++ )
			if( aI[k] == i )
				break;
		if( k < nI )	continue;

		float dt = bw->av - ac[i0].mean;
	
		if( ABS(dt) > 64 || bw->no < 10 ){
			bw->color = 2;
			continue;
		}

		int	d,	dm;
		if( (d = R - bw->R) < 0 )	d = -d;
		dm = d;

		if( (d =G - bw->G) < 0 )	d = -d;
		if( d > dm )	dm = d;
		if( (d =B - bw->B) < 0 )	d = -d;
		if( d > dm )	dm = d;

		if( ABS(dm) > 64){
			bw->color = 2;
			continue;
		}

	}


	return( 1 );
}


int	imageLabelUI_blobe_sb( imageLabel_type *abw, int iB, sbA_type *as );

int	sbA_side( sbA_type *as0, sbA_type *as1 );


int	CUnBackground::ProcessBlobB_TestBlob( int b0, int b1 )
{
sbA_type as[2];
int	i;

	imageLabelUI_blobe_sb( m_abw, b0, &as[0] );
	imageLabelUI_blobe_sb( m_abw, b1, &as[1] );


	int i0 = sbA_side( &as[0], &as[1] );
	int i1 = 1 - i0;

	if( i0 < 0 ){
		bwLabel_type *bw0 = &m_abw->a[b0];
		bwLabel_type *bw1 = &m_abw->a[b1];
		int	r,	g,	b;
		if( (r = bw0->R - bw1->R) < 0 )	r = -r;
		if( (g = bw0->G - bw1->G) < 0 )	g = -g;
		if( (b = bw0->B - bw1->B) < 0 )	b = -b;

		if( r < 64 && g < 64 && b < 64 )
			return( 1 );

		return( -1 );
	}



	bImage_set_sbA_sideR( m_bim, &as[i0], 10 );
	bImage_set_sbA_sideL( m_bim, &as[i1], 10 );

//	sbA_write3( &as[i0], &m_asB[0], &as[i1], stdout );

	int n;
	for( i = 0,	n = 0 ; i < as[i0].nA ; i++ ){


		sb_type *s0 = &as[i0].a[i];
		sb_type *s1 = &as[i1].a[i];
		if( s0->j0 < 0 || s1->j0 < 0 )
			continue;

		int	r,	g,	b;
		if( (r = s0->r - s1->r) < 0 )	r = -r;
		if( (g = s0->g - s1->g) < 0 )	g = -g;
		if( (b = s0->b - s1->b) < 0 )	b = -b;



		if( r < 32 && g < 32 && b < 32 )
			n++;

	}

	if( n < 2 )
		return( -1 );
	
	return( 1 );
}


// return 0 if as0 is left to as1 
int
sbA_side( sbA_type *as0, sbA_type *as1 )
{
int	i;

	int	n0,	n1;
	n0 = n1 = 0;
	for( i = 0 ; i < as0->nA ; i++ ){
		if( as0->a[i].j0 < 0  ||  as1->a[i].j0 < 0 )
		continue;

		if( as0->a[i].j0 < as1->a[i].j0 ){
			n0++;
		}
		else	n1++;
	}

	if( n0 == 0 && n1 == 0 )
		return( -1 );

	if( n0 > n1 )
		return( 0 );

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


int	CUnBackground::TestSilhouetteHead( sbA_type *as, int i1, imageLabel_type *abw, float nw[] )
{
	int	i,	n[3];
	float	w[3];

	w[0] = w[1] = w[2] = n[0] = n[1] = n[2] = 0;


	for( i = 0 ; i < i1 ; i++ ){
		sb_type *s = &as->a[i];

		if( s->j0 < 0 )	continue;

		w[0] += imageLabelUI_nColor( abw, i, s->j0-8, s->j0, 0 );
		n[0] += 8;

		w[1] += imageLabelUI_nColor( abw, i, s->j1, s->j1+8, 0 );
		n[1] += 8;

		w[2] += imageLabelUI_nColor( abw, i, s->j0+4, s->j1-4, 0 );
		n[2] += s->j1 - s->j0 - 8;

	}

	nw[0] = (n[0] > 0 )? w[0] / (float)n[0] : 0;
	nw[1] = (n[1] > 0 )? w[1] / (float)n[1] : 0;
	nw[2] = (n[2] > 0 )? w[2] / (float)n[2] : 0;

	fprintf( stdout, "TestSilhouette- Head: %.2f( %d )   %.2f( %d )   %.2f( %d )  \n", nw[0], n[0], nw[1], n[1], nw[2], n[2] );


	return( 1 );

}


static  int
imageLabelUI_nColor( imageLabel_type *abw, int i0, int j0, int j1, int color )
{
	int	j,	n;
	
	u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i0, j0 );

	if( j0 < 0 )	j0 = 0;
	if( j1 >= abw->im->width )	j1 = abw->im->width;

	for( j = j0, n = 0 ; j < j1 ; j++, sp++ ){
		if( abw->a[*sp].color == color )
			n++;
	}



	return( n );
}
	


