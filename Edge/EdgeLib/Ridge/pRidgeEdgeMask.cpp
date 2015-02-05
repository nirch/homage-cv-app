/*****************************
***   pRidgeDetector.cpp   ***
******************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Uigp/igp.h"
#include "Ulog/Log.h"


#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"


#include "pRidgeType.h"



void	image1B_dilate( image_type *sim, int m0, int m1 );
void	image1B_erode1( image_type *sim );

static void	pRidge_filter_edge( image_type *rim, image_type *eim );




void
pRidge_edge_mask( image_type *sim, image_type *rim )
{
image_type	*eim;


	// edge filter
	eim = image1_gridient_sobol_value( sim, NULL );
	IMAGE_DUMP( eim, "r", 1, "eim" );

	image1_convolution_guasian( eim, 1, eim );

	image1_binary( eim, 12, eim );
	IMAGE_DUMP_SCALE( eim, 128, "r", 1, "eim12");


	image1B_erode1( eim );
	image1B_dilate( eim, 0x01, 0x02 );
	image1B_dilate( eim, 0x02, 0x04 );
	IMAGE_DUMP_SCALE( eim, 128, "r", 1, "eim12D");




	pRidge_filter_edge( rim, eim );
	PRIDGE_DUMP( rim, "r", 0, "dilate" );
}






static void
pRidge_filter_edge( image_type *rim, image_type *eim )
{
	pRidge_type	*rp;
	u_char	*ep;


	int	i,	j;


	for( i = 1 ; i < rim->height - 1 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 1 );
		ep = IMAGE_PIXEL( eim, i, 1 );

		for( j = 1 ; j < rim->width -1 ; j++, rp++, ep++ ){
			if( rp->state == 0 )	continue;
			if( *ep != 0 )
				rp->state = 0x02;
		}
	}
}





void
image1B_dilate( image_type *sim, int m0, int m1 )
{
	u_char	*sp,	*sp0,	*sp1;
	int	i,	j,	tmp;



	for( i = 1 ; i < sim->height-1 ; i++ ){

		sp = IMAGE_PIXEL( sim, i, 1 );

		sp0 = sp - sim->column;
		sp1 = sp + sim->column;
		for( j = 1 ; j < sim->width-1 ; j++, sp++, sp0++, sp1++ ){

			if( *sp != 0 )	continue;

			tmp = *(sp0-1) | *(sp0) | *(sp0+1) |
				*(sp-1) | *(sp+1) |
				*(sp1-1) | *(sp1) | *(sp1+1);

			if( tmp & m0 )
				*sp = m1;
		}
	}
}


void
image1B_erode1( image_type *sim )
{
	u_char	*sp,	*sp0,	*sp1;
	int	i,	j,	tmp;



	for( i = 1 ; i < sim->height-1 ; i++ ){

		sp = IMAGE_PIXEL( sim, i, 1 );

		sp0 = sp - sim->column;
		sp1 = sp + sim->column;
		for( j = 1 ; j < sim->width-1 ; j++, sp++, sp0++, sp1++ ){

			if( *sp == 0 )	continue;

			tmp = *(sp0-1) | *(sp0) | *(sp0+1) |
				*(sp-1) | *(sp+1) |
				*(sp1-1) | *(sp1) | *(sp1+1);

			if( tmp == 0 )
				*sp = 0;
		}
	}
}




image_type *
pRidge_weight( image_type *rim, image_type *wim )
{
u_char	w[] = { 0, 64, 0, 0 };

pRidge_type	*rp;
u_char	*tp;
int	i,	j;


	wim = image_realloc( wim, rim->width, rim->height, 1, IMAGE_TYPE_U8, 1 );


	tp = wim->data;
	rp = (pRidge_type *)rim->data;
	for( i = 0 ; i < rim->height ; i++ ){
		for( j = 0 ; j < rim->width ; j++, rp++ ){
			*tp++ = w[rp->state];
		}
	}


	IMAGE_DUMP( wim, "r", 1, "w" );


	image1_convolution_guasian( wim, 1, wim );

	IMAGE_DUMP( wim, "r", 1, "ws" );

	return( wim );
}