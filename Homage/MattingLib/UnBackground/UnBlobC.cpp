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



int	imageLabelUI_blobe_seg( imageLabel_type *abw, int i0, int j0, int j1, int b, sbA_type *as );

int	imageLabelUI_blobe_seg_color( imageLabel_type *abw, int i0, int j0, int j1, int color, sbA_type *as );


int	imageLabelUI_blobe0( imageLabel_type *abw, int i0, int j0, int dj, int *k0, int *b0, int *k1, int *b1 );





int	CUnBackground::ProcessBlobC()
{
	int	i;

	int b0,	b1,	k0,	k1;
	imageLabelUI_blobe0( m_abw, m_hp.x/8, m_hp.y/8, m_hd[0]/8, &k0, &b0, &k1, &b1 );
	m_iHead = 0;
	if( k1 < 0 || k0 < 0 || m_hp.y - 8*k0 > 1.5*m_hd[0] && 8*k1 - m_hp.y > 1.5*m_hd[0] ){

		imageLabelUI_blobe0( m_abw, m_hp.x/8, m_hp.y/8, m_hd[1]/8, &k0, &b0, &k1, &b1 );
		if( k1 < 0 || k0 < 0 || m_hp.y - 8*k0 > 1.5*m_hd[1] || 8*k1 - m_hp.y > 1.5*m_hd[1] )
			return( -1 );

		m_iHead = 1;
	}









	u_char T[6] = { 255, 0, 128, 64 };


//	image_type *im = imageLabelUI_color_image( m_abw, 0, NULL );
	image_type *im = imageLabelUI_color_imageT( m_abw, T, NULL );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM-2" );



	im = imageLabelUI_imageID( m_abw, im );
	IMAGE_DUMP_DUP( im, 8, 1, "ub", 1, "DM-ID-2" );




	int	i0 = m_hp.x/8;
	ProcessBlobSide( m_abw, b0, m_hp.x/8, k0, 0, m_hp.y/8, &m_as0 );
	ProcessBlobSide_test( m_abw, b0, 0, m_hp.y/8, &m_as0 );
#ifdef _DEBUG
//	sbA_write( &m_as0, stdout );
#endif

	ProcessBlobSide( m_abw, b1, m_hp.x/8, k1, m_hp.y/8, m_abw->im->width, &m_as1 );
	ProcessBlobSide_test( m_abw, b1, m_hp.y/8, m_abw->im->width, &m_as1 );
#ifdef _DEBUG
//	sbA_write( &m_as1, stdout );
#endif

	int n  = ( m_as1.a[i0].j0 - m_as0.a[i0].j1 );
	if( n*8 > 100 )	m_closeUp = 1;

#ifdef _DEBUG
	sbA_write3( &m_as0, &m_asB[0], &m_as1, stdout );
#endif


	ProcessBlobSide_gap(  m_abw, b0, 0, m_hp.y/8, &m_as0 );
	ProcessBlobSide_gap(   m_abw, b1, m_hp.y/8, m_abw->im->width, &m_as1 );

#ifdef _DEBUG
	sbA_write3( &m_as0, &m_asB[0], &m_as1, stdout );
#endif


	for( i = 0 ; i < m_as0.nA ; i++ ){
		if( m_as0.a[i].j1 != m_as1.a[i].j0-1 )
			break;

		m_as0.a[i].j1 = m_as1.a[i].j0 = -1;
	}


	image_destroy( im, 1 );

	return( 1 );
}




int	CUnBackground::ProcessBlobSide( imageLabel_type *abw, int iB, int i0, int j00, int j0, int j1, sbA_type *as )
{
	sbA_type ac;
	int	i;
	int	a[100],	nA;

	as->nA = abw->im->height;
	for( i = 0 ; i < as->nA ; i++ )
		as->a[i].j0 = as->a[i].j1 = -1;

	imageLabelUI_blobe_seg( abw, i0, j0, j1, iB, &ac );


	int k = sbA_in( &ac, j00 );

	if( k < 0 ){
		return( -1 );
	}

	as->a[i0] = ac.a[k];

	for( i = i0+1 ; i < abw->im->height ; i++ ){
		imageLabelUI_blobe_seg( abw, i, j0, j1, iB, &ac );
		if( ac.nA == 0 )
			break;

		sbA_overlap( &ac, &as->a[i-1], a, &nA );
		if( nA == 0 ){
			if( j0 == 0 )
				as->a[i] = ac.a[ac.nA-1];
			else 	as->a[i] = ac.a[0];
			continue;
		}

		if( j0 == 0 )
			as->a[i] = ac.a[a[nA-1]];
		else as->a[i] = ac.a[0];
	}

	for( i = i0-1 ; i >= 0 ; i-- ){
		imageLabelUI_blobe_seg( abw, i, j0, j1, iB, &ac );
		if( ac.nA == 0 )
			break;

		sbA_overlap( &ac, &as->a[i+1], a, &nA );
		if( nA == 0 ){
			if( j0 == 0 )
				as->a[i] = ac.a[ac.nA-1];
			else 	as->a[i] = ac.a[0];
			continue;
		}


		if( j0 == 0 )
			as->a[i] = ac.a[a[nA-1]];
		else
			as->a[i] = ac.a[a[0]];
	}



	return( 1 );
}

