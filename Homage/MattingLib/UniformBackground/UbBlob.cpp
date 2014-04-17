
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



#define EXCEPTION



static void	imageLabelUS_edge( imageLabel_type *abw, image_type *eim );






int	CUniformBackground::ProcessBlob()
{

	gpTime_start( &m_tBlobR );
	image1_remove_blobM( m_cim, 255, 100*100, 1, 0 );
	gpTime_stop( &m_tBlobR );




	gpTime_start( &m_tBlobF );

	gpTime_start( &m_tBlobEdge );
	m_eim = image1_gridient_sobol_value( m_yim, m_eim );
	gpTime_stop( &m_tBlobEdge );

	//if( m_dFrame == m_iFrame ){
	//	IMAGE_DUMP( m_eim, "edge", m_iFrame, NULL );
	//}

//	image1_fill_blobV( m_dim, m_cim, 255, 100*100, 4, m_T*0.5, 255 );

	float vT = m_T*0.5;

	m_abw = imageLabelUS( m_cim, 255, 1, m_abw );
	
	
	imageLabel2_set_boundary( m_abw->im, m_abw->a, m_abw->nA );
	imageLabel2_set_value( m_dim, m_abw->im, m_abw->a, m_abw->nA );

	
	gpTime_start( &m_tBlobEdge );
	imageLabelUS_edge( m_abw, m_eim );
	gpTime_stop( &m_tBlobEdge );




	int	i;
	for( i = 0 ; i < m_abw->nA ; i++ ){
		bwLabel_type *bw = &m_abw->a[i];
		if( bw->id != i )	continue;
//		bw->existence = 0;
		
		if( bw->no > 120*120 )	
			continue;

		if( bw->av < vT &&  bw->no > 100 )
			continue;

		if( bw->nE < 0.65*bw->nB ){
			bw->id = -1;
			continue;
		}

		if( bw->boundary == 1 )
			continue;
//			bw->existence = 1;


//			bw->existence = 1;

		bw->id = -1;
	}



	imageLabelUS_set_pixel( m_cim, m_abw, 255 );



	gpTime_stop( &m_tBlobF );

	return( 1 );
}



static void
imageLabelUS_edge( imageLabel_type *abw, image_type *eim )
{
	int	i,	j;


	for( i = 0 ; i < abw->nA ; i++ )
		abw->a[i].nB = abw->a[i].nE = 0;

	u_char *ep = IMAGE_PIXEL( eim, 1, 1 );
	short *bp = (short *)IMAGE_PIXEL( abw->im, 1, 1 );

	for( i = 1 ; i < abw->im->row-1 ; i++, bp += 2, ep += 2 ){
		short *bp0 = bp - abw->im->width;
		short *bp1 = bp + abw->im->width;

		for( j = 1 ; j < abw->im->column-1 ; j ++, bp++, bp0++, bp1++, ep++ ){

			bwLabel_type *bw = &abw->a[*bp];
			if( bw->id < 0 )	continue;

			int n = *(bp0-1) + *(bp0) + *(bp0+1) + 
				*(bp-1) +  *(bp+1) +
				*(bp1-1) + *(bp1) + *(bp1+1);

			if( (n >> 3) == *bp )
				continue;

			bw->nB ++;
			if( *ep > 8 )
				bw->nE ++;
		}
	}
}