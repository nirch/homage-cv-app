//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"
#include "Uvl/TfType.h"

#include "Umath/Matrix3Type.h"
#include "Umath/Matrix2Type.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"




#include "HrEffectAlignment.h"





CHrEffectAlignment::CHrEffectAlignment()
{
	m_id = EFFECT_ALIGNMENT;

	m_im = NULL;


	lt2_unity( &m_lt );

}

CHrEffectAlignment::~CHrEffectAlignment()
{
	DeleteContents();
}





void CHrEffectAlignment::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}



}

int CHrEffectAlignment::Init( char *inFile )
{

	box2f_read_from_file( &m_box, inFile );


	float x0 = 0.5*(m_box.x0 + m_box.x1 );
	float y0 = 0.5*(m_box.y0 + m_box.y1 );
	x0 *= 0.5;
	y0 *= 0.5;


	//box2f_type b;
	//b.x0 = m_box.x0 - x0 + 240;
	//b.x1 = m_box.x1 - x0 + 240;

	//b.y0 = m_box.y0 - y0 + 160;
	//b.y1 = m_box.y1 - y0 + 160;




	m_lt.c0 =  y0 - 120; 
	m_lt.c1 =  x0 - 80;


	m_iFrame = -1;
	return( 1 );
}


int CHrEffectAlignment::InitFromData( char *data0, char *data )
{

	if( box2f_read_data( &m_box, data ) < 0 )
		return( -1 );


	float x0 = 0.5*(m_box.x0 + m_box.x1 );
	float y0 = 0.5*(m_box.y0 + m_box.y1 );
	x0 *= 0.5;
	y0 *= 0.5;



	float x00 = 80;
	float y00 = 120;
	if( data0 != NULL ){
		if( box2f_read_data( &m_box0, data0 ) < 0 )
			return( -1 );

		x00 = 0.5*(m_box0.x0 + m_box0.x1 );
		y00 = 0.5*(m_box0.y0 + m_box0.y1 );
		
	}


	m_lt.c0 =  y0 - y00; 
	m_lt.c1 =  x0 - x00;


	m_iFrame = -1;

	return( 1 );
}


int	CHrEffectAlignment::Process( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{


	IMAGE_DUMP( sim, "aa", iFrame, "pose-src");
	m_im = image_dewarp_lt2( sim, &m_lt, m_im );
	IMAGE_DUMP( m_im, "aa", iFrame, "pose-out");



	m_iFrame = iFrame;

	*im = m_im;


	return( 1 );
}




int	CHrEffectAlignment::Close()
{
		DeleteContents();

		return( 1 );
}