int	CUnBackground::ProcessBlobSide_gap( imageLabel_type *abw, int iB, int j0, int j1, sbA_type *as )
{
	sbA_type ac;

	int	i,	k;




	for(  i = 0 ; i < as->nA ; i++ ){
		if( as->a[i].j0 >= 0 )	continue;

		imageLabelUI_blobe_seg_color( abw, i, j0, j1, 0, &ac );

		if( ac.nA == 0 )	continue;;

		int id = ac.a[0].id;
		int	k0 = 0;
		for( k = 1 ; k < ac.nA ; k++ ){
			if( m_abw->a[id].no < m_abw->a[ac.a[k].id].no ){
				id = ac.a[k].id;
				k0 = k;
			}
		}

		if( m_abw->a[id].no < 100 )	continue;


		if( m_closeUp ){
			if( j0 > 0 ){
				if(  i > as->nA - 15 && as->a[i-1].j0 > ac.a[k0].j1 )
					break;
			}
		
			else {
				if(  i > as->nA - 15 && as->a[i-1].j0 > ac.a[k0].j1 )
					break;
			}
		}

		as->a[i] = ac.a[k0];

		ProcessBlobSide_overlap_down( abw, id, i,  j0,  j1,  as );


	}



	return( 1 );
}


//
//int	CUnBackground::ProcessBlobSide_filter_right( sbA_type *as )
//{
//int	i;
//	for( i = as ->nA-15 ; i < as ->nA ; i++ ){
//		if( as->a[i-1].j1 < 0 || as->a[i].j1 < 0 )	continue;
//
//	}
//
//	return( 1 );
//}


int	CUnBackground::ProcessBlobSide_test( imageLabel_type *abw, int iB, int j0, int j1, sbA_type *as )
{
int	i;
	


	for( i = 1 ; i < as ->nA ; i++ ){
		if( as->a[i-1].j1 < 0 )	continue;

		if( seg_overlap( &as->a[i], &as->a[i-1]) > 0 )
			continue;

		
		ProcessBlobSide_overlap_up( abw, iB, i,  j0,  j1,  as );
	}


	for( i = as ->nA-2 ; i > 0 ; i-- ){
		if( as->a[i-1].j1 < 0 )	continue;

		if( seg_overlap( &as->a[i], &as->a[i+1]) > 0 )
			continue;


		ProcessBlobSide_overlap_down( abw, iB, i,  j0,  j1,  as );
	}


	return( 1 );
}



int	CUnBackground::ProcessBlobSide_overlap_up( imageLabel_type *abw, int iB, int i0, int j0, int j1, sbA_type *as )
{
	sbA_type ac;

	int	a[100],	nA;
	int	i;



	for( i = i0-1 ; i >= 0 ; i-- ){
		imageLabelUI_blobe_seg( abw, i, j0, j1, iB, &ac );
		if( ac.nA == 0 )
			break;

		sb_type	sb;
		sbA_overlap( &ac, &as->a[i+1], a, &nA );
		if( nA == 0 ){
			break;
		}

		if( j0 == 0 )
			sb = ac.a[a[nA-1]];
		else
			sb = ac.a[a[0]];

		if( as->a[i].j0 == sb.j0 && as->a[i].j1 == sb.j1 )
			break;

		as->a[i] = sb;
	}



	return( 1 );
}


int	CUnBackground::ProcessBlobSide_overlap_down( imageLabel_type *abw, int iB, int i0, int j0, int j1, sbA_type *as )
{
	sbA_type ac;

	int	a[100],	nA;
	int	i;



	for( i = i0+1 ; i < as->nA ; i++ ){

		if( as->a[i].j0 >= 0 && as->a[i].id != as->a[i-1].id )
			break;

		imageLabelUI_blobe_seg( abw, i, j0, j1, iB, &ac );
		if( ac.nA == 0 )
			break;


		sbA_overlap( &ac, &as->a[i-1], a, &nA );
		if( nA == 0 ){
			break;
		}

		sb_type	sb;
		if( j0 == 0 )
			sb = ac.a[a[nA-1]];
		else
			sb = ac.a[a[0]];

		if( as->a[i].j0 == sb.j0 && as->a[i].j1 == sb.j1 )
			break;

		as->a[i] = sb;
	}



	return( 1 );
}


