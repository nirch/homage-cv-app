
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



//#define EXCEPTION



//static void	imageLabelUS_edge( imageLabel_type *abw, image_type *eim );






int	CUniformBackground::ProcessContour()
{

	if( m_contour ==  0  )
		return( 1 );


	gpTime_start( &m_tCln );



	if( m_dFrame == m_iFrame ){
		IMAGE_DUMP( m_cim, "cim", m_iFrame, NULL );
	}






	m_abwC = imageLabelUS_N( m_cim, 128, 0, 1, m_abwC );


	int i;

	for( i = 0; i < m_abwC->nA ; i++  ){
		if( m_abwC->a[i].id != i )	continue;	
		if( m_abwC->a[i].color == 0 )
			continue;

		m_cln = imageLabelUS_contour( m_abwC->im, i );

		cln_translate( m_cln, -m_abwC->margin, -m_abwC->margin );
		break;
	}

	

	if( m_dFrame == m_iFrame ){
		CLN_DUMP( m_cln, "AA", m_iFrame,  NULL );
	}


	plnA_type *apl = cln_to_plnA( m_cln, 1 );
	plnF_add( m_fpl, apl, m_iFrame );


	gpTime_stop( &m_tCln );

	return( 1 );
}


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



#ifdef _AA_
static int
	imageLabelUS_contour( image_type *mim, contourA_type *ac )
{


	bwLabel_type *aBw;
	int nBw;
	int	i;

	image_type *im = image1_binary( mim, 0, NULL );



	contourA_clear( ac );



	image_type *lim = image1_label( im, &aBw, &nBw, NULL );
	imageLabelUS_set_aux( lim, aBw, nBw );

	for( i = 1; i < nBw ; i++  ){
		if( aBw[i].id != i )	continue;	
		if( aBw[i].color == 0 )
			continue;

		if( aBw[i].no < 1000 )
			continue;

		eigen2d_type e;
		imageLabelUI_eigen2d( lim, i, &aBw[i].b, &e );

		contour_type *c = imageLevel_contour( lim, i );
		contourA_add( ac, c );

	}

	//	if( i >= nBw )	return( NULL );

	free( aBw );

	image_destroy( im, 1 );
	image_destroy( lim, 1 );


	return( 1 );

}
#endif