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


#include "HrEffectGray.h"



static image_type *	image4_gray_efect( image_type *sim, image_type *im );


CHrEffectGray::CHrEffectGray()
{
	m_id = EFFECT_GRAY;

	m_im = NULL;


}

CHrEffectGray::~CHrEffectGray()
{
    Close();
	DeleteContents();
}





void CHrEffectGray::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}



}

int	CHrEffectGray::Close()
{
    return( 1 );
}




int CHrEffectGray::Init( char *file )
{
	return( 1 );
}


int	CHrEffectGray::Process( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{
	m_im = image4_gray_efect( sim, m_im );

	*im = m_im;

	return( 1 );
}







static image_type *
	image4_gray_efect( image_type *sim, image_type *im )
{
	int	i,	j;


	im = image_reallocL( sim, im );

	u_char *sp = sim->data;
	u_char *tp = im->data;

	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp += 4, tp += 4 ){

		

			int tmp;
			tmp  =  0.29900 * sp[0] + 0.58700 * sp[1] + 0.11400 * sp[2] + 0.5;
			tp[0] = tp[1] = tp[2] = PUSH_TO_RANGE( tmp, 0, 255 );

			tp[3] = sp[3];
		}
	}

	return( im );

}
