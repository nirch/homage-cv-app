
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"
#include "ImageLabel/ImageLabel.h"

#include "../PlnAdjust/PlnAdjust.h"


//#define EXCEPTION





int	CUniformBackground::ProcessContour()
{

	if( m_contour ==  0  )
		return( 1 );


	gpTime_start( &m_tCln );



	if( m_dFrame == m_iFrame ){
		IMAGE_DUMP( m_cim, "cim", m_iFrame, NULL );
	}






	m_abwC = imageLabelUS_N( m_cim, 128, 0, 1, m_abwC );



	int n = imageLabel_bigest( m_abwC, 1 );
	
	m_cln = imageLabelUS_contour( m_abwC->im, n );

	cln_translate( m_cln, -m_abwC->margin, -m_abwC->margin );

#ifdef _AA_
	int i;
	for( i = 0; i < m_abwC->nA ; i++  ){
		if( m_abwC->a[i].id != i )	continue;	
		if( m_abwC->a[i].color == 0 )
			continue;

		m_cln = imageLabelUS_contour( m_abwC->im, i );

		cln_translate( m_cln, -m_abwC->margin, -m_abwC->margin );
		break;
	}
#endif

	

	if( m_dFrame == m_iFrame ){
		CLN_DUMP( m_cln, "AA", m_iFrame,  NULL );
	}


	plnA_type *apl = cln_to_plnA( m_cln, 1 );


	plnA_smooth( apl );

	plnA_adjust_start( apl );



	//if( m_contour  == 2 ){
	//	plnA_adjust_thin( apl, m_iFrame );

	//	
	//}

	ProcessContourAdjust( apl );


	plnF_add( m_fpl, apl, m_iFrame );


	gpTime_stop( &m_tCln );

	return( 1 );
}


static void	image1_mask_sharp( image_type *sim, float a );


int	CUniformBackground::ProcessContourAdjust( plnA_type *apl )
{
	if( m_contour  < 2  )	
		return( -1 );

	plnA_adjust_thin( apl, m_iFrame );



	plnA_adjust_in( apl, m_fpl->a[m_iFrame-1], m_iFrame );



	cln_type *cln = cln_from_plnA( apl, 1 );
	m_cim = image1_mask_cln( cln, m_sim->width, m_sim->height, 1, m_cim );
	cln_destroy( cln );


//	image1_mask_sharp( m_cimS, 1.25 );

	ProcessSmooth();

	return( 1 );
}

#ifdef _AA_
int	CUniformBackground::ProcessContourUI()
{

	if( m_contour ==  0  )
		return( 1 );


	gpTime_start( &m_tCln );


	//	m_eim = image1_gridient_sobol_value( m_yim, m_eim );

	if( m_dFrame == m_iFrame ){
		IMAGE_DUMP( m_cim, "cim", m_iFrame, NULL );
	}



	//	m_abwC = imageLabelUS( m_cim, 128, 0, m_abwC );


	m_abwC = imageLabelUI_T( m_cim, 128, 1, m_abwC );

	//	imageLabelUI_set_aux( m_abwC->im, m_abwC->a, m_abwC->nA );
	imageLabelUI_set_box( m_abwC );




	int i;

	for( i = 0; i < m_abwC->nA ; i++  ){
		if( m_abwC->a[i].id != i )	continue;	
		if( m_abwC->a[i].color == 0 )
			continue;

		m_cln = imageLabelUI_contour( m_abwC->im, i );

		cln_translate( m_cln, -m_abwC->margin, -m_abwC->margin );
		break;
	}


	//	CLN_DUMP( m_cln, "AA", m_iFrame,  NULL );


	plnA_type *apl = cln_to_plnA( m_cln, 1 );
	plnF_add( m_fpl, apl, m_iFrame );


	gpTime_stop( &m_tCln );

	return( 1 );
}
#endif

int	CUniformBackground::WriteContour( char *file )
{
	cln_write( m_cln, file );

	return( 1 );
}


int	CUniformBackground::Write( char *file )
{
	plnF_write( m_fpl, file );

	return( 1 );
}




static void
image1_mask_sharp( image_type *sim, float a )
{
	u_char	*sp;
	int	i,	j;


	sp = (u_char *)sim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++ ){
			if( *sp == 0 || *sp == 255 )	continue;

			int t = (*sp - 128)*a + 0.5;
			*sp = PUSH_TO_RANGE( t, 0, 255 );
		}
	}
}