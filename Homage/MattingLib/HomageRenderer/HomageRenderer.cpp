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

#include "HomageRenderer.h"




CHomageRenderer::CHomageRenderer()
{
	m_im = NULL;


}

CHomageRenderer::~CHomageRenderer()
{
	DeleteContents();
}





void CHomageRenderer::DeleteContents()
{
	if( m_im != NULL ){
		image_destroy( m_im, 1 );
			m_im = NULL;
	}

}


int
CHomageRenderer::Process( CHrSourceI *b, CHrSourceI *f,  CHrSourceI *h, CHrOutputI *out )
{
int	i;
image_type *im;

	for( i = 0 ; ; i++ ){

		if( b->ReadFrame( i, &im ) < 0 )
			break;

		IMAGE_DUMP( im, "aa", i, "0" );
		IMAGE_DUMP_ALPHA( im, "aa", i, "0-a" );

		m_im = image_make_copy( im, m_im );

		IMAGE_DUMP( m_im, "aa", i, "1" );
		IMAGE_DUMP_ALPHA( m_im, "aa", i, "1-a" );


		if( f->ReadFrame( i, &im ) < 0 )
			break;

		imageA_combine( im, m_im );

		IMAGE_DUMP( m_im, "aa", i, "2" );
		IMAGE_DUMP_ALPHA( m_im, "aa", i, "2-a" );



		if( h->ReadFrame( i, &im ) < 0 )
			continue;

		imageA_combine( im, m_im );

		IMAGE_DUMP( m_im, "aa", i, "3" );
		IMAGE_DUMP_ALPHA( m_im, "aa", i, "3-a" );


		out->WriteFrame( m_im );

	}
	


	return( -1 );
}


image_type *
CHomageRenderer::Process( image_type *bim,  
							int x0, int y0, float scale, image_type *him, image_type *himA,  
							image_type *fim[], int nFim,
							image_type *im )
{
int	i;
	
	
	im =  image_make_copy( bim, im );


	image3_add_layer( im, him, himA, x0, y0 );


	for( i = 0; i < nFim ; i++ )
		image3_add_layerA( im, fim[i], 0, 0 );



	return( im );
}




int CHomageRenderer::SetBackground( image_type *bim )
{

	m_im =  image_make_copy( bim, m_im );

	return( 1 );
}



int CHomageRenderer::AddLayer( int x0, int y0, image_type *sim, image_type *aim )
{
	if( m_im == NULL )
		return( 1 );

	image3_add_layer( m_im, sim, aim, x0, y0 );


	return( 1 );
}


int CHomageRenderer::AddLayer( int x0, int y0, image_type *sim )
{
	if( m_im == NULL )
		return( 1 );

	image3_add_layerA( m_im, sim, x0, y0 );


	return( 1 );
}
