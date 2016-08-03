//  Defines the entry point for the console application.
//
#include <stdio.h>
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif


#ifndef __ANDROID__
#define EXCEPTION
#endif


#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageMorphology/ImageMorphology.h"
#include "Imagelabel/BwLabel.h"

#define DIFF_THRESH 16// 12


#include	"UnBackground.h"

#include	"BnImage/BnImage.h"

#include	 "../UnEdge/UnEdge.h"

#include	 "ImageLabel/ImageLabel.h"


#include	 "bImage.h"




int	image1_mask_sbA( image_type *im, sbA_type *as );



CUnBackground::CUnBackground()
{
	m_yim = NULL;
	m_bim = NULL;

	m_bimD = NULL;

	m_N = 8;

	m_abw = NULL;

	m_prm = NULL;


	m_ac = NULL;

	m_apt = NULL;
	m_imMask = NULL;


	m_ue = NULL;


	m_closeUp = 0;

	gpTime_init( &m_gTime );
	gpTime_init( &m_tEdge );
}

CUnBackground::~CUnBackground()
{

	if( m_ac != NULL ){
		clnA_destroy( m_ac );
		m_ac = NULL;
	}


	if( m_yim != NULL ){
		image_destroy( m_yim, 1 );
		m_yim = NULL;
	}

	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}



	if( m_bimD != NULL ){
		image_destroy( m_bimD, 1 );
		m_bimD = NULL;
	}


	int i;
	for( i = 0 ; i < m_nM ; i++ ){
		image_destroy( m_mim[i], 1 );
		m_mim[i] = NULL;

	}

	if( m_abw != NULL ){
		imageLabel_destroy( m_abw );
		m_abw = NULL;
	}


	if( m_apt != NULL ){
		pt2dA_destroy( m_apt );
		m_apt = NULL;
	}

	if( m_imMask != NULL ){
		image_destroy( m_imMask, 1 );
		m_imMask = NULL;
	}


	if( m_ue != NULL ){
		delete m_ue;
		m_ue = NULL;
	}
}


int	CUnBackground::Init( char *prmFile, char *ctrFile, int width, int height )
{

	m_roi.x0 = 0;
	m_roi.x1 = width;
	m_roi.y0 = 0;
	m_roi.y1 = height;




	if( prmFile != NULL && strcmp( prmFile, "none")  != 0 ){
		if( ReadPrm( prmFile) < 0 )
			return( -1 );
	}
	else m_prm = unPrm_alloc();



	if( ctrFile != NULL ){
		if( ReadMask( ctrFile,  width, height ) < 0 )
			return( -1 );
	}


	ProcessLineInit( prmFile );


	return( 1 );
}


int	CUnBackground::ReadMask( char *inFile, int width, int height )
{
	int	i;

	if( clnA_read( &m_ac, inFile ) < 0 )
		return( -1 );


	if( m_ac->nA == 1 ){
		gpWarning( "UnBackground::ReadMask", "There is no head contour" );
		return( -1 );
	}


	for( i = 0, m_nM = 0 ; i < m_ac->nA ; i += 2 ){
		image_type *im = image1_mask_cln( m_ac->a[i], width, height, NULL );
		image1_binaryM( im, 128, im );
		image_dump( im, "mask", m_nM, NULL );

		m_mim[m_nM] = image1_sample( im, 8, NULL );
		IMAGE_DUMP_DUP( m_mim[m_nM], 8, 1.0, "mask", m_nM, "DUP" );
		image1_threshold( m_mim[m_nM], 255 );

		image_destroy( im, 1 );

		image1_mask_sbA( m_mim[m_nM], &m_asB[m_nM] );



		m_nM++;
	}






	m_hp = m_ac->a[1]->p;
	m_hd[0] = 80;

	
	for( i = 0 ; 2*i+1 < m_ac->nA ; i++ ){
		cln_box( m_ac->a[2*i+1], &m_hbox[i] );
		m_hd[i] = 0.5*(m_hbox[i].x1 - m_hbox[i].x0 );
	}



	BOX2D_SET( m_roi, 0,0, width, height );

	return( 1 );
}

image_type *
CUnBackground::GetBim( image_type *bim )
{
	bim = bImage_copy( m_bim, bim );
	return( bim );
}

