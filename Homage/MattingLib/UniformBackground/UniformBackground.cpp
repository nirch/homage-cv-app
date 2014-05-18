
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"

#include "ImageMorphology/ImageMorphology.h"


#define EXCEPTION





void		bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );



image_type *	bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im );

static void	image1_close1( image_type *sim );




CUniformBackground::CUniformBackground()
{

	m_mim = NULL;

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

	m_contour = 0;

	m_flip = 0;

	m_prm = ubPrm_alloc();

	gpTime_init( &m_rTime );
	gpTime_init( &m_tCompare );
	gpTime_init( &m_tUpdate );
	gpTime_init( &m_gTime );
	gpTime_init( &m_tSmooth );
	gpTime_init( &m_tThin );

	gpTime_init( &m_tBn );
	gpTime_init( &m_tCln );


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
}



void	CUniformBackground::SetRoi( box2i_type *b )
{
	m_roi = *b;
}

int	CUniformBackground::Init( char *xmlFile, char *ctrFile, int width, int height )
{

	if( ReadMask( ctrFile,  width, height ) < 0 )
		return( -1 );

	if( ReadPrm( xmlFile) < 0 )
		return( -1 );

	return( 1 );
}


int	CUniformBackground::ReadMask( char *inFile, int width, int height )
{
	cln_type *cln;
	if( cln_read( &cln, inFile ) < 0 )
		return( -1 );

	m_mim = image1_mask_cln( cln, width, height, NULL );

	cln_destroy( cln );

	image_dump( m_mim, "mask", 1, NULL );


	BOX2D_SET( m_roi, 0,0, width, height );

	return( 1 );
}




int	CUniformBackground::Process( image_type *sim, int iFrame, image_type **cim )
{
	m_iFrame = iFrame;

	gpTime_start( &m_gTime );


#ifdef EXCEPTION
	try {
#endif

	if( m_flip == 1 )
		image_flipV( sim );


	if( m_bim == NULL ){
		ProcessInitBackground( sim, m_mim );
	}



	m_yim = image1_from( sim, m_yim );

	ProcessCompare( sim, cim );



	ProcessThin( 8 );

	ProcessSmooth();

//	*cim = m_cim;
	*cim = m_cimS;


	ProcessContour();


	ProcessBn( sim, m_bnT );



	gpTime_start( &m_tUpdate );
	ProcessUpdate( sim );
	gpTime_stop( &m_tUpdate );



	gpTime_stop( &m_gTime );


	ProcessLog();

#ifdef EXCEPTION
	}

	catch (...) {
		return( -1 );
	}
#endif


	return( 1 );
}



int	CUniformBackground::ProcessCompare( image_type *sim, image_type **cim )
{

	gpTime_start( &m_tCompare );


//	m_cim = image_realloc( m_cim, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	m_dim = bImage_diff( sim,  &m_roi, m_N, m_bim, m_T, m_dim );

//	m_cim = bImage_diff( sim,  &m_roi, m_N, m_bim, m_T, m_cim );
	m_cim = image1_binaryM( m_dim, m_T, m_cim );



	if( m_iFrame == m_dFrame ){
		bImage_dump( m_bim, m_N, "bim", m_iFrame, "B" );
		IMAGE_DUMP( sim, "m", m_iFrame, "SIM" );
		IMAGE_DUMP( m_cim, "m", m_iFrame, "1" );
	}



//	ProcessBlob();

//	ProcessBlob1();

	ProcessBlob2();




	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "4" );
	}


	gpTime_start( &m_tOpen );
	image1_open( m_cim, 1, 0 );
	gpTime_stop( &m_tOpen );


	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "m", m_iFrame, "2" );
	}

	//image1_close1( m_cim );

	//if( m_iFrame == m_dFrame ){
	//	IMAGE_DUMP( m_cim, "m", m_iFrame, "3" );
	//}


	gpTime_stop( &m_tCompare );


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
	gpTime_print( fp, "Contour", &m_tCln );

	gpTime_print( fp, "UniformBackground", &m_gTime );



	gpTime_print( fp, "Compare-BlobR", &m_tBlobR );
	gpTime_print( fp, "Compare-BlobF", &m_tBlobF );
	gpTime_print( fp, "Compare-BlobEdge", &m_tBlobEdge );
	gpTime_print( fp, "Compare-Open", &m_tOpen );

	gpTime_print( fp, "Compare-Blob", &m_tBlob );
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




