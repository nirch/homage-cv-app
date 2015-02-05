 
#include	<string.h>
#include	<math.h>

#include	"Uigp/igp.h"
#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif


#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"

//#include "Wutil/UWgpw/GpwType.h"
#include "Uln/PlnType.h"





#include	"pEdgeType.h"
#include	"SLineDetector.h"





CSlineDetector::CSlineDetector()
{

	m_gim =  NULL;
	m_gimS = NULL;

	m_yim = NULL;

	m_yim2 = NULL;


	m_ae = NULL;

	m_box.x0 = m_box.y0 = 0;


	m_prm = pEdgePrm_alloc();
}
	

CSlineDetector::~CSlineDetector()
{
	DeleteContents();
}


void CSlineDetector::DeleteContents()
{
	if( m_gim != NULL ){
		image_destroy( m_gim, 1 );
		m_gim = NULL;
	}

	if( m_gimS != NULL ){
		image_destroy( m_gimS, 1 );
		m_gimS = NULL;
	}


	if( m_yim != NULL ){
		image_destroy( m_yim, 1 );
		m_yim = NULL;
	}

	if( m_yim2 != NULL ){
		image_destroy( m_yim2, 1 );
		m_yim2 = NULL;
	}


	if( m_ae != NULL ){
		plEdgeA_destroy( m_ae );
		m_ae = NULL;
	}



}



int CSlineDetector::Init( char *prmFile )
{
	return( 1 );
}


int			
CSlineDetector::Process( image_type *sim, box2i *box, int iFrame )
{
int	ret;
box2i	b;

	b.x0 = (box->x0 / m_prm->d) * m_prm->d;
	b.x1 = ((box->x1 + m_prm->d -1 ) / m_prm->d) * m_prm->d;

	b.y0 = (box->y0 / m_prm->d) * m_prm->d;
	b.y1 = ((box->y1 + m_prm->d -1 ) / m_prm->d) * m_prm->d;



	m_box = *box;

	IMAGE_DUMP( sim, "PP", 1, NULL );
	image_type *im = image_cut( sim, box->y0, box->x0, box->y1-box->y0+1, box->x1-box->x0+1, NULL );
	IMAGE_DUMP( im, "PP", 1, "b" );

	ret = Process( im, iFrame );

	

	PEDGE_DUMPT( m_gim,box->x0, box->y0, "gim", iFrame, NULL );


	vec2f_type	p;
	p.y = box->y0;
	p.x = box->x0;
	plEdgeA_translate( m_ae, &p );


	image_destroy( im, 1 );

	return( ret );
}



int			
CSlineDetector::Process( image_type *sim, int iFrame )
{


	m_yim = image1_from( sim, m_yim );



	if( m_ae != NULL ){
		plEdgeA_destroy( m_ae );
		m_ae = NULL;
	}

	m_ae = plEdgeA_alloc( 1000 );


	if( m_prm->scale != 1 ){
		m_yim2 = image1_sample2( m_yim, m_yim2 );

		m_gim = pEdge_detector( m_yim2, m_prm, m_gim );

		pEdge_linking( m_gim );

		m_gimS = pEdge_static( m_gimS, m_gim );


		pEdge_line(  m_gim, 0, m_prm, m_ae );

		m_scale = 2;
		plEdgeA_scale( m_ae, 2 );

	

	}
	else	{
		m_gim = pEdge_detector( m_yim, m_prm, m_gim );

		pEdge_linking( m_gim );

//		m_gimS = pEdge_static( m_gimS, m_gim );
		m_gimS = pEdge_staticN( m_gimS, m_gim );


		pEdge_line(  m_gim, 0, m_prm, m_ae );

		m_scale = 1;
	}


//	GPLOG( ("L: %d", m_nL ) );


	IMAGE_DUMP( sim, "m_yim", 1, NULL );
//	PLNA_DUMP( m_apl, "Line", iFrame, NULL );

	if( m_yim == sim )
		m_yim = NULL;

	return( 1 );
}



plnA_type *		
CSlineDetector::Get( int fCopy )
{
	plnA_type *apl;

	apl =  plEdgeA_to_plnA( m_ae, 0.0, 1 );

	return( apl );
}

plnA_type *		
CSlineDetector::Get( int minLen, int fCopy )
{
	plnA_type *apl;

	apl =  plEdgeA_to_plnA( m_ae, minLen, 1 );

	return( apl );
}

void		
CSlineDetector::Write( char *outFile )
{
char	file[256];

	plEdgeA_write( m_ae, outFile );




	gpFilename_force_extension( outFile, ".pt", file );
	pEdge_write_to_file( m_gim, file );
}



