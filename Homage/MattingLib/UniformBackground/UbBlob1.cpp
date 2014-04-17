
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






int	imageLabelUI_remove_A( image_type *sim, imageLabel_type *abw, int color, int nT );

int	imageLabelUI_remove_B( image_type *sim, imageLabel_type *abw, int color, int nT );






int	CUniformBackground::ProcessBlob1()
{

	gpTime_start( &m_tBlob );

	m_abwBlob = imageLabelUI_T( m_cim, 1, 0, m_abwBlob );

	imageLabelUI_remove_A( m_cim,  m_abwBlob, 1, 1 );

	imageLabelUI_remove_B( m_cim,  m_abwBlob, 0, 120*120 );



	gpTime_stop( &m_tBlob );

	return( 1 );
}







#ifdef _AA_
int
	imageLabelUI_test( image_type *sim )
{
	imageLabel_type *abw = NULL;


	abw = imageLabelUI_T( sim, 1, 0, abw );



	imageLabelUI_remove_A( sim,  abw, 1, 1 );

	imageLabelUI_remove_B( sim,  abw, 0, 1000 );


	//int no = imageLabel_no( abw );
	//fprintf( stdout, "blob: %d\n", no );



	return( 1 );
}
#endif


void	imageLabelUI_remove( image_type *sim, imageLabel_type *abw, int color );

int	imageLabelUI_neig( imageLabel_type *abw, int k );



int
	imageLabelUI_remove_A( image_type *sim, imageLabel_type *abw, int color, int nT )
{

	int n = imageLabel_bigest( abw, 1 );


	int	i;
	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		bw->existence = 0;
		if( bw->id != i )	continue;
		if( bw->color != 1 )	continue;
		if( i == n )	continue;

		int k = imageLabelUI_neig( abw, i );
		if( k >= 0 ){
			bwLabel_type *bw1 = &abw->a[k];
			bw1->no += bw->no;
			bw->id = bw1->id;
			bw->existence = -1;
		}
	}

	imageLabelUI_remove( sim, abw, 0 );

	imageLabelUI_set_id( abw );


	return( 1 );
}

int
	imageLabelUI_remove_B( image_type *sim, imageLabel_type *abw, int color, int nT )
{



	int	i;
	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		bw->existence = 0;
		if( bw->id != i )	continue;
		if( bw->color != color )	continue;
		if( bw->no > nT )	continue;

		int k = imageLabelUI_neig( abw, i );
		if( k >= 0 ){
			bwLabel_type *bw1 = &abw->a[k];
			bw1->no += bw->no;
			bw->id = bw1->id;
			bw->existence = -1;
		}
	}

	imageLabelUI_remove( sim, abw, 255*(1-color) );

	imageLabelUI_set_id( abw );


	return( 1 );
}



int
	imageLabelUI_neig( imageLabel_type *abw, int k )
{
	int	i,	j,	k1;
	bwLabel_type *bw = &abw->a[k];

	i = bw->i;
	j = bw->j;

	u_int *sp = (u_int *)IMAGE_PIXEL( abw->im, i, j );
	if( j > 0 ){
		k1 = *(sp-1);
		return( k1 );
	}

	if( i > 0 ){
		sp -= abw->im->width;
		k1 = *sp;
		return( k1 );
	}
	return( -1 );
}



void 
imageLabelUI_remove( image_type *sim, imageLabel_type *abw, int color )
{
	int	i,	j;

	u_char *sp = sim->data;

	u_int	*tp = abw->im->data_ui + abw->margin*(abw->mim->width + 1);
	for( i = 0 ; i < sim->height ; i++, tp += 2*abw->margin ){
		for( j = 0 ; j < sim->width ; j++, sp++, tp++ )
			if( abw->a[*tp].existence < 0 )
				*sp = color;
	}
}
