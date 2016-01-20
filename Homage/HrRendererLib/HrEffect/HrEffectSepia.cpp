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

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#ifndef __APPLE__
#include "Png/PngCodec/Png/ImageReadPng.h"
#endif


#include "HrEffectSepia.h"





CHrEffectSepia::CHrEffectSepia()
{
	m_id = EFFECT_SEPIA;

	m_im = NULL;
}

CHrEffectSepia::~CHrEffectSepia()
{
	DeleteContents();
}





void CHrEffectSepia::DeleteContents()
{
	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}





int CHrEffectSepia::Init( char *file )
{
	return( 1 );
}


int	CHrEffectSepia::Process( image_type *sim, int iFrame, image_type **im ) 
{
	m_im = image_sepia_efect( sim, m_im );


	*im = m_im;

	return( 1 );
}




int	CHrEffectSepia::Close()
{
		DeleteContents();

		return( 1 );
}



