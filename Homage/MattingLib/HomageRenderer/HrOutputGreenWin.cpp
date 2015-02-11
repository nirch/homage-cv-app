//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"

#include	"VideoCodec\VideoEncoder.h"
#include	"VideoCodec\VideoDecoder.h"
#include	"Avi\Avi\AviEncoder\AviGetEncoder.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "HrOutputI.h"


#include "HrOutputGreenWin.h"


CHrOutputGreenWin::CHrOutputGreenWin()
{
	m_codec = AVI_CODEC_INDEO50;
	m_frameRate = 10;
	m_bitrate = 10000;

	m_videoEncoder = NULL;

	m_im = NULL;

	m_color = 0x02FE05;
}

CHrOutputGreenWin::~CHrOutputGreenWin()
{
	DeleteContents();
}





void CHrOutputGreenWin::DeleteContents()
{
	if( m_videoEncoder != NULL ){
		delete m_videoEncoder;
		m_videoEncoder = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}

void CHrOutputGreenWin::Set( int codec , int frameRate, int bitrate )
{

	m_codec = codec;
	m_frameRate = frameRate;
	m_bitrate = bitrate;
}

int CHrOutputGreenWin::Init( char *outFile, int width, int height )
{	
	m_width = width;
	m_height = height;

	m_videoEncoder = AviVideoEncoder_create( outFile, width, height, m_codec , m_frameRate, m_bitrate );


	if( m_videoEncoder == NULL )
		return( -1 );

	return( 1 );
}





int CHrOutputGreenWin::WriteFrame( image_type *im )
{

	m_im = imageA_set_colorM( im, m_color, m_im );

	m_videoEncoder->WriteFrame( m_im, 0 );

	return( 1 );
}



int	CHrOutputGreenWin::Close()
{
	if( m_videoEncoder != NULL ){
		m_videoEncoder->Close();
		delete m_videoEncoder;
		m_videoEncoder = NULL;
	}


	return( 1 );
}