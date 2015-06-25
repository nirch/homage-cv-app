
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"
#include "ImageLabel/ImageLabel.h"



static image_type *	image1_sample2N( image_type *sim, image_type *im );

static void	image1_set( image_type *sim, image_type *im );


static int	imageLabelUS_remove_A( image_type *sim, imageLabel_type *abw, int color, vec2f_type *mp );


static int	imageLabelUS_remove_B( image_type *sim, imageLabel_type *abw, int nT, int fillAll, int color );


static void	imageLabelUS_remove( image_type *sim, imageLabel_type *abw, int color );


void	imageLabelUS2_value( imageLabel_type *abw, image_type *sim );




int	CUniformBackground::ProcessBlob2()
{

	gpTime_start( &m_tBlob );

	image_type *im = image1_sample2N( m_cim, NULL );


	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_SCALE( im, 32, "blob", m_iFrame, "1" );
	}

	// remove
	m_abwBlob = imageLabelUS_N( im, 4, 1, 0, m_abwBlob );


	imageLabelUS_remove_A( im,  m_abwBlob, 0x5, &m_mp );


	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_SCALE( im, 32, "blob", m_iFrame, "2" );
	}

	// fill
	m_abwBlob = imageLabelUS_N( im, 1, 0, 0, m_abwBlob );

	imageLabelUS2_value( m_abwBlob, m_dim );
	imageLabelUS_remove_B( im,  m_abwBlob, m_prm->fillBlob/4, m_prm->fillBlobAll, 0x06 );


	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_SCALE( im, 32,"blob", m_iFrame, "3" );
	}


	image1_set( m_cim, im );

	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP_SCALE( im, 32,"blob", m_iFrame, "4" );
		IMAGE_DUMP( m_cim,"blob", m_iFrame, "5" );
	}

	image_destroy( im, 1 );

	gpTime_stop( &m_tBlob );

	return( 1 );
}




static image_type *
	image1_sample2N( image_type *sim, image_type *im )
{
	u_char	*sp0, *sp2,	*tp;
	int	i,	j;


	im = image_recreate( im, sim->row/2, sim->column/2, 1, 1 );


	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 2*i, 0 );
		sp2 = sp0 + sim->width;


		for( j = 0 ; j < im->column ; j++ ){

			int tmp =  ( *(sp0) + sp0[1] + *(sp2) + sp2[1] + 4 ) >>8;


			*tp++ = 4 - tmp;

			sp0 += 2;			
			sp2 += 2;

		}
	}

	return( im );
}


static void
	image1_set( image_type *sim, image_type *im )
{
	u_char	*sp0, *sp2,	*mp;
	int	i,	j;



	mp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 2*i, 0 );
		sp2 = sp0 + sim->width;


		for( j = 0 ; j < im->column ; j++, sp0 += 2, sp2 += 2, mp++ ){

			if( *mp < 0x5 )		continue;

			int val = ( *mp == 0x05 ) ? 0 : 255;

			sp0[0] = sp0[1] = sp2[0] = sp2[1] = val;
		}
	}
}




static int	imageLabel_main_blob( imageLabel_type *abw, int color, vec2f_type *mp );

static int
	imageLabelUS_remove_A( image_type *sim, imageLabel_type *abw, int color, vec2f_type *mp )
{
	imageLabelUS_set_mass( abw );

//	int n = imageLabel_bigest( abw, 1 );

	int n = imageLabel_main_blob( abw, 1, mp );
	if( n < 0 )	return( 1 );

	*mp = abw->a[n].p;


	int	i;
	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		bw->existence = 0;
		if( bw->id != i )	continue;
		if( i == n || i == 0 )	continue;

		bw->existence = -1;
	}

	imageLabelUS_remove( sim, abw, color );


	return( 1 );
}

static int
imageLabel_main_blob( imageLabel_type *abw, int color, vec2f_type *mp )
{
int	a[6],	nA,	i;


	bwLabelA_order( abw, color, -1, 4, a, &nA );

	if( nA < 0 )	return( -1 );
	if( nA == 1 )	return( a[0] );

	bwLabel_type *bw0 = &abw->a[a[0]];
	int	i0 = -1;
	float dMin = 0 ;

	for( i = 0 ; i < nA ; i++ ){
		vec2f_type p;
		bwLabel_type *bw = &abw->a[a[i]];
		if( bw->no < 0.6 * bw0->no )
			return( i0 );

		p.x = bw->p.x - mp->x;
		p.y = bw->p.y - mp->y;
		float d = p.x*p.x + p.y*p.y;

		if( i0 < 0 || d < dMin ){
			dMin = d;
			i0 = a[i];
		}
	}
	
	return( i0 );
}


static int
	imageLabelUS_remove_B( image_type *sim, imageLabel_type *abw, int nT, int fillAll, int color )
{
//	fillAll = 0;

	imageLabelUS_set_boundary( abw );

	float vT = 12;
	int	i;
	for( i = 1 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		bw->existence = 0;
		if( bw->id != i )	continue;

		if( bw->boundary > 0 && bw->boundary != 3 )
			continue;

		if( bw->no > sim->width*sim->height/8 )
			continue;

		if( fillAll == 0 ){
			//if( bw->no > nT ||  (fillAll == 0 && bw->av < vT)  )
			if( bw->no > nT || bw->av < vT )
				continue;
		}

		bw->existence = -1;

	}

	imageLabelUS_remove( sim, abw, color );


	return( 1 );
}





static void 
	imageLabelUS_remove( image_type *sim, imageLabel_type *abw, int color )
{
	int	i,	j;

	u_char *sp = sim->data;

	u_short	*tp = abw->im->data_us + abw->margin*(abw->im->width + 1);
	for( i = 0 ; i < sim->height ; i++, tp += 2*abw->margin ){
		for( j = 0 ; j < sim->width ; j++, sp++, tp++ )
			if( abw->a[*tp].existence < 0 )
				*sp = color;
	}
}









void
imageLabelUS2_value( imageLabel_type *abw, image_type *sim )
{
	int	i,	j;
	short	*bp;
	u_char	*sp;

	bwLabel_type *bw;

	for( i = 0 ; i < abw->nA ; i++ ){
		abw->a[i].no = 0;
		abw->a[i].av = 0;
		abw->a[i].var = 0;
	}



	sp = sim->data;
	bp = abw->im->data_s;
	for( i = 0 ; i < abw->im->height ; i++ ){
		u_char *sp0 = IMAGE_PIXEL( sim, 2*i, 0 );
		u_char *sp2 = sp0 + sim->width;


		for( j = 0 ; j < abw->im->width ; j++, bp++, sp0 += 2, sp2 += 2 ){
			bw = &abw->a[*bp];
			int val = ( sp0[0] + sp0[1] + sp2[0] + sp2[1])>> 2;
			bw->no++;
			bw->av += val;
//			bw->var += val * val;
		}
	}



	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];

		if( bw->no == 0  )	continue;

		bw->av /= bw->no;
//		bw->var = bw->var / bw->no - bw->av*bw->av;
	}
}