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



#include "HrEffectAlpha.h"



static image_type *	image4_alpha_efect( image_type *sim, float f, image_type *im );



CHrEffectAlpha::CHrEffectAlpha()
{
	m_id = EFFECT_ALPHA;

	m_im = NULL;

	m_atf = NULL;

}

CHrEffectAlpha::~CHrEffectAlpha()
{
    Close();
	DeleteContents();
}


int	CHrEffectAlpha::Close()
{
    return( 1 );
}


void CHrEffectAlpha::DeleteContents()
{


	if( m_im != NULL ){
		image_destroy( m_im, 1 );
		m_im = NULL;
	}


	if( m_atf != NULL ){
		tfA_destroy( m_atf );
		m_atf = NULL;
	}

}	

int CHrEffectAlpha::Init( char *tfFile )
{

	if( tfA_read( &m_atf, tfFile ) < 0 )
		return( -1 );

	return( 1 );
}

int CHrEffectAlpha::InitFromData( char *data )
{
    char	buf[10001];
    strncpy( buf, data, 1000 );
    GPLOG( ("%s\n", buf));
    
    if( tfA_read_from_data( &m_atf, data ) < 0 )
        return( -1 );
    
    return( 1 );
}

int	CHrEffectAlpha::Process( image_type *sim, int iFrame, long long timeStamp, image_type **im )
{
	float	a;
	GetPose( m_atf, iFrame, &a );

	m_im = image4_alpha_efect( sim, a, m_im );

	*im = m_im;

	return( 1 );
}







int	CHrEffectAlpha::GetPose( tfA_type *atf, int iFrame, float *a )
{


	tf_type *tf0 = tfA_get_prev( atf, iFrame+1 );
	tf_type *tf1 = tfA_get_next( atf, iFrame );



	if( tf0 != NULL && (tf0->iFrame == iFrame || tf1 == NULL) ){
		*a = tf0->a[0];
		return( 1 );
	}


	if( tf0 == NULL && tf1 != NULL ){
		*a = tf1->a[0];

		return( 1 );
	}


	float t = ( iFrame - tf0->iFrame)/(float)(tf1->iFrame - tf0->iFrame);

	*a = (1-t)*tf0->a[0] + t*tf1->a[0];


	return(  1 );

}



static image_type *
image4_alpha_efect( image_type *sim, float f, image_type *im )
{
	int	i,	j;


	im = image_reallocL( sim, im );

	u_char *sp = sim->data;
	u_char *tp = im->data;

	for( i = 0; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp += 4, tp += 4 ){

		

			tp[0] = sp[0];
			tp[1] = sp[1];
			tp[2] = sp[2];
			
			tp[3] = f*sp[3];
		}
	}

	return( im );

}
