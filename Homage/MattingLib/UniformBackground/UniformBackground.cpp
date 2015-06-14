
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
#include "UniformBackground.h"

#include "UnBackground/bImage.h"

#include "ImageMorphology/ImageMorphology.h"

#include "UnBackground/UnBackground.h"

#include "EdgeLib/Ridge/RidgeDetector.h"

//#define _DYNAMIC_BACKGROUND

#ifdef _DYNAMIC_BACKGROUND
#include "../DynamicMask/DynamicMask.h"
#endif



//#define EXCEPTION





void		bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );



image_type *	bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im );


static void	image1_close1( image_type *sim );




CUniformBackground::CUniformBackground()
{
	m_unBackground = NULL;
	m_headTracking = NULL;

	m_nM = 0;
	m_mim[0] = NULL;
	m_mim[1] = NULL;

	m_cim = NULL;
	m_cimS = NULL;

	m_dim = NULL;

	m_bim = NULL;
	m_bimDx = NULL;
	m_bimC = NULL;

	m_N = 8;

	m_T = 24;

	m_bT = 8;
	m_bnT = 12;
	
	m_sim = NULL;
	m_yim = NULL;
	m_eim = NULL;
	m_bnIm = NULL;
	m_bnImD = NULL;

	m_cim8 = NULL;


	m_abwBlob = NULL;

	m_abw = NULL;

	m_abwC = NULL;

	m_cln = NULL;

	m_fpl = plnF_alloc( 1000 );
	m_fplH = plnF_alloc( 1000 );

	m_aplEdge = NULL;

	m_contour = 0;

	m_flip = 0;

	m_dr = NULL;

#ifdef _DYNAMIC_BACKGROUND
	m_dm = NULL;
#endif

	m_prm = ubPrm_alloc();

	gpTime_init( &m_rTime );
	gpTime_init( &m_tCompare );
	gpTime_init( &m_tUpdate );
	gpTime_init( &m_gTime );
	gpTime_init( &m_tSmooth );
	gpTime_init( &m_tThin );

	gpTime_init( &m_tBn );
	gpTime_init( &m_tCln );
	gpTime_init( &m_tEdge );

	gpTime_init( &m_tOpen );
	gpTime_init( &m_tBlobR );
	gpTime_init( &m_tBlobF );
	gpTime_init( &m_tBlobEdge );
	gpTime_init( &m_tBlob );
}


CUniformBackground::~CUniformBackground()
{
	DeleteContents();

}



void CUniformBackground::DeleteContents()
{

	if( m_cln != NULL ){
		cln_destroy( m_cln );
		m_cln = NULL;
	}



	if( m_headTracking != NULL ){
		delete m_headTracking;
		m_headTracking = NULL;
	}

	if( m_unBackground != NULL ){
		delete m_unBackground;
		m_unBackground = NULL;
	}


	if( m_dr != NULL ){
		//m_dr->DeleteContents();
		delete m_dr;
		m_dr = NULL;
	}

#ifdef _DYNAMIC_BACKGROUND
	if( m_dm != NULL ){
		delete m_dm;
		m_dm = NULL;
	}
#endif
	

	if( m_ac != NULL ){
		clnA_destroy( m_ac );
		m_ac = NULL;
	}
	

	int	i;
	for( i = 0 ; i < m_nM ; i++ ){
		image_destroy( m_mim[i], 1 );
		m_mim[i] = NULL;
	}

	if( m_sim != NULL ){
		image_destroy( m_sim, 1 );
		m_sim = NULL;
	}

	if( m_yim != NULL ){
		image_destroy( m_yim, 1 );
		m_yim = NULL;
	}

	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}

	if( m_bimC != NULL ){
		image_destroy( m_bimC, 1 );
		m_bimC = NULL;
	}
	

	if( m_dim != NULL ){
		image_destroy( m_dim, 1 );
		m_dim = NULL;
	}

	if( m_cim != NULL ){
		image_destroy( m_cim, 1 );
		m_cim = NULL;
	}

	if( m_cimS != NULL ){
		image_destroy( m_cimS, 1 );
		m_cimS = NULL;
	}

	if( m_bnIm != NULL ){
		image_destroy( m_bnIm, 1 );
		m_bnIm = NULL;
	}

	if( m_bnImD != NULL ){
		image_destroy( m_bnImD, 1 );
		m_bnImD = NULL;
	}

	if( m_eim != NULL ){
		image_destroy( m_eim, 1 );
		m_eim = NULL;
	}

	if( m_abwBlob != NULL ){
		imageLabel_destroy( m_abwBlob );
		m_abwBlob = NULL;
	}


	if( m_abw != NULL ){
		imageLabel_destroy( m_abw );
		m_abw = NULL;
	}

	if( m_abwC != NULL ){
		imageLabel_destroy( m_abwC );
		m_abwC = NULL;
	}



	if( m_fpl != NULL ){
		plnF_destroy( m_fpl );
		m_fpl = NULL;
	}

	if( m_fplH != NULL ){
		plnF_destroy( m_fplH );
		m_fplH = NULL;
	}
	
	if( m_aplEdge != NULL ){
		plnA_destroy( m_aplEdge );
		m_aplEdge = NULL;
	}
	

	

}



