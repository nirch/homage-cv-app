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

#ifndef __APPLE__
#include "Png/PngCodec/Png/ImageReadPng.h"
#endif


#include "HrEffectMask.h"


static image_type *	imageA_merge_alpha( image_type *sim, image_type *imA, image_type *im );



CHrEffectMask::CHrEffectMask()
{
	m_id = EFFECT_MASK;

	m_im = NULL;


	m_alphaIm = NULL;

}

CHrEffectMask::~CHrEffectMask()
{
	DeleteContents();
}





void CHrEffectMask::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	if( m_alphaIm != NULL ){
		image_destroy( m_alphaIm, 1 );
		m_alphaIm = NULL;
	}

}

int CHrEffectMask::Init( image_type *im )
{
	if( im->depth != 1 ){
		m_alphaIm = image1_from( im, NULL );
		image_destroy( im, 1 );
		return( 1 );
	}

	m_alphaIm = im;

	return( 1 );


}



int CHrEffectMask::Init( char *file )
{
#ifndef __APPLE__
	image_type *im = image_read_png_file( file );
	if( im == NULL )
		return( -1 );

	if( im->depth != 1 ){
		m_alphaIm = image1_from( im, NULL );
		image_destroy( im, 1 );
		return( 1 );
	}

	m_alphaIm = im;

	return( 1 );

#endif
}


int	CHrEffectMask::Process( image_type *sim, int iFrame, image_type **im ) 
{

	m_im = imageA_merge_alpha( sim, m_alphaIm, m_im );

	*im = m_im;

	return( 1 );
}




int	CHrEffectMask::Close()
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