
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"

#include "ImageMorphology/ImageMorphology.h"


#define EXCEPTION



void		bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );

image_type *	bImage_set( image_type *sim, image_type *mim, box2i_type *b, int n, image_type *bim );

image_type *	bImage_set( image_type *sim, box2i_type *b, int N, image_type *bim );




int			bImage_update( image_type *bim, image_type *cim, float T );


int			bImage_bnMask( image_type *bim, image_type *bnIm );





int	CUniformBackground::ProcessUpdate( image_type *sim )
{
	m_bimC = bImage_set( sim, m_cim, &m_roi, m_N, m_bimC );


	if( m_bnImD != NULL )
		bImage_bnMask( m_bimC, m_bnImD );

//	bImage_fill_1( m_bimC );

	bImage_update( m_bim, m_bimC, m_bT );

	if( m_iFrame >= m_dFrame-2 && m_iFrame <= m_dFrame +2)
		bImage_dump( m_bim, m_N, "bim", m_iFrame, NULL );


//	bImage_complete( m_bim );

	return( 1 );
}










int	bImage_update_1( image_type *bim, image_type *cbim );


int
bImage_update( image_type *bim, image_type *cim, float T )
{
	int	i,	j,	no;
	bImage_type *bp,	*cp;




	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	cp = ( bImage_type *)IMAGE_PIXEL( cim, 1, 1 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += 2, cp += 2 ){

		for( j = 1 ; j < bim->width-1 ; j++, bp++, cp++ ){
			if( cp->n == 0 )		continue;

			int dr = cp->r - bp->r;
			int dg = cp->g - bp->g;
			int db = cp->b - bp->b;

			int d = ABS(dr);
			if( dg < 0 )	dg = -dg;
			if( d < dg )	d = dg;

			if( db < 0 )	db = -db;
			if( d < db )	d = db;


			if( d < T )
				cp->n = 1;
			else	cp->n = 0;

		}
	}



	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	cp = ( bImage_type *)IMAGE_PIXEL( cim, 1, 1 );
	for( i = 1, no = 0 ; i < bim->height-1 ; i++, bp += 2, cp += 2 ){
		bImage_type *cp0 = cp - bim->width;
		bImage_type *cp1 = cp + bim->width;

		for( j = 1 ; j < bim->width-1 ; j++, bp++, cp++, cp0++, cp1++ ){
			if( cp->n  == 0 )		continue;

			//int n =  (cp0-1)->n + (cp0)->n + (cp0+1)->n +
			//	     (cp-1)->n  + (cp)->n  + (cp+1)->n  +
			//	     (cp1-1)->n + (cp1)->n + (cp1+1)->n;

			//
			//if( n != 9 )	continue;

			bp->r = cp->r;
			bp->g = cp->g;
			bp->b = cp->b;

			bp->n = 1;

			no++;
		}
	}



	bImage_update_1( bim, cim );
	
//	fprintf( stdout, "%d", no );
	return( no );
}



int
bImage_bnMask( image_type *bim, image_type *bnIm )
{
	int	i,	j;
	bImage_type *bp;



	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	u_char	*mp = bnIm->data;
	for( i = 1 ; i < bim->height-1 ; i++, bp += 2 ){

		for( j = 1 ; j < bim->width-1 ; j++, bp++, mp++ ){
			if( *mp != 0 )
				bp->n = 0;
		}
	}

	return( 1 );
}


int
bImage_update_1( image_type *bim, image_type *cbim )
{
	int	i,	j,	n,	k,	no;
	bImage_type *bp,	*cp;



	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	cp = ( bImage_type *)IMAGE_PIXEL( cbim, 1, 1 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += 2, cp += 2 ){


		for( j = 1 ; j < bim->width-1 ; j++, bp++, cp++ ){
			if( cp->n > 0 )		continue;

			bImage_type p;
			p.n = p.r = p.g = p.b = 0;

			bImage_type *sp = cp - bim->width -1;
			for( k = 0; k < 3 ; k++, sp += bim->width-3 ){
				for( n = 0; n < 3 ; n++, sp++ ){
					if( sp->n == 0 )
						continue;

					p.n += 1;
					p.r += sp->r;
					p.g += sp->g;
					p.b += sp->b;
				}
			}


			if(p.n == 0 )
				continue;

			p.r /= p.n;
			p.g /= p.n;
			p.b /= p.n;

			bp->r = p.r;
			bp->g = p.g;
			bp->b = p.b;
			



			no++;
		}
	}



	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += 2 ){
		for( j = 1 ; j < bim->width-1 ; j++, bp++ )
			bp->n &= 0xffff;
	}

	return( no );
}




