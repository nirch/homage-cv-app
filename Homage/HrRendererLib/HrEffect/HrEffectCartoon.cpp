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


#include "HrEffectCartoon.h"



static image_type *	image4_cartoon( image_type *sim, palette_type *pl, image_type *im );



CHrEffectCortoon::CHrEffectCortoon()
{
	m_id = EFFECT_CARTOON;

	m_im = NULL;


	m_palette = NULL; 

}

CHrEffectCortoon::~CHrEffectCortoon()
{
	DeleteContents();
}





void CHrEffectCortoon::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}



}





int CHrEffectCortoon::Init( int nColor, char *file )
{
	m_nColor = nColor;
	return( 1 );
}


int	CHrEffectCortoon::Process( image_type *sim, int iFrame, image_type **im ) 
{
	if( m_palette == NULL ){

		m_palette = palette_alloc( 256 );
		m_palette->nColor = 0;


		imageT_negative_alpha( sim, sim );
		image_adaptive_palette( &sim, 1, m_palette, m_nColor );
		imageT_negative_alpha( sim, sim );
	}

	m_im = image4_cartoon( sim, m_palette, m_im );

	*im = m_im;

	return( 1 );
}




int	CHrEffectCortoon::Close()
{
		DeleteContents();

		return( 1 );
}



static image_type *
	image4_cartoon( image_type *sim, palette_type *pl, image_type *im )
{
	int	i,	j;

	imageT_negative_alpha( sim, sim );
	image_type *im8 = image_24to8( sim, pl, NULL );
	imageT_negative_alpha( sim, sim );



	im = image_reallocL( sim, im );

	u_char *sp = sim->data;
	u_char *mp = im8->data;
	u_char *tp = im->data;

	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp += 4, tp += 4, mp++ ){
			tp[3] = sp[3];

			int k = *mp;
			tp[0] = pl->a[k].Blue;
			tp[1] = pl->a[k].Green;
			tp[2] = pl->a[k].Red;
		}
	}

	image_destroy( im8, 1 );

	return( im );

}
