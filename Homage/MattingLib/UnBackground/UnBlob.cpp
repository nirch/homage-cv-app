//  Defines the entry point for the console application.
//
#include <stdio.h>
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageMorphology/ImageMorphology.h"
#include "Imagelabel/BwLabel.h"

#define DIFF_THRESH 16// 12


#include	"UnBackground.h"

#include	"BnImage/BnImage.h"

#include "ImageLabel/ImageLabel.h"


#include "bImage.h"




int	CUnBackground::ProcessBlob( image_type *sim )
{
	int	i,	j;
	int	T0,	T1;

	T0 = -18;
	T1 = 18;

	image_type *im = image_alloc( sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );


	s_char *sp = (s_char *)sim->data;
	u_char *tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, tp++ ){
			if( *sp > T0 && *sp < T1 )
				*tp = 0;
			else *tp = ABS( *sp );
		}
	}


	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM" );


	m_abw = imageLabelUI_T( im, 1, 0,  m_abw );
	imageLabelUI_svalue( m_abw, sim );


	im = imageLabelUI_color_image( m_abw, 0, im );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM-0" );

	imageLabelUI_set_box( m_abw );

	//imageLabel_print( m_abw, stdout );

	for( i = 0 ; i < m_abw->nA ; i++ ){
		bwLabel_type *bw = &m_abw->a[i];
		if( bw->id != i )	continue;

		if( bw->color == 1 && bw->no < 20 && bw->av >= -20.5 && bw->var < 2.5 ){
			int k = imageLabelUI_nieg1( m_abw, i );
			if( k >= 0 )
				imageLabelUI_merge( m_abw, i, k );
		}
	}



	im = bImage_to_image1(m_bim, im );
	imageLabelUI_value( m_abw, im );

	//imageLabel_print( m_abw, stdout );


	im = imageLabelUI_color_image( m_abw, 0, im );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM-1" );


	im = imageLabelUI_imageID( m_abw, im );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM-ID-1" );

	image_destroy( im, 1  );

	return( 1 );
}

