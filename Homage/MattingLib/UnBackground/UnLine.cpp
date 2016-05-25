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
#include "../UnEdge/UnEdge.h"





int	CUnBackground::ProcessLineInit( char *prmFile )
{
	m_ue = new CUnEdge();

	m_ue->Init( prmFile );

	return( 1 );
}



int	CUnBackground::ProcessLine( int k0, int k1 )
{

	gp_time_start( &m_tEdge );


	int ret = m_ue->Process( m_yim, 1 );



	vl2fA_type *avl;

	m_ue->ProcessStraightLine( &avl );

	VL2FA_DUMP( avl, "un", 1, "line" );

	int	i;
	for( i = 0 ; i < avl->nA ; i++ )
		ProcessLineMask( m_abw, k0, k1, &avl->a[i] );

	gp_time_stop( &m_tEdge );





	return( 1 );
}



int	CUnBackground::ProcessLineMask( imageLabel_type *abw, int k0, int k1, vl2f_type *vl )
{
	if( ABS(vl->v.x) < ABS(vl->v.y) )
		return( -1 );

	if( vl->v.x < 0 ){
		vl->v.x = -vl->v.x;
		vl->v.y = -vl->v.y;
	}

	vec2f_type bu;
	VEC2D_RIGHT( vl->v, bu );



	vec2f_type hp1;
	hp1.x = m_hp.x;
	hp1.y = k1;

	vec2f_type	dp;
	dp.x = m_hp.x - vl->p.x;
	dp.y = k1 *8 - vl->p.y;

	float t1 = VEC2D_INNER( bu, dp );

	dp.x = m_hp.x - vl->p.x;
	dp.y = k0 *8 - vl->p.y;

	float t0 = VEC2D_INNER( bu, dp );


	if( t1 > 0 ){
		bu.x = -bu.x;
		bu.y = -bu.y;
	}
	else 
		if( t0 > 0 )
			return( -1 );



	int	i,	j;


	vec2f_type p0,	p1;
	vl2f_points( vl, &p0, &p1 );


	int i0 = p0.x /8 -1 ;
	int i1 = p1.x /8 + 1;


	
	u_int *bp = abw->im->data_ui;
	for( i = i0 ; i < i1 ; i++ ){
		u_int *bp = ( u_int *)IMAGE_PIXEL( abw->im, i, 0 );
		for( j = 0 ; j < abw->im->width ; j++, bp++ ){
			dp.x = 8*i - vl->p.x;
			dp.y = 8*j - vl->p.y;

			float t = VEC2D_INNER( bu, dp );
			if( t > 0 )	*bp = 0;
		}
	}

	return( 1 );

}
			
