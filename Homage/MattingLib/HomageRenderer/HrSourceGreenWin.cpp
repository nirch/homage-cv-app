//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"


#include	"VideoCodec\VideoEncoder.h"
#include	"VideoCodec\VideoDecoder.h"
#include	"Avi\Avi\AviEncoder\AviGetEncoder.h"
#include	"Avi\Avi\AviDecoder\\AviGetDecoder.h"
#include "ImageVideoCodec\\ImageGetVideoDecoder.h"



#include "HrSourceGreenWin.h"





CHrSourceGreenWin::CHrSourceGreenWin()
{
	m_videoDecoder = NULL;

	m_color = 0x02FE05;

	m_im = NULL;


}

CHrSourceGreenWin::~CHrSourceGreenWin()
{
	DeleteContents();
}





void CHrSourceGreenWin::DeleteContents()
{
	if( m_videoDecoder != NULL ){
		delete m_videoDecoder;
		m_videoDecoder = NULL;
	}
}

int CHrSourceGreenWin::Init( char *inFile )
{

	
	m_videoDecoder = GetAviDecoder( inFile );
	if( m_videoDecoder == NULL )
		m_videoDecoder = GetImageVideoDecoder();


	m_videoDecoder->SetImageFormat( IMAGE_FORMAT_RGB );



	if( m_videoDecoder->Open( inFile ) < 0  ){
		delete m_videoDecoder;

		gpWarning( "UniformMattingCA", "Open  %s  failed", inFile  );
		return( -1 );
	}

	return( 1 );


}


int	CHrSourceGreenWin::ReadFrame( int iFrame, image_type **im )
{
image_type	*sim;

	if( m_videoDecoder->ReadFrame( iFrame, &sim ) < 0 )
		return( -1 );

	IMAGE_DUMP( sim, "bb", 1, NULL );

	m_im = image3_green_alpha( sim, m_im );

	*im = m_im;

	return( 1 );
}


int	CHrSourceGreenWin::Close()
{
		DeleteContents();

		return( 1 );
}


#ifdef _AA_
image_type *
image3_green_alpha( image_type *sim, image_type *im )
{
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 4, IMAGE_TYPE_U8, 1 );

	u_char *sp = sim->data;
	u_int *tp = im->data_ui;

	for(i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, tp++ ){
			if( sp[0] == 0 && sp[1] == 255 && sp[2] == 0  ){
				*tp = 0;
				continue;
			}

			*tp = IMAGE4_RGB( sp[0], sp[1], sp[2] );
		}
	}

	return( im );
}

#endif