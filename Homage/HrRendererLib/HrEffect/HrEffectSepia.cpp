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

	m_fR = 1.0;
	m_fG = 1.0;
	m_fB = 1.0;
}

CHrEffectSepia::~CHrEffectSepia()
{
    Close();
	DeleteContents();
}

int	CHrEffectSepia::Close()
{
    return( 1 );
}

void CHrEffectSepia::DeleteContents()
{
	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}


int CHrEffectSepia::Init(  float fR, float fG, float fB, char *file )
{
	m_fR = fR;
	m_fG = fG;
	m_fB = fB;

	return( 1 );
}


int	CHrEffectSepia::Process( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{
	m_im = image_sepia_efect( sim, m_fR, m_fG, m_fG,  m_switchBR, m_im);


	*im = m_im;

	return( 1 );
}