void	CUniformBackground::SetRoi( box2i_type *b )
{
	m_roi = *b;
}

int	CUniformBackground::Init( char *xmlFile, char *ctrFile, int width, int height )
{
	m_mp.x = width/2;
	m_mp.y = height/2;


	m_unBackground = new CUnBackground();

	if( m_unBackground->Init( xmlFile, ctrFile, width, height) < 0 )
		return( -1 );


	if( xmlFile != NULL && strcmp( xmlFile, "none")  != 0 ){
		if( ReadPrm( xmlFile) < 0 )
			return( -1 );
	}



	if( ctrFile != NULL ){
		if( ReadMask( ctrFile,  width, height ) < 0 )
			return( -1 );
	}

	if( m_contour > 0 )
		m_prm->contour = m_contour;

	ProcessEdgeContourInit();



	return( 1 );
}


int	CUniformBackground::InitHeadTracker( int iHead )
{
	cln_type *cln = clnA_detach( m_ac, iHead );
	if ( cln != NULL ){


		if( m_headTracking == NULL )
			m_headTracking = new CPlnHeadTracker();
		m_headTracking->Init( cln );
	}

	return( 1 );
}


void CUniformBackground::SetContour( int contour )
{
	if( contour <= 0 )	return;

	if( m_prm != NULL ){
		m_prm->contour = contour;
	}

	m_contour = contour;
}




int	CUniformBackground::ReadMask( char *inFile, int width, int height )
{
int	i;

	if( clnA_read( &m_ac, inFile ) < 0 )
		return( -1 );



	for( i = 0, m_nM = 0 ; i < m_ac->nA ; i += 2 ){
		m_mim[m_nM] = image1_mask_cln( m_ac->a[i], width, height, 1, m_mim[m_nM] );
		image_dump( m_mim[m_nM], "mask", m_nM, NULL );
		m_nM++;
	}






	BOX2D_SET( m_roi, 0,0, width, height );

	return( 1 );
}



int	CUniformBackground::ResetBackground()
{
	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}

	return( 1 );
}


