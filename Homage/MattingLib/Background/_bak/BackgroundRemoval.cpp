
//#ifdef _DEBUG
#define _DUMP
//#endif

#include	<string.h>
#include	<math.h>

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"

#include "Wutil/UWgpw/GpwType.h"



#include	"Finger.h"
#include "Image2/BwLabel.h"


static image_type *	image1_connect_component( image_type *sim, int T, bwLabel_type **abw, int *nBw, image_type *im );

static int	bwLabel_find( bwLabel_type *abw, int nB, image_type *mim, int g );




int			
CFinger::ProcessBacground( image_type *sim )
{
	if( m_iFrame < 10 ){
		if( m_bg == 3 )
			m_bim = image_make_copy( sim, m_bim );
		else
			m_bim = image_make_copy( m_yim, m_bim );
		IMAGE_DUMP( m_bim, "bim", 1+ m_orient*1000, NULL );
		return( -1 );
	}

	if( m_bim == NULL )
		return( -1 );

	gpTime_start( &m_gtBg );
	if( m_dFrame == m_iFrame ){
		IMAGE_DUMP( m_yim, "yim", 1+ m_orient*1000, NULL );
	}


	if( m_bg == 3 )
		m_dim = image3_abs_diff_image1( sim, m_bim, m_dim );
	else
		m_dim = image1_abs_diff( m_yim, m_bim, m_dim );


	//if( m_mim != NULL )
	//	image1_mask( m_im, m_mim );


	image1_threshold( m_dim, 32 );



	if( m_iFrame == m_dFrame ){
		IMAGE_DUMP( m_dim, "im", m_dFrame+ m_orient*1000, "bg" );
	}


	if( m_bg & 0x08 )
		FindLine();

	gpTime_stop( &m_gtBg );

	return( 1 );
}



int			
CFinger::FindLine()
{
image_type	*im;
bwLabel_type *abw;
int nBw;
int	j,	k;

	m_nBw = 3;
	m_abw[0].no = m_abw[1].no = m_abw[2].no = 0;

	image1_color( m_im, 0, 0, 0, 1280, 60 );

	im = image1_connect_component( m_im, 16, &abw, &nBw, NULL );
	if( im == NULL )
		return( m_nBw );



	m_nBw = 0;
	for( k = 0; k < 3 ; k++ ){
		j = bwLabel_find( abw, nBw, m_mim, k+1 );
		if( j < 0 ){
			m_abw[k].no = 0;
			continue;
		}

		m_abw[k] = abw[j];
		abw[j].no = 0;
	}
	m_nBw = 3;


	image_destroy( im, 1 );
	free( abw );

	return( m_nBw );
}




static int
bwLabel_find( bwLabel_type *abw, int nB, image_type *mim, int g )
{
int	i,	j,	i0,	j0;

	for( i = 1, j = -1 ; i < nB ; i++ ){
		if( abw[i].id != i ||  abw[i].no == 0 )	continue;
		if( j >= 0 && abw[j].no > abw[i].no )
			continue;

		if( mim != NULL ){
			j0 = abw[i].p.x;
			i0 = abw[i].p.y;

			u_char *sp = IMAGE_PIXEL( mim, i0, j0 );

			if( *sp != g )
				continue;
		}

		j = i;
	}

	return( j );
}



static image_type *
image1_connect_component( image_type *sim, int T, bwLabel_type **abw, int *nBw, image_type *im )
{
	int	i,	j;
	u_char *sp;
	short *bp;

	im = image_create( sim->height, sim->width, 2, 1, NULL );

	sp = sim->data;
	bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < T )? 0 :1;


	image2_bwLabel( im, abw, nBw );
	if( *nBw <= 1 ){
		free( *abw );
		image_destroy( im, 1);
		return( NULL );
	}

	image2_bwLabel_set_id( im, *abw );


	image2_bwLabel_set_direction( im, *abw, *nBw );


	return( im );
}




void CFinger::DrawBackground( gpw_type *gpw )
{
int	i;

vec2f_type	p0,	p1;

	for( i = 0; i < m_nBw ; i++ ){

		if( m_abw[i].no == 0 )	break;

		p0.x = m_abw[i].p.x - 20 * m_abw[i].v1.x;
		p0.y = m_abw[i].p.y - 20 * m_abw[i].v1.y;

		p1.x = m_abw[i].p.x + 20 * m_abw[i].v1.x;
		p1.y = m_abw[i].p.y + 20 * m_abw[i].v1.y;

		gpw_line( gpw, p0.x, p0.y, p1.x, p1.y, RED );
	}
}
