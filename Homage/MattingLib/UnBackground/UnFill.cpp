
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


#define DIFF_THRESH 16// 12


#include	"UnBackground.h"



#include "bImage.h"



static int	bImage_fill( image_type *bim, int a0[], int a1[], float sT, int *similarity );



int	CUnBackground::ProcessFill()
{

	int	a0[256],	a1[256];
	int	i;

	a0[0] = a1[0] = -1;
	for( i = 0 ; i < m_as0.nA ; i++ ){

		a0[i+1] = ( m_as0.a[i].j1 < 0 )? -1 : m_as0.a[i].j1+1;
		a1[i+1] = ( m_as1.a[i].j0 < 0 )? -1 : m_as1.a[i].j0+1;
	}
	a0[i+1] = a1[i+1] = -1;


//#ifdef _NEWA_
	//if( m_closeUp ){
	//	bImage_fill_gapR( m_bim, a1 );
	//	bImage_fill_gapL( m_bim, a0 );
	//}
//#endif

		
	bImage_fill_gap( m_bim, a0, F_LEFT );
	bImage_fill_gap( m_bim, a1, F_RIGHT );
		

//#ifdef _NEWA_
	bImage_fill_gap_up( m_bim, a0, a1 );
//#endif

	int	T = 24;
	
	int no = bImage_fill( m_bim, a0, a1, T, &m_similarity );

	fprintf( stdout, "similarity: %d ( %d)\n", m_similarity, no );

	BIMAGE_DUMP( m_bim, m_N, "un", 1, "F" );



	for( i = 0 ; i < m_as0.nA ; i++ ){
		m_asFigure.a[i].j0 = a0[i];
		m_asFigure.a[i].j1 = a1[i];
	}
	m_asFigure.nA = m_as0.nA;

	return( 1 );
}




static int
bImage_fill( image_type *bim, int a0[], int a1[], float sT, int *similarity )
{
	int	i,	k,	no,	nh;
	bImage_type *bp;

	*similarity = 0;
	no = nh = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 0 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += bim->width ){

		if( a0[i] == -1 || a1[i] == -1 )
			continue;


		if( a0[i] < 0 || a1[i] < 0 )
			continue;


		int	j0,	j1;
		bImage_type *bp0,	*bp1;

		if( a0[i] < 0 ){
			j0 = 0;
			bp0 = bp + a1[i];
		}
		else {
			j0 = a0[i];
			bp0 = bp + a0[i];
		}



		if( a1[i] < 0 ){
			j1 = bim->width-1;
			bp1 = bp0;
		}
		else {
			j1 = a1[i];
			bp1 = bp + a1[i];
		}



		nh++;

		bImage_type *cp;
		for( k = j0 + 1, cp = bp0+1 ; k < j1 ; k++, cp++ ){


			float f = (k - j0)/(float)( j1 - j0 ); 


			if( f < 0.25 )	f /= 3;
			else	if( f < 0.75 )	f = 0.5 + ( f - 0.5)*5.0/3 ;
			else	f = 1 - ( 1-f)/3;


			int r = (1-f) * bp0->r + f* bp1->r;
			int g = (1-f) * bp0->g + f* bp1->g;
			int b = (1-f) * bp0->b + f* bp1->b;

			if( nh > 3 && k > j0+3 && k < j1-3 && cp->n > 1  ){
				int dr = r - cp->r;
				int dg = g - cp->g;
				int db = b - cp->b;

				if( dr < 0 )	dr = -dr;
				if( dg < 0 )	dg = -dg;
				if( db < 0 )	db = -db;


				bImage_type *cc = ( bImage_type *)IMAGE_PIXEL( bim, i-1, k-1 );

				if( dr < sT && dg < sT && db < sT )
					(*similarity)++;
			}



			cp->r = r;
			cp->g = g;
			cp->b = b;
			cp->n = 1;

			no++;
		}
	}


	return( no );
}
