
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include "Uvl/TfType.h"

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "plnTracker/PlnHeadTracker/PlnHeadTracker.h"
#include "UniformBackground.h"

#include "UnBackground/bImage.h"

#include "ImageMorphology/ImageMorphology.h"

#include "UnBackground/UnBackground.h"

#include "EdgeLib/Ridge/RidgeDetector.h"

//#define _DYNAMIC_BACKGROUND

#ifdef _DYNAMIC_BACKGROUND
#include "../DynamicMask/DynamicMask.h"
#endif



//#define EXCEPTION





void		bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );



image_type *	bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im );


static void	image1_close1( image_type *sim );





int	CUniformBackground::ProcessCompare( image_type *sim )
{

	gpTime_start( &m_tCompare );




	m_dim = bImage_diff( sim,  &m_roi, m_N, m_bim, m_T, m_dim );


	m_cim = image1_binaryM( m_dim, m_T, m_cim );



	if( m_iFrame == m_dFrame ){
		bImage_dump( m_bim, m_N, "bim", m_iFrame, "B" );
		IMAGE_DUMPF( sim, "m", m_iFrame, "SIM", m_dFrame == m_iFrame  );
		IMAGE_DUMPF( m_cim, "m", m_iFrame, "1", m_dFrame == m_iFrame  );
	}


	gpTime_start( &m_tOpen );
	image1_negative( m_cim, m_cim );
	image1_open( m_cim, 1, 0 );
	image1_negative( m_cim,m_cim );
	//	image1_close( m_cim, 1, 255 );
	gpTime_stop( &m_tOpen );




	IMAGE_DUMPF( m_cim, "m", m_iFrame, "22", m_dFrame == m_iFrame  );



	gpTime_start( &m_tOpen );
	image1_open( m_cim, 1, 0 );
//	image1_close( m_cim, 1, 255 );
	gpTime_stop( &m_tOpen );


	IMAGE_DUMPF( m_cim, "m", m_iFrame, "2", m_dFrame == m_iFrame  );



	ProcessBlob2();
	IMAGE_DUMPF( m_cim, "m", m_iFrame, "3", m_dFrame == m_iFrame  );





	gpTime_stop( &m_tCompare );


	return( 1 );
}








image_type *
bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im )
{
	int	i,	j,	k,	n,	i0;
	u_char	*sp,	*tp;
	bImage_type *bp,	*bp0;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= sim->height )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );
			tp = IMAGE_PIXEL( im, i0, b->x0 );


			for( j = 1, bp = bp0 ; j < bim->width-1 ; j++, bp++ ){
				for( n = 0 ; n < N ; n++, tp++ ){
	
					int dr = *sp++ - bp->r;
					int dg = *sp++ - bp->g;
					int db = *sp++ - bp->b;

					int d = ABS(dr);
					//d *= 1.2;
					if( dg < 0 )	dg = -dg;
					if( d < dg )	d = dg;

					if( db < 0 )	db = -db;
					if( d < db )	d = db;


					//if( d < T )
					//	*tp = 0;
					//else *tp = 255;

					*tp = d;
				}
			}
		}

	}

	return( im );
}





static void
image1_close1( image_type *sim )
{

	int	i,	j;



	u_char *sp = IMAGE_PIXEL( sim, 1, 1 );

	for( i = 1 ; i < sim->height-1 ; i++, sp += 2 ){
		u_char *sp0 = sp - sim->width;
		u_char *sp1 = sp + sim->width;

		for( j = 1 ; j < sim->width-1 ; j ++, sp++, sp0++, sp1++ ){

			if( *sp != 0 )	continue;

			int i1 =  ( *(sp0-1) + *(sp0) + *(sp0+1) + 
				*(sp-1) + *(sp+1) +
				*(sp1-1) + *(sp1) + *(sp1+1) );

			if( i1 >= 255 * 7 )
				*sp = 255;

			
		}
	}
}




