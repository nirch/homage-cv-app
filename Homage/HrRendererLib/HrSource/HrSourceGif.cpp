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


#include	"Gif/ImageReadGif.h"



#include "HrSourceGif.h"





CHrSourceGif::CHrSourceGif()
{
	m_gifIo = NULL;


	m_im = NULL;


}

CHrSourceGif::~CHrSourceGif()
{
	DeleteContents();
}





void CHrSourceGif::DeleteContents()
{
	if( m_gifIo != NULL ){
		image_read_gif_close( m_gifIo );
		m_gifIo = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}

int CHrSourceGif::Init( char *inFile )
{

	if( (m_gifIo = image_read_gif_open_file( inFile )) == NULL )
		return( -1 );

	m_width = m_gifIo->column;
	m_height = m_gifIo->row;

	image_read_gif_get_frame_no( m_gifIo );
	m_nFrame = m_gifIo->frame_no;

	m_width = m_gifIo->column;
	m_height - m_gifIo->row;

	m_iFrame = -1;
	return( 1 );


}


int	CHrSourceGif::ReadFrame( int iFrame, image_type **im )
{


	if( iFrame < 0 || iFrame >= m_nFrame )	return( -1 );

	//if( m_iFrame == iFrame ){
	//	if( m_im == NULL )
	//		return( -1 );


	//	*im = m_im;
	//	return( 1 );
	//}


	if( image_read_gif_i( m_gifIo, iFrame ) < 0 )
		return( -1 );

	m_im = image_make_copy( m_gifIo->im, m_im );
	imageT_negative_alpha( m_im, m_im );



	ProcessEffect( m_im, iFrame, im );
//	*im = m_im;

	m_iFrame = iFrame;




	return( 1 );
}


int	CHrSourceGif::Close()
{
		DeleteContents();

		return( 1 );
}

