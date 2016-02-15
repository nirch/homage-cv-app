//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif



#define _GPLOG

#include "Ulog/Log.h"
#include "Uvl/TfType.h"
#include "Uln/PlnType.h"

#include "Umath/Matrix3Type.h"
#include "Umath/Matrix2Type.h"


#include "ImageType/ImageType.h"

#include "ImageDump/ImageDump.h"


//#include "Png/PngCodec/Png/ImageReadPng.h"
#include "ImageMbt/ImageMbt.h"



#include "HrEffectMorphing.h"
#include "MorphingPlnMap.h"




CHrEffectMorphing::CHrEffectMorphing()
{
	m_id = EFFECT_MORPHING;

	m_im = NULL;

	m_sim = NULL;
	m_apl = NULL;

	m_imT = NULL;
	m_aplT = NULL;


	m_apt = NULL;
	m_aptT = NULL;

	m_atf = NULL;

	m_mbt = NULL;
	m_mbtI = NULL;

	m_width = -1;
	m_height = -1;

}

CHrEffectMorphing::~CHrEffectMorphing()
{
	DeleteContents();
}





void CHrEffectMorphing::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	if( m_apl != NULL ){
		plnA_destroy( m_apl );
		m_apl = NULL;
	}

	//if( m_imT != NULL ){
	//	image_destroy( m_imT, 1 );
	//	m_imT = NULL;
	//}
	image_destroy( m_imT, 1 );

	if( m_aplT != NULL ){
		plnA_destroy( m_aplT );
		m_aplT = NULL;
	}


	if( m_apt != NULL ){
		pt2dA_destroy( m_apt );
		m_apt = NULL;
	}

	if( m_aptT != NULL ){
		pt2dA_destroy( m_aptT );
		m_aptT = NULL;
	}

	if( m_atf != NULL ){
		tfA_destroy( m_atf );
		m_atf = NULL;
	}


	image_destroy( m_mbt, 1 );
	image_destroy( m_mbtI, 1 );

}

//int CHrEffectMorphing::Init( char *splFile,  char *tFile, char *plnFile, char *tfFile )
//{
//	
//	if( plnA_read( &m_apl, splFile ) < 0 )
//		return( -1 );
//
//	m_p0 = m_apl->p;
//
//
//	if( (m_imT = image_read_png_file( tFile)) == NULL ) 
//		return( -1 );
//
//	//IMAGE_DUMP_ALPHA( m_imT, "AA", 0, "SS_A" );
//
//
//	imageT_negative_alpha( m_imT, m_imT );
//	//IMAGE_DUMP( m_imT, "AA", 0, "SSN" );
//	//IMAGE_DUMP_ALPHA( m_imT, "AA", 0, "SSNA1" );
//
//
//	if( plnA_read( &m_aplT, plnFile ) < 0 )
//		return( -1 );
//	pln_interior_force_right( m_aplT->a[0] );
//
//
//	m_apt = morohing_pln_map( m_apl, m_aplT, 480, 480, 16, 0, NULL );
//
//	m_aptT = morohing_pln_map( m_aplT, m_apl, 480, 480, 16, 0, NULL );
//
//
//
//	//m_mbt = image_mbt_create( m_imT->width, m_imT->height, 16 );
//	//image_mbt_transparency( mbt, 16, 0, 0, sim );
//
//
//	m_dp0.x = -(m_apl->p.x - m_aplT->p.x);
//	m_dp0.y = -(m_apl->p.y - m_aplT->p.y);
//	m_mbtI = image_mbt_create( m_imT->width, m_imT->height, 16 );
//	//image_mbt_transparency( m_mbtI, 16, -m_dp0.y, -m_dp0.x, m_imT );
//	image_mbt_transparency( m_mbtI, 16, 0, 0, m_imT );
//
//
//	if( tfA_read( &m_atf, tfFile ) < 0 )
//		return( -1 );
//
//
//
//
//	m_iFrame = -1;
//	return( 1 );
//}


