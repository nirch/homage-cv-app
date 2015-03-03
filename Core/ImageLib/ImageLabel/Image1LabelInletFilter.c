/************************************
 ***   Image1LabelInLetFilter.c   ***
 ************************************/
#include <math.h>
#include "Uigp/igp.h"
#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"


#include "ImageLabel/ImageLabel.h"

#include "ImageMorphology/ImageMorphology.h"

#ifdef _DEBUG
//#define _DUMP
#endif

#define		MN	8

static void	imageLabel_inlet_filter_1( image_type *sim, image_type *uim, bwLabel_type *bw, int n );



int 
imageLabel_inlet_filter( image_type *sim, int n )
{
	image_type *lim;
	bwLabel_type *aBw,	*bw;
	int nBw;
	int	*aI,	nI;
	int	i;

	lim = image1_label( sim, &aBw, &nBw, NULL );

	imageLabelUI_set_aux( lim, aBw, nBw );


	aI = (int * )malloc( nBw*sizeof(int) );
	bwLabel_order(aBw, nBw, nBw, aI, &nI );


	for( i = 0 ; i < nI ; i++ ){
		bw = &aBw[aI[i] ];

		if( bw->no < 100 )	break;

		imageLabel_inlet_filter_1( sim, lim, bw, n );
	}




	image_destroy( lim, 1 );

	free( aBw );
	free( aI );

	return( 1 );
}






static void
imageLabel_inlet_filter_1( image_type *sim, image_type *uim, bwLabel_type *bw, int n )
{
	int	i,	j,	i0;
	u_char	*tp,	*sp;
	u_int	*up;
	int	width,	height;
	image_type	*im;

	width = bw->b.x1 -  bw->b.x0 + 2*MN;
	height = bw->b.y1 -  bw->b.y0 + 2*MN;

	im = image_create( height, width, 1, 1, NULL );
	image1_const( im, 0 );

	for( i = bw->b.y0, i0 = MN ; i <= bw->b.y1 ; i++, i0++ ){
		up = (u_int *)IMAGE_PIXEL( uim, i, bw->b.x0 );
		tp = (u_char *)IMAGE_PIXEL( im, i0, MN );

		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++, up++ ){
			if( *up != bw->id )	continue;

			*tp = 1;
		}
	}

#ifdef _DUMP
	image_dump_scale( im, 255, "inlet", bw->id, NULL );
#endif

	image1_close( im, n, 255 );


	for( i = bw->b.y0, i0 = MN ; i <= bw->b.y1 ; i++, i0++ ){
		sp = (u_char *)IMAGE_PIXEL( sim, i, bw->b.x0 );
		up = (u_int *)IMAGE_PIXEL( uim, i, bw->b.x0 );

		tp = (u_char *)IMAGE_PIXEL( im, i0, MN );

		for( j = bw->b.x0 ; j <= bw->b.x1 ; j++, tp++, sp++, up++ ){
			if( *tp != 0 ){
				*sp = bw->color;
				*up = bw->id;
			}
		}
	}


#ifdef _DUMP
	image_dump_scale( im, 255, "inlet", bw->id, "AA" );
#endif

	image_destroy( im, 1 );
}
