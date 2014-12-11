
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"

static image_type *	image1_smooth( image_type *sim, image_type *im );




int	CUniformBackground::ProcessSmooth()
{
	gpTime_start( &m_tSmooth );

	if( m_cimS == NULL ){
		m_cimS = image_alloc( m_cim->width, m_cim->height, 1, IMAGE_TYPE_U8, 1 );
		image1_const( m_cimS, 0 );
	}


	
//	m_cimS = image1_convolution_guasian( m_cim, 0.75, m_cimS );

	m_cimS = image1_smooth( m_cim, m_cimS );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_cim, "SM", m_iFrame, NULL );
		IMAGE_DUMP( m_cimS, "SM", m_iFrame, "SS" );
	}
	
	gpTime_stop( &m_tSmooth );

	return( 1 );
}




static image_type *
image1_smooth( image_type *sim, image_type *im )
{
int	a0[] = { 0, 127 };
int a1[] = { 0, 16, 32, 48, 64, 80, 96, 112, 128 };
int	i,	j;

	
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	u_char *tp = IMAGE_PIXEL( im, 0, 0 );
	u_char *sp = IMAGE_PIXEL( sim, 0, 0 );


	for( j = 0 ; j < sim->width ; j++ )
		*tp++ = *sp++;
	
	for( i = 1 ; i < sim->height-1 ; i++ ){
		*tp++ = *sp++;
		u_char *sp0 = sp - sim->width;
		u_char *sp1 = sp + sim->width;


		for( j = 1 ; j < sim->width-1 ; j ++, sp++, sp0++, sp1++ ){
			
				int i1 =  ( *(sp0-1) + *(sp0) + *(sp0+1) + 
					*(sp-1) + *(sp+1) +
					*(sp1-1) + *(sp1) + *(sp1+1)  + 8 ) >> 8;

				int i0 = ( (*sp) + 1)>> 8;

				*tp++ = a0[i0] + a1[i1];
		}
		*tp++ = *sp++;
	}


	for( j = 0 ; j < sim->width ; j++ )
		*tp++ = *sp++;



	return( im );
}