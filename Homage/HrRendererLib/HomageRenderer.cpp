//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#define _GPLOG

#include "Ulog/Log.h"

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

	m_nS = 0;
	m_nOut = 0;

	m_process = 0;
    
    this->wasCanceled = false;
    
    // Default value
    this->setDuration(2.0, 12);
}

CHomageRenderer::~CHomageRenderer()
{
	DeleteContents();
}





void CHomageRenderer::DeleteContents()
{
int	i;

	for( i = 0 ; i < m_nS ; i++ ){
		delete m_as[i];
		m_as[i]  = 0;
	}
	m_nS = 0;

	for( i = 0 ; i < m_nOut ; i++ ){
		delete m_aOut[i];
		m_aOut[i]  = 0;
	}

	m_nOut = 0;

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
			m_im = NULL;
	}

}

int CHomageRenderer::AddSource( CHrSourceI *s )
{
	m_as[m_nS++] = s;

	return( m_nS -1  );
}

CHrSourceI * CHomageRenderer::GetSource( int i )
{
	if( i < 0 || i >= m_nS )
		return( NULL );

	return( m_as[i] );
}


void
CHomageRenderer::SetFrameSize( int width, int height )
{
	int	i,	k;


	for( i = 0 ; i < m_nS ; i++ ){
		CHrSourceI *s = m_as[i];
		s->SetFrameSize( width, height );
	}


	for( i = 0 ; i < m_nOut ; i++ ){
		CHrOutputI *o = m_aOut[i];
		o->SetFrameSize( width, height );
	}
}

void CHomageRenderer::setDuration(double duration, int fps)
{
    this->duration = duration;
    this->fps = fps;
    this->maxTimeStamp = duration * 1000000000;
    this->timeDeltaPerFrame = 1000000000 / fps;
}


int CHomageRenderer::AddOutput( CHrOutputI *o )
{
	m_aOut[m_nOut++] = o;

GPLOGF( ("AddOutput: %d\n", m_nOut ));

	return( m_nOut-1 );
}

CHrOutputI * CHomageRenderer::GetOuput( int i )
{
	if( i < 0 || i >= m_nOut )
		return( NULL );

	return( m_aOut[i] );
}


void
CHomageRenderer::Cancel()
{
    this->wasCanceled = true;
}


int
CHomageRenderer::Process()
{
	int	i,	k;
	image_type *im;

	m_process = 1;
    long long timeStamp = 0;
	for( i = 0 ; ; i++ ){
        
        // Iterate sources, read frame for each source and combine.
		for( k = 0 ; k < m_nS ; k++ ){
            
            if( timeStamp >= maxTimeStamp || wasCanceled == true)
                break;
            
			if( m_as[k]->ReadFrame( i, timeStamp, &im ) < 0 )
				break;

			if( k == 0 ){
				m_im = image_make_copy( im, m_im );

				continue;
			}

			imageA_combine( im, m_im );

			IMAGE_DUMP( m_im, "AA", k, NULL );
			IMAGE_DUMP( im, "AA", k, "M" );
		}

        // Continue only if all sources combined.
		if( k < m_nS )
            break;
		
        // Iterate output
		for( k = 0 ; k < m_nOut ; k++ ){
			GPLOGF( ("Frame Write %d-%d\n", i, k ));
			m_aOut[k]->WriteFrame( m_im, i );
		}


		IMAGE_DUMP( m_im, "im", i, NULL );
		IMAGE_DUMP_ALPHA( m_im, "im-a", i, NULL );


		//fprintf( stdout, "  ." );
        timeStamp += this->timeDeltaPerFrame;
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
