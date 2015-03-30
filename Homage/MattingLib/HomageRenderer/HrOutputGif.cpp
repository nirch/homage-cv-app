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

#include	"Gif/ImageWriteGif.h"

#include "HrOutputI.h"


#include "HrOutputGif.h"


CHrOutputGif::CHrOutputGif()
{
	m_gifIo = NULL;

	m_im = NULL;

	m_color = 0x02FE05;

	m_palette = NULL;

	m_delay = 30;

	m_aim = NULL;
}



CHrOutputGif::~CHrOutputGif()
{
	DeleteContents();
}





void CHrOutputGif::DeleteContents()
{
	Close();

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}
}



int CHrOutputGif::Init( char *outFile, int width, int height, int frameSpeed )
{	
	m_width = width;
	m_height = height;

	
	strcpy( m_file, outFile );

    m_delay = frameSpeed / 10;


	return( 1 );
}


int CHrOutputGif::SetPalette( char *data )
{
	m_palette = palette_read_heax_data( data );

	return( 1 );
}


int CHrOutputGif::WriteFrame( image_type *im, int iFrame )
{
    //	m_im = image_make_copy( im, m_im );
	m_im = imageT_negative_alpha( im, m_im );
	if( m_gifIo == NULL ){

		if( m_aim == NULL )
			m_aim = imageA_alloc( 4 );

		m_aim->a[m_aim->nA++] = image_make_copy( m_im, NULL );
		if( m_aim->nA < 4 )
			return( 1 );



		if( m_palette == NULL ){
			m_palette = palette_alloc( 256 );
			m_palette->nColor = 0;
		}


		image_adaptive_palette( m_aim->a, m_aim->nA, m_palette, (1<<8) - 1 );

		imageA_destroy( m_aim );

		int	i;
		for( i = m_palette->nColor ; i < 255 ; i++  ){
			m_palette->data[i].r = 255;
			m_palette->data[i].b = 255;
			m_palette->data[i].g = 255;
		}
	
		m_palette->nColor = 255;

//		if( tim != im )
//			image_destroy( tim, 1 );

		m_palette->data[255].r = 255;
		m_palette->data[255].b = 255;
		m_palette->data[255].g = 255;

//		m_palette->nColor = 255;
		int transparent_index = 255;


		m_gifIo = image_write_gifIo_open_file( m_file, m_height, m_width,
			m_delay,
			m_palette, transparent_index, 0,
			-1 );

	}



	int ret = image_write_gifIo_add_frame( m_gifIo, m_im, 0 );

	return( ret );
}



int	CHrOutputGif::Close()
{
	if( m_gifIo != NULL ){
		image_write_gifIo_close( m_gifIo );

		m_gifIo = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	return( 1 );
}