int
imageLabelUI_blobe0( imageLabel_type *abw, int i0, int j0, int dj, int *k0, int *b0, int *k1,int *b1 )
{


	int	j;


	*b0 = *b1 = -1;
	*k0 = *k1 = -1;

	u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i0, j0-dj );
	for( j = j0-dj ; j > j0 - 4*dj ; j--, sp-- ){
		if( abw->a[*sp].color == 0 && abw->a[*sp].no > 150 ){
			*b0 = abw->a[*sp].id;
			*k0 = j;
			break;
		}
	}


	sp = (u_int *)IMAGE_PIXEL( abw->im, i0, j0+dj );
	for( j = j0+dj ; j < j0 + 4*dj ; j++, sp++ ){
		if( abw->a[*sp].color == 0 && abw->a[*sp].no > 150  ){
			*b1 = abw->a[*sp].id;
			*k1 = j;
			break;
		}
	}



	return( 1 );
}




int
imageLabelUI_blobe_sb( imageLabel_type *abw, int iB, sbA_type *as )
{
int	i;
sbA_type ac;

	bwLabel_type *bw = &abw->a[iB];

	for( i = 0 ; i < bw->b.y0 ; i++ )
		as->a[i].j0 = as->a[i].j1 = -1;


	for( i = bw->b.y0 ; i < bw->b.y1 ; i++ ){
		imageLabelUI_blobe_seg( abw, i, bw->b.x0, bw->b.x1, iB, &ac );
		if( ac.nA == 0 ){
			as->a[i].j0 = as->a[i].j1 = 0;
			continue;
		}

		int	k,	k0;
		for( k = 1, k0 = 0 ; k < ac.nA ; k++ ){
			if( ac.a[k].j1 - ac.a[k].j0 > ac.a[k].j1 - ac.a[k].j0 )
				k0 = k;
		}
			

		as->a[i].j0 = ac.a[k0].j0;
		as->a[i].j1 = ac.a[k0].j1;


	}

	for( i = bw->b.y1 ; i < abw->im->height ; i++ )
		as->a[i].j0 = as->a[i].j1 = -1;


	as->nA = abw->im->height;


		return( 1 );
}


int
	imageLabelUI_blobe_sbA( imageLabel_type *abw, int iB, sbA_type *as )
{
	int	i;
	sbA_type ac;

	bwLabel_type *bw = &abw->a[iB];

	for( i = 0 ; i < bw->b.y0 ; i++ )
		as->a[i].j0 = as->a[i].j1 = -1;


	for( i = bw->b.y0 ; i < bw->b.y1 ; i++ ){
		imageLabelUI_blobe_seg( abw, i, bw->b.x0, bw->b.x1, iB, &ac );
		if( ac.nA == 0 ){
			as->a[i].j0 = as->a[i].j1 = -1;
			continue;
		}

		as->a[i].j0 = ac.a[0].j0;
		as->a[i].j1 = ac.a[ac.nA-1].j1;


	}

	for( i = bw->b.y1 ; i < abw->im->height ; i++ )
		as->a[i].j0 = as->a[i].j1 = -1;


	as->nA = abw->im->height;


	return( 1 );
}



int
imageLabelUI_blobe_seg( imageLabel_type *abw, int i0, int j0, int j1, int b, sbA_type *as )
{
	int	j;

	as->nA = 0;
	sb_type *s = &as->a[as->nA];
	s->j0 = s->j1 = -1;

	u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i0, j0 );
	for( j = j0 ; j < j1 ; j++, sp++ ){
		if( *sp == b ){
			if( s->j0 <= 0 ){
				s->j0 = j;
				s->id = b;
			}
			s->j1 = j;
			continue;
		}

		if( s->j1 > 0 ){
			as->nA++ ;
			s = &as->a[as->nA];
			s->j0 = s->j1 = -1;
		}
	
	}

	if( s->j1 > 0 )
		as->nA++ ;


	return( as->nA );
}



int
imageLabelUI_blobe_seg_color( imageLabel_type *abw, int i0, int j0, int j1, int color, sbA_type *as )
{
	int	j;

	as->nA = 0;
	sb_type *s = &as->a[as->nA];
	s->j0 = s->j1 = -1;

	u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i0, j0 );
	for( j = j0 ; j < j1 ; j++, sp++ ){
		if( abw->a[*sp].color == color ){
			if( s->j0 <= 0 ){
				s->j0 = j;
				s->id = abw->a[*sp].id;
			}

			s->j1 = j;
			continue;
		}

		if( s->j1 > 0 ){
			as->nA++ ;
			s = &as->a[as->nA];
			s->j0 = s->j1 = -1;
		}

	}

	if( s->j1 > 0 )
		as->nA++ ;


	return( as->nA );
}
		