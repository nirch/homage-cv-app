
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"

#include "ColorBackground.h"






#define RGB2CBI( R, G, B )	((-0.16874 * R - 0.33126 * G + 0.50000 * B) + 128.5)
#define RGB2CRI( R, G, B )	(( 0.50000 * R - 0.41869 * G - 0.08131 * B) + 128.5)

#define RGB2CB( R, G, B )	((-11058 * R - 21709 * G + 32768 * B + 8421376)>>16)
#define RGB2CR( R, G, B )	(( 32768 * R - 27439 * G - 5328  * B + 8421376)>>16)







CColorBackground::CColorBackground()
{

	m_him = NULL;

	m_cim = NULL;


	colorShiftThresh = 15;

}


CColorBackground::~CColorBackground()
{
	DeleteContents();

}



void CColorBackground::DeleteContents()
{

	if( m_him != NULL ){
		image_destroy( m_him, 1 );
		m_him = NULL;
	}

}




int	CColorBackground::ProcessHistogram( image_type *sim, image_type *mim )
{
	m_him = image3_histogram_CrCb( sim,  mim, m_him );

	imageUI_bluring( m_him, 7 );

	imageUI_maxPixel( m_him, &m_v.x, &m_v.y );

	return( 1 );


}






int	CColorBackground::Process( image_type *sim, image_type *mim, image_type **cim )
{
	int	i,	j;
	u_char	*sp,	*mp,	*tp;
	int	R,	G,	B;
	int	cR,	cB;


	m_cim = image_realloc( m_cim, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	mp = mim->data;
	sp = sim->data;
	tp = m_cim->data;
	

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++,  mp++, tp++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

			if( *mp == 0 )	continue;


			cB = RGB2CB( R, G, B );
			cR = RGB2CR( R, G, B );

			int x = cR - m_v.x;
		
			int y = cB - m_v.y;
			
			int d = x*x + y*y;

			if( d < 10*10 )
				*tp = 0;
			else *tp = 255;

		}
	}

	*cim = m_cim;

	return( 1 );
	//	image_dump( im, "mask", 1, "mm"  );
}




