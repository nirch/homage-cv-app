
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include "Uvl/TfType.h"

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "plnTracker/PlnHeadTracker/PlnHeadTracker.h"
#include "MattingContour.h"

#include "UnBackground/bImage.h"

#include "ImageMorphology/ImageMorphology.h"

#include "UnBackground/UnBackground.h"






//#define EXCEPTION





void		bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );



image_type *	bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im );


static void	image1_close1( image_type *sim );




CMattingContour::CMattingContour()
{

	m_headTracking = NULL;


	m_cim = NULL;
	m_cimS = NULL;





	m_cln = NULL;

	m_fpl = plnF_alloc( 1000 );
	m_fplH = plnF_alloc( 1000 );
	m_fplEdge = plnF_alloc( 1000 );

	m_aplEdge = NULL;




	m_prm = mcPrm_alloc();



	gpTime_init( &m_gTime );



	gpTime_init( &m_tCln );

}


CMattingContour::~CMattingContour()
{
	DeleteContents();

}



void CMattingContour::DeleteContents()
{

	if( m_cln != NULL ){
		cln_destroy( m_cln );
		m_cln = NULL;
	}



	if( m_headTracking != NULL ){
		delete m_headTracking;
		m_headTracking = NULL;
	}








	if( m_ac != NULL ){
		clnA_destroy( m_ac );
		m_ac = NULL;
	}
	






	


	if( m_cim != NULL ){
		image_destroy( m_cim, 1 );
		m_cim = NULL;
	}

	if( m_cimS != NULL ){
		image_destroy( m_cimS, 1 );
		m_cimS = NULL;
	}




	if( m_fpl != NULL ){ 

		plnF_destroy( m_fpl );
		m_fpl = NULL;
	}

	if( m_fplH != NULL ){
		plnF_destroy( m_fplH );
		m_fplH = NULL;
	}
	if( m_fplEdge != NULL ){
		plnF_destroy( m_fplEdge );
		m_fplEdge = NULL;
	}
	else 
	if( m_aplEdge != NULL ){
		plnA_destroy( m_aplEdge );
		m_aplEdge = NULL;
	}
	

	

}





int	CMattingContour::Init( char *xmlFile, char *ctrFile, int iHead, int width, int height )
{
	m_mp.x = width/2;
	m_mp.y = height/2;


	m_width = width;
	m_height = height;


	if( xmlFile != NULL && strcmp( xmlFile, "none")  != 0 ){
		if( ReadPrm( xmlFile) < 0 )
			return( -1 );
	}



	if( ctrFile != NULL ){

		if( clnA_read( &m_ac, ctrFile ) < 0 )
			return( -1 );

		InitHeadTracker( 2*iHead+1 );

	}



	return( 1 );
}




int	CMattingContour::Read( char *inFile )
{
	if( plnF_read( &m_fpl, inFile ) < 0 )
		return( -1 );

	return( 1 );
}


int	CMattingContour::Process( int iFrame )
{
	int ret = Process( m_fpl->a[iFrame], iFrame );

	return( ret );
}


int	CMattingContour::Process( plnA_type *apl, int iFrame )
{


	m_iFrame = iFrame;

	gpTime_start( &m_gTime );



#ifdef EXCEPTION1
	try {
#endif

	ProcessHead( apl, iFrame );
			
			
	ProcessAdjust( apl );



	ProcessAlpha( apl );


	gpTime_stop( &m_gTime );


	ProcessLog();

#ifdef EXCEPTION1
	}

	catch (...) {
		return( -1 );
	}
#endif


	return( 1 );
}

int	CMattingContour::ProcessAlpha( plnA_type *apl )
{
	cln_type *cln = cln_from_plnA( apl, 1 );


	if( m_prm->smooth == 0 ){	
		m_cim = image1_mask_cln( cln, m_width, m_height, m_cim );
		image1_binaryM( m_cim, 128, m_cim );
		//		ProcessSmooth();
		m_cimS = image_make_copy( m_cim, m_cimS );
	}
	else {
		m_cim = image1_mask_cln( cln, m_width, m_height, m_cim );
		m_cimS = image_make_copy( m_cim, m_cimS );
	}

	cln_destroy( cln );




	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "SM", m_iFrame, NULL );
		IMAGE_DUMP( m_cimS, "SM", m_iFrame, "SS" );
	}


	return( 1 );
}


int	CMattingContour::Write( char *outFile )
{
	if( m_fpl != NULL )
		plnF_write( m_fpl, outFile );


	char	file[256];
	gpFilename_force_extension( outFile, "-h.plf", file );
	plnF_write( m_fplH, file );

	//gpFilename_force_extension( outFile, "-e.plf", file );
	//plnF_write( m_fplEdge, file );

	if( m_headBoxF == 1 ){

		gpFilename_force_extension( outFile, ".ebox", file );
		box2f_write_from_file( &m_headBox, file );


		pln_type *pl = pln_from_box( &m_headBox );

		gpFilename_force_extension( outFile, "-box.plf", file );
		pln_write_to_file( pl, file );


	}

	return( 1 );
}



void CMattingContour::Trace( FILE *fp )
{
	gpTime_print( fp, "Contour", &m_tCln );

	gpTime_print( fp, "Total", &m_gTime );

}


char * CMattingContour::GetProcessLog()
{

	sprintf( m_processLog, "%d", gpTime_mpf( &m_tCln) );

	return( m_processLog );
}


void CMattingContour::ProcessLog()
{
	gpLog_time( "Contour", &m_tCln );

	gpLog_time( "Total", &m_gTime );




}










image_type *CMattingContour::GetAlpha()
{

	return(m_cimS );
}

