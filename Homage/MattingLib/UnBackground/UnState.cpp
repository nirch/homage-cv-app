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


	float	wh[2];
	int i1 = m_hbox[i0].x1/8;
	TestSilhouetteHead( &m_asB[0], i1, m_abw, wh );

	if( wh[0] < 0.25 || wh[1] < 0.25 ){
		m_state = -11;
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



	if( TestSilhouetteDeviation() != 0 ){
		//if( nw[0] < 0.25 || nw[1] < 0.25 ){
		//	m_state = -11;
		//	return( m_state );
		//}
		m_state = -5;
		return( m_state );
	}



	m_state = 1;

	return( m_state );
}


