//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifndef _GPLOG
#define _GPLOG
#endif

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"

#include "Png/PngCodec/Png/ImageReadPng.h"




#include "HrSourcePng.h"





CHrSourcePng::CHrSourcePng()
{
	m_im = NULL;

	m_af = NULL;
}

CHrSourcePng::~CHrSourcePng()
{
	DeleteContents();
}





void CHrSourcePng::DeleteContents()
{

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}

	if( m_af != NULL )
		intA_destroy( m_af );

}

int CHrSourcePng::Init( char *inFile )
{

	strcpy( m_dir, inFile );
	
	m_iFrame = 0;

	return( 1 );


}


int CHrSourcePng::Init( char *inFile, int aFrame[], int nF )
{

	strcpy( m_dir, inFile );


	m_af = intA_alloc( nF );

	GPLOG(("PngSource:nF %d", nF ) );

	int	i;
	for( i = 0 ; i < nF ; i++ ){
		m_af->a[i] = aFrame[i];
		GPLOG(("PngSource:  %d", aFrame[i] ) );
	}

	GPLOG(("\n" ) );
	m_iFrame = 0;

	image_type *im;
	if( ReadFrame( 0, 0, &im ) < 0 )
		return( -1 );

	m_width = im->width;
	m_height = im->height;

	return( 1 );


}


int	CHrSourcePng::ReadFrame( int iFrame, long long timeStamp, image_type **im )
{
image_type	*sim;

	int rFrame = ( m_af != NULL )? m_af->a[iFrame] : iFrame;

	m_iFrame = rFrame;


	GPLOG(("PngSource:  %d %d", iFrame, rFrame ) );

	char file[256];
	sprintf( file, "%s/im-%.4d.png", m_dir, rFrame );
	sim = image_read_png_file( file );
	if( sim == NULL ){
		GPLOG(( "reading %s failed\n", file ) );
		return( -1 );
	}

	GPLOG(( "reading %s succeed\n", file ) );

	IMAGE_DUMP( sim, "bb", 1, NULL );

	m_im = image_make_copy( sim, m_im );
		
	imageT_negative_alpha( m_im, m_im );


	ProcessEffect( m_im, iFrame, im );

	image_destroy( sim, 1 );


	return( 1 );
}


int	CHrSourcePng::Close()
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