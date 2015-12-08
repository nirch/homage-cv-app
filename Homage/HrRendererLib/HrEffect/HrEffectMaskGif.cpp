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



#include	"Gif/ImageReadGif.h"


#include "HrEffectMaskGif.h"



static image_type *	imageA_merge_alpha( image_type *sim, image_type *imA, image_type *im );



CHrEffectMaskGif::CHrEffectMaskGif()
{
	m_id = EFFECT_MASKGIF;

	m_gifIo = NULL;

	m_im = NULL;


	m_alphaIm = NULL;

}

CHrEffectMaskGif::~CHrEffectMaskGif()
{
	DeleteContents();
}





void CHrEffectMaskGif::DeleteContents()
{
	if( m_gifIo != NULL ){
		image_read_gif_close( m_gifIo );
		m_gifIo = NULL;
	}

	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	if( m_alphaIm != NULL ){
		image_destroy( m_alphaIm, 1 );
		m_alphaIm = NULL;
	}

}



int CHrEffectMaskGif::Init( char *inFile )
{

	if( (m_gifIo = image_read_gif_open_file( inFile )) == NULL )
		return( -1 );

	m_width = m_gifIo->column;
	m_height = m_gifIo->row;

	image_read_gif_get_frame_no( m_gifIo );
	m_nFrame = m_gifIo->frame_no;

	m_iFrame = 0;
	return( 1 );


}


int	CHrEffectMaskGif::Process( image_type *sim, int iFrame, image_type **im ) 
{

	if( image_read_gif_i( m_gifIo, iFrame ) < 0 )
		return( -1 );


	m_alphaIm = image_band( m_gifIo->im, 0, m_alphaIm );


	m_im = imageA_merge_alpha( sim, m_alphaIm, m_im );

	*im = m_im;

	return( 1 );
}




int	CHrEffectMaskGif::Close()
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