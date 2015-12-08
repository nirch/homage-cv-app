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
#include "ImageMorphology/ImageMorphology.h"
#include "Imagelabel/BwLabel.h"

#define DIFF_THRESH 16// 12


#include	"UnBackground.h"

#include	"BnImage/BnImage.h"

#include "ImageLabel/ImageLabel.h"


#include "bImage.h"


int	CUnBackground::ProcessState()
{
int	r0[3],	r1[3],	i;

	

	if( TestDrakness( m_yim ) > 0 ){
		m_state = -10;
		return( m_state );
	}



	int	i0 = 0;
	for( i = i0+1 ; i < m_nM ; i++  )
		if( m_sWeight[i] > m_sWeight[i0] )	i0  = i;


	float	wh[3];
	int i1 = m_hbox[i0].x1/8;
	TestSilhouetteHead( &m_asB[0], i1, m_abw, wh );


	if( wh[0] < 0.25 || wh[1] < 0.25 ){
		m_state = -11;
		return( m_state );
	}


	if( wh[2] > 0.85 ){
		m_state = -6;
		return( m_state );
	}
	
	int	val0,	val1,	y0,	y1;
	r0[2] = sbA_value( &m_as0, &r0[0], &r0[1], &y0, &val0 );
	r1[2] = sbA_value( &m_as1, &r1[0], &r1[1], &y1,	&val1 );


	if(  m_sWeight[i0] < 30 ){
		m_state = -11;
		return( m_state );
	}


	if( r0[1] < 0.65* m_as0.nA || r0[1] < 0.65*m_as1.nA - 10 ){
		m_state = -4;
		return( m_state );
	}

#ifdef _AA_
	if( r0[1] < m_as0.nA - 10 || r0[1] < m_as1.nA - 10 ){
		m_state = -1;
		return( m_state );
	}


	if( r0[1] < m_as0.nA - 4 || r0[1] < m_as1.nA - 4 ){
		m_state = -1;
		return( m_state );
	}
#endif


	int dval = val0 - val1;
	int dr = y1 - val1;
	int	dl = y0 - val0;
	if(  ABS(dval) > 40 && ( ABS(dr) > 12  || ABS(dl) > 12 ) ){
		fprintf( stdout, "shadow: %d(%d) %d(%d)\n", val0, y0, val1, y1 );
		m_state = -4;//( val0 < val1 )? -2 : -3;
		return( m_state );
	}
	else fprintf( stdout, "shadow: %d(%d) %d(%d)\n", val0, y0, val1, y1 );


#ifdef _SHADOW_IN
	if( TestShadowIn() > 0 ){
		m_state = -7;
		return( m_state );
	}
#endif

	if( TestSilhouetteDeviation() != 0 ){

		m_state = -5;
		return( m_state );
	}



	m_state = 1;

	return( m_state );
}




int	imageLabelUI_blobe_sbA( imageLabel_type *abw, int iB, sbA_type *as );

int	sbA_in( sbA_type *as0, sbA_type *as1, sbA_type *as );

int	sbA_in_side( sbA_type *as0, sbA_type *as1, sbA_type *as, float *d0, float *d1 );


int	CUnBackground::TestShadowIn()
{

sbA_type as;
int	i;


	for( i = 0 ; i < m_abw->nA ; i++ ){

		bwLabel_type *bw = &m_abw->a[i];
		if( bw->id != i )	continue;
		if( bw->color != 2 )	continue;
		if( bw->no < 30 )	continue;


		imageLabelUI_blobe_sbA( m_abw, i, &as );

		if( sbA_in( &m_as0, &m_as1, &as ) < 0 )
			continue;


		float d0,	d1;
		sbA_in_side( &m_as0, &m_as1, &as, &d0, &d1 );

		if( d0 < 10 && d1 > 10 ||  d0 > 10 && d1 < 10)
			return( 1 );

	}

	return( -1 );

}



int
sbA_in( sbA_type *as0, sbA_type *as1, sbA_type *as )
{
	int	i;

	int	n0,	n1;
	n0 = n1 = 0;
	for( i = 0 ; i < as0->nA ; i++ ){
		if( as->a[i].j0 < 0 )	continue;


		if( as0->a[i].j1 > as->a[i].j0  ||  as1->a[i].j0 < as->a[i].j1 )
			return( -1 );

	}


	return( 1 );
}


int
	sbA_in_side( sbA_type *as0, sbA_type *as1, sbA_type *as, float *d0, float *d1 )
{
	int	i;

	int	n;
	n = 0;
	*d0 = *d1 = 0;
	for( i = 0 ; i < as0->nA ; i++ ){
		if( as->a[i].j0 < 0 )	continue;

		*d0 += as->a[i].j0 - as0->a[i].j1;
		*d1 += as1->a[i].j0 - as->a[i].j1;
		n++;
	}

	*d0 /= n;
	*d1 /= n;

	return( 1 );
}