int	CUniformBackground::Process( image_type *sim, int iFrame, image_type **cim )
{


	m_iFrame = iFrame;

	gpTime_start( &m_gTime );



#ifdef EXCEPTION1
	try {
#endif

	if( m_flip == 1 )
		m_sim = image3_rotate180( sim, m_sim );
	else m_sim = image_make_copy( sim, m_sim );
	

	m_yim = image1_from( m_sim, m_yim );

	if( m_bim == NULL ){
		ProcessInitBackground( m_sim );

		InitHeadTracker( m_iHead );

		m_headBoxF = 0;


#ifdef _DYNAMIC_BACKGROUND
		ProcessDynamicMaskInit();
#endif
	}



	m_yim = image1_from( m_sim, m_yim );

	ProcessCompare( m_sim );


#ifdef _DYNAMIC_BACKGROUND
	ProcessDynamicMask();
#endif



	ProcessThin( 8 );

	ProcessSmooth();

//	*cim = m_cim;
	if( cim != NULL )
		*cim = m_cimS;


	ProcessContour();


	ProcessBn( m_sim, m_bnT );



	gpTime_start( &m_tUpdate );
	ProcessUpdate( m_sim );
	gpTime_stop( &m_tUpdate );



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



int	CUniformBackground::ProcessCompare( image_type *sim )
{

	gpTime_start( &m_tCompare );




	m_dim = bImage_diff( sim,  &m_roi, m_N, m_bim, m_T, m_dim );


	m_cim = image1_binaryM( m_dim, m_T, m_cim );



	if( m_iFrame == m_dFrame ){
		bImage_dump( m_bim, m_N, "bim", m_iFrame, "B" );
		IMAGE_DUMP( sim, "m", m_iFrame, "SIM" );
		IMAGE_DUMP( m_cim, "m", m_iFrame, "1" );
	}


	gpTime_start( &m_tOpen );
	image1_open( m_cim, 1, 0 );
//	image1_close( m_cim, 1, 255 );
	gpTime_stop( &m_tOpen );


	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "2" );
	}


//	ProcessBlob();

//	ProcessBlob1();

	ProcessBlob2();




	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "3" );
	}




	//image1_close1( m_cim );

	//if( m_iFrame == m_dFrame ){
	//	IMAGE_DUMP( m_cim, "m", m_iFrame, "4" );
	//}


	gpTime_stop( &m_tCompare );


	return( 1 );
}



int	CUniformBackground::Write( char *outFile )
{
	plnF_write( m_fpl, outFile );


	char	file[256];
	gpFilename_force_extension( outFile, "-h.plf", file );
	plnF_write( m_fplH, file );


	if( m_headBoxF == 1 ){

		gpFilename_force_extension( outFile, ".ebox", file );
		box2f_write_from_file( &m_headBox, file );


		pln_type *pl = pln_from_box( &m_headBox );

		gpFilename_force_extension( outFile, "-box.plf", file );
		pln_write_to_file( pl, file );


		DynamicPose( outFile );

	}

	return( 1 );
}



void CUniformBackground::Trace( FILE *fp )
{
	gpTime_print( fp, "Calibrate", &m_rTime );
	gpTime_print( fp, "Compare", &m_tCompare );
	gpTime_print( fp, "Thin", &m_tThin );
	gpTime_print( fp, "Smooth", &m_tSmooth );

	gpTime_print( fp, "Bn-dd", &m_tBn );
	gpTime_print( fp, "Update", &m_tUpdate );
	gpTime_print( fp, "Edge", &m_tEdge );
	gpTime_print( fp, "Contour", &m_tCln );

	gpTime_print( fp, "Total", &m_gTime );

	if( m_dr != NULL )
		m_dr->Trace( fp );



	//gpTime_print( fp, "Compare-BlobR", &m_tBlobR );
	//gpTime_print( fp, "Compare-BlobF", &m_tBlobF );
	//gpTime_print( fp, "Compare-BlobEdge", &m_tBlobEdge );
	//gpTime_print( fp, "Compare-Open", &m_tOpen );
	//gpTime_print( fp, "Compare-Blob", &m_tBlob );
}


char * CUniformBackground::GetProcessLog()
{

	sprintf( m_processLog, "%d  %d  %d %d", gpTime_mpf( &m_tCompare), gpTime_mpf( &m_tThin),  gpTime_mpf( &m_tSmooth), gpTime_mpf( &m_tUpdate) );

	return( m_processLog );
}


void CUniformBackground::ProcessLog()
{

	if( m_tCompare.no == 0 || (m_tCompare.no%10) != 0 )
		return;

	int compare = gpTime_mpf( &m_tCompare);
	int nn = gpTime_mpf( &m_tThin);
	int g = gpTime_mpf( &m_tSmooth);
	int update = gpTime_mpf( &m_tUpdate);

}



int	CUniformBackground::ProcessBn( image_type *sim, float bnT )
{

	gpTime_start( &m_tBn );

//	m_yim = image1_from( sim, m_yim );
	m_bnIm = bnImage_average4( m_yim, m_bnIm );

	m_bnImD = bnImage_dd( m_bnIm, bnT, m_bnImD );

	gpTime_stop( &m_tBn );

	return( 1 );
}





