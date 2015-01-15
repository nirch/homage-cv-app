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


int	image1_mask_sbA( image_type *im, sbA_type *as );




CUnBackground::CUnBackground()
{
	m_yim = NULL;
	m_bim = NULL;

	m_bimD = NULL;

	m_N = 8;

	m_abw = NULL;

	m_prm = NULL;

	m_bimDx = NULL;

	gpTime_init( &m_gTime );
}

CUnBackground::~CUnBackground()
{

}


int	CUnBackground::Init( char *xmlFile, char *ctrFile, int width, int height )
{

	m_roi.x0 = 0;
	m_roi.x1 = width;
	m_roi.y0 = 0;
	m_roi.y1 = height;




	if( xmlFile != NULL && strcmp( xmlFile, "none")  != 0 ){
		if( ReadPrm( xmlFile) < 0 )
			return( -1 );
	}
	else m_prm = unPrm_alloc();



	if( ctrFile != NULL ){
		if( ReadMask( ctrFile,  width, height ) < 0 )
			return( -1 );
	}






	return( 1 );
}


int	CUnBackground::ReadMask( char *inFile, int width, int height )
{
	int	i;

	if( clnA_read( &m_ac, inFile ) < 0 )
		return( -1 );



	for( i = 0, m_nM = 0 ; i < m_ac->nA ; i += 2 ){
		image_type *im = image1_mask_cln( m_ac->a[i], width, height, 1, NULL );
		image_dump( im, "mask", m_nM, NULL );

		m_mim[m_nM] = image1_sample( im, 8, NULL );
		image1_threshold( m_mim[m_nM], 255 );

		image_destroy( im, 1 );

		image1_mask_sbA( m_mim[m_nM], &m_asB[m_nM] );



		m_nM++;
	}






	m_hp = m_ac->a[1]->ctr;
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

int
CUnBackground::Process( image_type *sim, int iFrame )
{
int	ret;

	iFrame = m_iFrame;


	IMAGE_DUMP( sim, "ub", 1, "SRC" );	

	gpTime_start( &m_gTime );


	m_yim = image1_from( sim, m_yim );


	m_bim = bImage_set( sim, &m_roi, m_N, m_bim );





	BIMAGE_DUMP( m_bim, m_N, "ub", 1, "A" );	



	m_bimD = bImage_grad_c( m_bim, m_bimD );
	image_type *im = image1S_axb(  m_bimD, 1, 128, NULL );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "D" );
	image_destroy( im, 1 );
	



	ret = ProcessBlob( m_bimD );

	ret = ProcessBlobB();

	ProcessSilhouette();

	ret = ProcessBlobC();
	if( ret < 0 ){

		if( TestDrakness( m_yim ) > 0 )
			m_state = -10;
		else
			m_state = -11;

		fprintf( stdout, "State: %d\n", m_state );
		return( -1 );
	}


	
	bImage_set_sbA( m_bim, &m_as0 );
	bImage_set_sbA( m_bim, &m_as1 );

	bImage_set_sbA_sideR( m_bim, &m_as0, 10 );
	bImage_set_sbA_sideL( m_bim, &m_as1, 10 );


#ifdef _DEBUG
	sbA_write3( &m_as0, &m_asB[0], &m_as1, stdout );
#endif


	ProcessFill();

	ProcessState();

	fprintf( stdout, "State: %d\n", m_state );


	gpTime_stop( &m_gTime );

	return( 1 );
}




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


	bImage_fill_gap( m_bim, a0, F_LEFT );
	bImage_fill_gap( m_bim, a1, F_RIGHT );


	bImage_fill( m_bim, a0, a1 );

	BIMAGE_DUMP( m_bim, m_N, "un", 1, "F" );

	return( 1 );
}



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