int CHrEffectMorphing::Init( char *splFile,  image_type *tim, char *plnFile, char *tfFile )
{
    
    if( plnA_read( &m_apl, splFile ) < 0 )
        return( -1 );
    
    m_p0 = m_apl->p;
    
    m_imT = image_make_copy(tim, m_imT);
//    if( (m_imT = image_read_png_file( tFile)) == NULL )
//        return( -1 );
    
    //IMAGE_DUMP_ALPHA( m_imT, "AA", 0, "SS_A" );
    
    
    imageT_negative_alpha( m_imT, m_imT );
    //IMAGE_DUMP( m_imT, "AA", 0, "SSN" );
    //IMAGE_DUMP_ALPHA( m_imT, "AA", 0, "SSNA1" );
    
    
    if( plnA_read( &m_aplT, plnFile ) < 0 )
        return( -1 );
    pln_interior_force_right( m_aplT->a[0] );
    
    
    m_apt = morohing_pln_map( m_apl, m_aplT, 480, 480, 16, 0, NULL );
    
    m_aptT = morohing_pln_map( m_aplT, m_apl, 480, 480, 16, 0, NULL );
    
    
    
    //m_mbt = image_mbt_create( m_imT->width, m_imT->height, 16 );
    //image_mbt_transparency( mbt, 16, 0, 0, sim );
    
    
    m_dp0.x = -(m_apl->p.x - m_aplT->p.x);
    m_dp0.y = -(m_apl->p.y - m_aplT->p.y);
    m_mbtI = image_mbt_create( m_imT->width, m_imT->height, 16 );
    //image_mbt_transparency( m_mbtI, 16, -m_dp0.y, -m_dp0.x, m_imT );
    image_mbt_transparency( m_mbtI, 16, 0, 0, m_imT );
    
    
    if( tfA_read( &m_atf, tfFile ) < 0 )
        return( -1 );
    
    
    
    
    m_iFrame = -1;
    return( 1 );
}



int CHrEffectMorphing::InitFromData( char *data )
{
	char	buf[10001];
	strncpy( buf, data, 1000 );
	GPLOG( ("%s\n", buf));

	if( tfA_read_from_data( &m_atf, data ) < 0 )
		return( -1 );

	m_iFrame = -1;
	return( 1 );
}



int	CHrEffectMorphing::Process( image_type *sim, int iFrame, image_type **im ) 
{


	float t,	w;
	GetPose( m_atf, iFrame, &t, &w );

	fprintf( stdout, "%f %f\n", t, w );



	if( m_mbt == NULL ){
		m_mbt = image_mbt_create( m_imT->width, m_imT->height, 16 );
		image_mbt_transparency( m_mbt, 16, 0, 0, sim );
		m_sim = image_make_copy( sim, m_sim );
	}


	
	pt2dA_type *capt;
	capt = morohing_pln_map_step( m_apt, &m_p0, t, NULL );



//	pt2dA_dump_m( capt, "MOR", iFrame, "0" );
	image_mbt_set( m_mbt, capt, 0 );

	image_type *im0 = image_mbt( sim, m_mbt, 16, NULL, NULL );
	IMAGE_DUMP( im0, "AA", iFrame, "0" );
	IMAGE_DUMP_ALPHA( im0, "AA", iFrame, "0A" );


	
	capt = morohing_pln_map_step( m_aptT, &m_p0, 1-t, capt );
//	pt2dA_dump_m( capt, "MOR", iFrame, "1" );
	image_mbt_set( m_mbtI, capt, 0 );

///	image_type *im1 = image_mbt( m_imT, m_mbtI, 16, &m_dp0, NULL );
	image_type *im1 = image_mbt( m_imT, m_mbtI, 16,NULL, NULL );
	IMAGE_DUMP( im1, "AA", iFrame, "1" );
	IMAGE_DUMP_ALPHA( im1, "Aa", iFrame, "1A" );


	
	m_im = image_fusion(  im0, im1, w, m_im );
	IMAGE_DUMP( m_im, "AA", iFrame, "F" );
	IMAGE_DUMP_ALPHA( m_im, "BB", iFrame, "FA" );

	//char file[256];
	//sprintf( file, "%s-%.2d.png", outFile, i );
	//image_write_png_TA( m_im, file );


	pt2dA_destroy( capt );
	image_destroy( im0, 1 );
	image_destroy( im1, 1 );

	m_iFrame = iFrame;

	*im = m_im;


	return( 1 );
}



int	CHrEffectMorphing::GetPose( tfA_type *atf, int iFrame, float *T, float *a )
{


	tf_type *tf0 = tfA_get_prev( atf, iFrame+1 );
	tf_type *tf1 = tfA_get_next( atf, iFrame );



	if( tf0 != NULL && (tf0->iFrame == iFrame || tf1 == NULL) ){
		*T = tf0->a[0];
		*a = tf0->a[1];

		GPLOG(("A  %d   %f %f \n", iFrame, *T, *a ) );
		return( 1 );
	}


	if( tf0 == NULL && tf1 != NULL ){
		*T = tf1->a[0];
		*a = tf1->a[1];


		GPLOG(("A  %d   %f %f \n", iFrame, *T, *a ) );

		return( 1 );
	}


	float t = ( iFrame - tf0->iFrame)/(float)(tf1->iFrame - tf0->iFrame);

	*T = (1-t)*tf0->a[0] + t*tf1->a[0];
	*a = (1-t)*tf0->a[1] + t*tf1->a[1];


	GPLOG(("A  %d   %f %f \n", iFrame, *T, *a ) );

	return(  1 );

}




int	CHrEffectMorphing::Close()
{
		DeleteContents();

		return( 1 );
}

