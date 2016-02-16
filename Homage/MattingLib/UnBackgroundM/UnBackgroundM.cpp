
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


#include "UnBackgroundM.h"

#include "UnBackground/bImage.h"







CUnBackgroundM::CUnBackgroundM()
{


	m_nM = 0;
	m_mim[0] = NULL;
	m_mim[1] = NULL;


	m_bim = NULL;
	m_bimDx = NULL;

	m_N = 8;

	m_T = 24;

	m_bT = 8;
	m_bnT = 12;
	



	m_prm = umPrm_alloc();

}


CUnBackgroundM::~CUnBackgroundM()
{
	DeleteContents();

}



void CUnBackgroundM::DeleteContents()
{

	if( m_ac != NULL ){
		clnA_destroy( m_ac );
		m_ac = NULL;
	}
	

	int	i;
	for( i = 0 ; i < m_nM ; i++ ){
		image_destroy( m_mim[i], 1 );
		m_mim[i] = NULL;
	}





	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}

	if( m_bimDx != NULL ){
		image_destroy( m_bimDx, 1 );
		m_bimDx = NULL;
	}
}





int	CUnBackgroundM::Init( char *xmlFile, char *ctrFile, int width, int height )
{
	m_mp.x = width/2;
	m_mp.y = height/2;



	if( xmlFile != NULL && strcmp( xmlFile, "none")  != 0 ){
		if( ReadPrm( xmlFile) < 0 )
			return( -1 );
	}



	if( ctrFile != NULL ){
		if( ReadMask( ctrFile,  width, height ) < 0 )
			return( -1 );
	}


	return( 1 );
}






int	CUnBackgroundM::ReadMask( char *inFile, int width, int height )
{
int	i;

	if( clnA_read( &m_ac, inFile ) < 0 )
		return( -1 );



	for( i = 0, m_nM = 0 ; i < m_ac->nA ; i += 2 ){
		m_mim[m_nM] = image1_mask_cln( m_ac->a[i], width, height, m_mim[m_nM] );
		image1_binaryM( m_mim[m_nM], 128, m_mim[m_nM] );
		image_dump( m_mim[m_nM], "mask", m_nM, NULL );
		m_nM++;
	}






	BOX2D_SET( m_roi, 0,0, width, height );

	return( 1 );
}




int	CUnBackgroundM::Process( image_type *sim )
{

	ProcessInitBackground( sim, m_mim[0] );

	return( 1 );
}



image_type *
	CUnBackgroundM::GetBim( image_type *bim )
{
	bim = bImage_copy( m_bim, bim );
	return( bim );
}

