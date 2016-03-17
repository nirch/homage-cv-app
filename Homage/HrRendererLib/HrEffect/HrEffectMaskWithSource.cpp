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

#include "HrEffectMaskWithSource.h"



static image_type *	imageA_merge_alpha( image_type *sim, image_type *imA, image_type *im );



CHrEffectMaskWithSource::CHrEffectMaskWithSource()
{
	m_im = NULL;
    m_mask_owned_by_source = NULL;
    m_alpha = NULL;
}

CHrEffectMaskWithSource::~CHrEffectMaskWithSource()
{
	DeleteContents();
}

void CHrEffectMaskWithSource::DeleteContents()
{
    // Delete objects / free memory.
	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}

	if( m_mask_owned_by_source != NULL ){
        // No need to free this image.
        // It is owned by the source.
        m_mask_owned_by_source = NULL;
	}
    
    if( m_alpha != NULL ){
        image_destroy( m_alpha, 1 );
        m_mask_owned_by_source = NULL;
    }
    
    if (source != NULL) {
        // It is the responsibility of the source to call DeleteContents on
        // its own destructor to free its own resources.
        delete source;
        this->source = NULL;
    }
}

int CHrEffectMaskWithSource::InitWithSource(CHrSourceI *source)
{
    this->source = source;
	return( 1 );
}


int	CHrEffectMaskWithSource::Process( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{
    // Read mask frame from the source
    source->ReadFrame(iFrame, timeStamp, &m_mask_owned_by_source);
    
    // Use mask image to set alpha channel on current source image
    m_alpha = image_band( m_mask_owned_by_source, 0, m_alpha );
	m_im = imageA_merge_alpha( sim, m_alpha, m_im );
	*im = m_im;
    
	return( 1 );
}




int	CHrEffectMaskWithSource::Close()
{
		DeleteContents();
		return( 1 );
}




static image_type *
	imageA_merge_alpha( image_type *sim, image_type *imA, image_type *im )
{
	int	i,	j;

	im = image_reallocL( sim, im );

	u_int *sp = sim->data_ui;
	u_char *ap = imA->data;

	u_int *tp = im->data_ui;

	for(i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, ap++, sp ++, tp++ ){
			int a = (*sp)>>24;

			if( a > *ap ){
				a = *ap;
				*tp = (*sp&0X00FFFFFF) | ( (a << 24 ));
			}
			else	*tp = *sp;
		}
	}

	return( im );
}