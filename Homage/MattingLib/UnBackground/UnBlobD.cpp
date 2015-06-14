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
#include "Imagelabel/BwLabel.h"
#include "ImageLabel/ImageLabel.h"
#include "Histogram/Histogram.h"


#include	"UnBackground.h"

#include "bImage.h"




int	CUnBackground::ProcessBlobD()
{


	ProcessBlobD_figure_blob( m_abw, &m_asFigure );
	u_char T[6] = { 255, 0, 144, 96, 64  };



	image_type *im = imageLabelUI_color_imageT( m_abw, T, NULL );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM-3" );

	image_destroy( im, 1 );


	m_apt = ProcessBlobD_point(  m_abw, m_apt );
	PT2DA_DUMP( m_apt, "ub", 1, "DM-3" );

	return( 1 );
}




int	CUnBackground::ProcessBlobD_figure_blob( imageLabel_type *abw, sbA_type *as )
{
	int	i,	j;


	u_int *bp = abw->im->data_ui;
	for( i = 0 ; i < abw->im->height ; i++ ){
	
		int	j0,	j1;

		if(as->a[i].j0 == -1 && as->a[i].j1 == -1)
			continue;

		j0 = ( as->a[i].j0 < 0 )? 0 : as->a[i].j0;


		j1 = ( as->a[i].j1 < 0 )? abw->im->width : as->a[i].j1;


		u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i, j0 );

		
		for( j = j0 ; j < j1 ; j++, sp++ ){
			bwLabel_type *bw = &abw->a[*sp];

			if( bw->color == 0 )	continue;
			if( bw->color > 2 )	continue;

			bw->color += 2;
		}
	}

	return( 1 );
}



pt2dA_type *	CUnBackground::ProcessBlobD_point( imageLabel_type *abw, pt2dA_type *apt )
{
	int	i,	j;

	apt = pt2dA_realloc( apt, abw->im->width * abw->im->height/4 );

	u_char T[] = { 0, 1, 1, 0, 0, 0 };


	m_imMask = imageLabelUI_color_imageT( m_abw, T, m_imMask );




	
	for( i = 2 ; i < m_imMask->height-1 ; i += 4 ){

		u_char *sp = (u_char *)IMAGE_PIXEL( m_imMask, i, 2 );
		
		sp -= 1;
		u_char *sp0 = sp - abw->im->width;
		u_char *sp1 = sp + abw->im->width;


		for( j = 2 ; j < m_imMask->width-1 ; j += 4, sp += 4, sp1 += 4, sp0 += 4 ){

		//	if( *sp != 1 && *sp != 2 )	continue;

			int n = ( sp0[0] +  sp0[1] +  sp0[2]  + 
				sp[0] +  sp[1] +  sp[2] + 
				sp1[0] +  sp1[1] +  sp1[2] );

			if( n < 9 )	continue;


			pt2d_type *pt = &apt->a[apt->nA++];

			pt->p.x = ( j+0.5)*8;
			pt->p.y = ( i+0.5)*8;
		}
	}

	

	return( apt );
}