image_type *
bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im )
{
	int	i,	j,	k,	n,	i0;
	u_char	*sp,	*tp;
	bImage_type *bp,	*bp0;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= sim->height )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );
			tp = IMAGE_PIXEL( im, i0, b->x0 );


			for( j = 1, bp = bp0 ; j < bim->width-1 ; j++, bp++ ){
				for( n = 0 ; n < N ; n++, tp++ ){
	
					int dr = *sp++ - bp->r;
					int dg = *sp++ - bp->g;
					int db = *sp++ - bp->b;

					int d = ABS(dr);
					//d *= 1.2;
					if( dg < 0 )	dg = -dg;
					if( d < dg )	d = dg;

					if( db < 0 )	db = -db;
					if( d < db )	d = db;


					//if( d < T )
					//	*tp = 0;
					//else *tp = 255;

					*tp = d;
				}
			}
		}

	}

	return( im );
}





static void
image1_close1( image_type *sim )
{

	int	i,	j;



	u_char *sp = IMAGE_PIXEL( sim, 1, 1 );

	for( i = 1 ; i < sim->height-1 ; i++, sp += 2 ){
		u_char *sp0 = sp - sim->width;
		u_char *sp1 = sp + sim->width;

		for( j = 1 ; j < sim->width-1 ; j ++, sp++, sp0++, sp1++ ){

			if( *sp != 0 )	continue;

			int i1 =  ( *(sp0-1) + *(sp0) + *(sp0+1) + 
				*(sp-1) + *(sp+1) +
				*(sp1-1) + *(sp1) + *(sp1+1) );

			if( i1 >= 255 * 7 )
				*sp = 255;

			
		}
	}
}




image_type *CUniformBackground::GetImage(  int color, image_type *im )
{
	im = imageA_set_colorN( m_sim, m_cimS, color, im );

	return( im );
}


image_type *CUniformBackground::GetImage(  image_type *bim, image_type *im )
{
	im = imageA_set_backgorund( m_sim, m_cimS, bim, im );

	return( im );
}

image_type *CUniformBackground::GetImageAlpha( image_type *im )
{
	im = imageA_set_alpha( m_sim, 255, m_cimS, im );

	return( im );
}

int CUniformBackground::ProcessPl(  image_type *sim, int iFrame, plnA_type *apl )
{
	m_iFrame = iFrame;

	if( m_flip == 1 )
		m_sim = image3_rotate180( sim, m_sim );
	else m_sim = image_make_copy( sim, m_sim );



	cln_type *cln = cln_from_plnA( apl, 1 );


	m_cimS = image1_mask_cln( cln, sim->width, sim->height, 0, m_cimS );

	cln_destroy( cln );

	return( 1 );
}



int	CUniformBackground::DynamicPose( char *outFile )
{
	if( m_headBoxF == 0 )
		return( -1 );

	box2f_type	b;

	float x0 = 0.5*(m_headBox.x0 + m_headBox.x1 );
	float y0 = 0.5*(m_headBox.y0 + m_headBox.y1 );


	b.x0 = m_headBox.x0 - x0 + 240;
	b.x1 = m_headBox.x1 - x0 + 240;

	b.y0 = m_headBox.y0 - y0 + 160;
	b.y1 = m_headBox.y1 - y0 + 160;


	lt2_type lt;

	lt.c0 = 240 - y0; 
	lt.c1 = 160 - x0;



	tf_type *tf = tf_alloc( 6 );
	tf->a[0] = lt.c0;
	tf->a[1] = lt.c1;
	tf->a[2] = 1.0;

	tf->a[3] = 0;
	tf->a[4] = 0;
	tf->a[5] = 0;


	tfA_type *atf = tfA_alloc( 1 );
	tfA_add( atf, 0, tf );
	
	char file[256];
	gpFilename_force_extension( outFile, "-box.tf", file );
	tfA_write( atf, 1, file );

	return( 1 );
}