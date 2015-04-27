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
#include "PngVideoCodec\PngGetVideoDecoder.h"



#include "HrSourcePngWin.h"





CHrSourcePngWin::CHrSourcePngWin()
{
	m_videoDecoder = NULL;

	m_color = 0x02FE05;

	m_im = NULL;


}

CHrSourcePngWin::~CHrSourcePngWin()
{
	DeleteContents();
}





void CHrSourcePngWin::DeleteContents()
{
	if( m_videoDecoder != NULL ){
		delete m_videoDecoder;
		m_videoDecoder = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}

int CHrSourcePngWin::Init( char *inFile )
{

	
	m_videoDecoder = GetPngVideoDecoder();


	m_videoDecoder->SetImageFormat( IMAGE_FORMAT_RGB4 );



	if( m_videoDecoder->Open( inFile ) < 0  ){
		delete m_videoDecoder;

		gpWarning( "UniformMattingCA", "Open  %s  failed", inFile  );
		return( -1 );
	}

	return( 1 );


}


int	CHrSourcePngWin::ReadFrame( int iFrame, image_type **im )
{
image_type	*sim;

	if( m_videoDecoder->ReadFrame( iFrame, &sim ) < 0 )
		return( -1 );

	IMAGE_DUMP( sim, "bb", 1, NULL );

	m_im = image_make_copy( sim, m_im );
		
	imageT_negative_alpha( m_im, m_im );

	//MergeAlpha( m_im );

	ProcessEffect( m_im, iFrame, im );

	//*im = m_im;

	return( 1 );
}


int	CHrSourcePngWin::Close()
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