// Receives the a frame (sim) and creates the background estimation
int CUnBackground::Process( image_type *sim, int iFrame )
{
int	ret;


#ifdef EXCEPTION
	try {
#endif

	m_state = 0;


	iFrame = m_iFrame;

	GPLOG(("CUnBackground: %d %d %d %d", iFrame, sim->width, sim->height, sim->channel ) );

	IMAGE_DUMP( sim, "ub", 1, "SRC" );	

	gpTime_start( &m_gTime );

    // Greylevel image from the given frame
	m_yim = image1_from( sim, m_yim );

    // First step - taking the current frame and creating a 8X8 pixel of it
	m_bim = bImage_set( sim, &m_roi, m_N, m_bim );

	BIMAGE_DUMP( m_bim, m_N, "ub", 1, "A" );
    
    // Edge image of 8X8 pixels (only X derivative)
	m_bimD = bImage_grad_c( m_bim, m_bimD );
        
#ifdef _DUMP
	image_type *im = image1S_axb(  m_bimD, 1, 128, NULL );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "D" );
	image_destroy( im, 1 );
#endif
	
    // Creating blobs from Edge gradiant image
	ret = ProcessBlob( m_bimD );

    // Decision like who is the main blob what is the background blob
	ret = ProcessBlobB();

    // Calculating weight for each of the silhouettes (will use this weight later on).
	ProcessSilhouette();

    // Finding stright lines and choosing what is the background we shoud use to replace the area of the user
    // -1 is returned if the head was not found
	ret = ProcessBlobC();


	// If we didn't find a head we are using a different approach to estimate the background - based on the contour
	if( ret < 0 ){
		if( TestDrakness( m_yim ) > 0 )
			m_state = -10;
		else
			m_state = -11;
		int m = m_bim->width-3;

		sbA_set_from_contour( &m_asB[0], &m_as0, &m_as1, m );

		m_closeUp = 1;

		GPTRACE( (3, "Background: %d\n", m_state ) );
	}



#ifdef _DEBUG
	sbA_write3( &m_as0, &m_asB[0], &m_as1, stdout );
#endif
	
	bImage_set_sbA( m_bim, &m_as0 );
	bImage_set_sbA( m_bim, &m_as1 );

	bImage_set_sbA_sideR( m_bim, &m_as0, 10 );
	bImage_set_sbA_sideL( m_bim, &m_as1, 10 );


#ifdef _DEBUG
	sbA_write3( &m_as0, &m_asB[0], &m_as1, stdout );
#endif
	
    // Special case for close-up. The user most likely is touching the end of the frame on the x axis so we want to take the background color from the y axis instead
	if( m_closeUp )
		ProcessCloseUp();


#ifdef _DEBUG
	sbA_write3( &m_as0, &m_asB[0], &m_as1, stdout );
#endif

    // Filling the foreground area with an estimated background
	ProcessFill();

	if( m_state >= 0 )
		ProcessState();


	m_head.p.x = 11*8;
	m_head.p.y = 0.5*8*(m_as1.a[11].j0 + m_as0.a[11].j1);
	float h = 8*(m_asB[m_iHead].a[11].j1 - m_asB[m_iHead].a[11].j0);
	m_head.scale = 8*(m_as1.a[11].j0 - m_as0.a[11].j1-2) / h;
	m_head.qulity = 1;


	GPTRACE( (3, "Background: %d\n", m_state ) );


#ifdef _DYNAMIC
	ProcessBlobD();
#endif


	gpTime_stop( &m_gTime );

#ifdef EXCEPTION
	}

	catch (...) {
		m_state = -100;
		return( -1 );
	}
#endif

	return( 1 );
}



#ifdef _AA_
int	bImage_fill( image_type *bim, int a0[], int a1[] );


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

	bImage_fill( m_bim, a0, a1 );

	BIMAGE_DUMP( m_bim, m_N, "un", 1, "F" );



	for( i = 0 ; i < m_as0.nA ; i++ ){
		m_asFigure.a[i].j0 = a0[i];
		m_asFigure.a[i].j1 = a1[i];
	}
	m_asFigure.nA = m_as0.nA;

	return( 1 );
}
#endif


int image1_mask_sbA( image_type *im, sbA_type *as )
{
	int	i,	j,	no;
	

	no = 0;


	for( i = 0 ; i < im->height ; i++ ){
		u_char *bp0 = IMAGE_PIXEL( im, i, 0 );
		u_char *bp1 = bp0 + im->width-1;

		u_char *bp;
		for( j = 0, bp = bp0 ; j < im->width ; j++, bp++ )
			if(*bp > 0 )		break;

		if( j >= im->width ){
			as->a[i].j0 = as->a[i].j1 = -1;
			continue;
		}

		if( j == 0 )
			as->a[i].j0 = 0;
		else as->a[i].j0 = j-1;


		
		for( j = im->width-1, bp = bp1 ; j >= 0 ; j--, bp-- )
			if( *bp > 0)		break;


		if( j ==  im->width-1 )	
			as->a[i].j1 = j;//-1;
		else as->a[i].j1 = j+1;
	}

	as->nA = im->height;

	return( im->height );
}





