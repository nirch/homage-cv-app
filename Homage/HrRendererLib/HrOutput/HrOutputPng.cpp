//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"


#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "PngCodec/PngWriter/PngWriter.h"

#include "HrOutputI.h"


#include "HrOutputPng.h"


CHrOutputPng::CHrOutputPng()
{

	m_im = NULL;

	m_color = 0xFFFFFF;


	m_recorder = NULL;

	m_delay = 30;

	m_aim = NULL;
}



CHrOutputPng::~CHrOutputPng()
{
	DeleteContents();
}





void CHrOutputPng::DeleteContents()
{
	Close();

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}



int CHrOutputPng::Init( char *dir, int width, int height, int frameSpeed )
{	
	m_width = width;
	m_height = height;

	
	strcpy( m_file, dir );

    m_delay = frameSpeed / 10;


	m_recorder = new CPngWriter();

	m_recorder->Open( 100, dir );


	return( 1 );
}


int CHrOutputPng::SetPalette( char *data )
{


	return( 1 );
}


int CHrOutputPng::WriteFrame( image_type *im, int iFrame )
{
	m_im = imageT_negative_alpha( im, m_im );

	imageA_set_colorM4( m_im, m_color, m_im );

	int ret =  m_recorder->WriteFrame( m_im );


	return( ret );
}



int	CHrOutputPng::Close()
{
	m_recorder->Close();

	delete m_recorder;

	m_recorder = NULL;


	GPLOGF(("Stop>/n"));


	return( 1 );
}