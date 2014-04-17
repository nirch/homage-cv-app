
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






image_type *	bImage_realloc(  box2i_type *b, int n, image_type *bim );

image_type *	bImage_set( image_type *sim, image_type *mim, box2i_type *b, int n, image_type *bim );

image_type *	bImage_set( image_type *sim, box2i_type *b, int N, image_type *bim );


image_type *	bImage_to_image( image_type *bim, image_type *im );

image_type *	bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im );

void		bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix );

void		bImage_clear( image_type *bim );

int			bImage_update( image_type *bim, image_type *cim, float T );


int			bImage_bnMask( image_type *bim, image_type *bnIm );


int	bImage_complete( image_type *bim );



int	CUniformBackground::ProcessHistogram( image_type *sim, image_type *mim )
{
	
	gpTime_start( &m_rTime );

	m_yim = image1_from( sim, m_yim );

	m_bim = bImage_set( sim, mim,&m_roi, m_N, m_bim );



	bImage_dump( m_bim, m_N, "cc", 1, "S" );

	bImage_complete( m_bim );

	bImage_dump( m_bim, m_N, "cc", 1, "C" );


	ProcessBn( sim, 2*m_bnT );
	IMAGE_DUMP_DUP( m_bnImD, 8, 1, "cc", 1, "bn" );

	gpTime_stop( &m_rTime );
	return( 1 );
}









int	CUniformBackground::ProcessBn( image_type *sim, float bnT )
{

	gpTime_start( &m_tBn );

//	m_yim = image1_from( sim, m_yim );
	m_bnIm = bnImage_average4( m_yim, m_bnIm );

	m_bnImD = bnImage_dd( m_bnIm, bnT, m_bnImD );

	gpTime_stop( &m_tBn );

	return( 1 );
}

int	bImage_fill_1( image_type *bim );







image_type *
bImage_set( image_type *sim, image_type *mim, box2i_type *b, int N, image_type *bim )
{
	int	i,	j,	k,	n,	i0,	j0;
	u_char	*sp,	*mp;
	bImage_type *bp,	*bp0;


	 bim = bImage_realloc( b, N,  bim );

	 bImage_clear( bim );




	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){
			
			if( i0 >= sim->height-1 )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );
			mp = IMAGE_PIXEL( mim, i0, b->x0 );



			for( j = 1, j0 = b->x0, bp = bp0 ; j < bim->width-1 ; j++, j0 += N, bp++ ){
				int n1 = N;
				if( j0 + n1 > sim->width )	n1 = sim->width - j0;

				for( n = 0 ; n < n1 ; n++, sp += 3, mp++ ){
					if( *mp != 0 )
						continue;

					bp->r += sp[0];
					bp->g += sp[1];
					bp->b += sp[2];
					bp->n++;
				}
			}
		}
	}


	bp = ( bImage_type *)bim->data;
	for( i = 0 ; i < bim->height ; i++ ){
		
		for( j = 0 ; j < bim->width ; j++, bp++ ){
			if( bp->n < 0.5*n*n ){
				bp->n = 0;
				continue;
			}

			bp->r /= bp->n;
			bp->g /= bp->n;
			bp->b /= bp->n;
		}
	}

	return( bim );
}





image_type *
bImage_set( image_type *sim, box2i_type *b, int N, image_type *bim )
{
	int	i,	j,	k,	n,	i0,	j0;
	u_char	*sp;
	bImage_type *bp,	*bp0;


	bim = bImage_realloc( b, N,  bim );

	bImage_clear( bim );



	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= sim->height-1 )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );
			
			for( j = 1, j0 = b->x0, bp = bp0 ; j < bim->width-1 ; j++, j0 += N, bp++ ){
				int n1 = N;
				if( j0 + n1 > sim->width )	n1 = sim->width - j0;

				for( n = 0 ; n < n1 ; n++, sp += 3 ){
	
					bp->r += sp[0];
					bp->g += sp[1];
					bp->b += sp[2];
					bp->n++;
				}
			}
		}
	}


	bp = ( bImage_type *)bim->data;
	for( i = 0 ; i < bim->height ; i++ ){

		for( j = 0 ; j < bim->width ; j++, bp++ ){
			if( bp->n == 0 )	continue;

			bp->r /= bp->n;
			bp->g /= bp->n;
			bp->b /= bp->n;
		}
	}

	return( bim );
}









image_type *
bImage_realloc(  box2i_type *b, int n, image_type *bim )
{

	int	width,	height,	w,	h;
	width = b->x1 - b->x0;
	w = width/n;
	if( width - w*n > 0.25*n )	w++;

	height = b->y1 - b->y0;
	h = height/n;
	if( height - h*n > 0.25*n )	h++;


	if( bim != NULL && ( bim->width != w + 2 || bim->height != h + 2 )){
		image_destroy( bim, 1 );
		bim = NULL;
	}

	if( bim == NULL )
		bim = image_create( h+2, w+2, sizeof(bImage_type), 1, NULL );


	// bImage_clear( bim );


	//bImage_type *bp = (bImage_type *)bim->data;

	//for( i = 0 ; i < bim->height ; i++ ){
	//	for( j = 0 ; j < bim->width ; j++, bp++ ){
	//		bp->n= 0;
	//		bp->r = bp->g = bp->b = 0;
	//	}
	//}

	return( bim );
}



void
bImage_clear( image_type *bim )
{
	int	i,	j;

	bImage_type *bp = (bImage_type *)bim->data;

	for( i = 0 ; i < bim->height ; i++ ){
		for( j = 0 ; j < bim->width ; j++, bp++ ){
			bp->n= 0;
			bp->r = bp->g = bp->b = 0;
		}
	}

}



int	bImage_fill_1( image_type *bim );
int	bImage_fill_row( image_type *bim );



int
bImage_complete( image_type *bim )
{

	//bImage_dump( bim, "aa", 1, "1" );

	//	while( bImage_fill_1( bim ) > 0 );

	bImage_fill_row( bim );


//	bImage_dump( bim, 8, "aa", 1, "fill" );

	return( 1 );
}

int
bImage_fill_1( image_type *bim )
{
	int	i,	j,	n,	k,	no;
	bImage_type *bp;



	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += 2 ){


		for( j = 1 ; j < bim->width-1 ; j++, bp++ ){
			if( bp->n > 0 )		continue;
			bp->r = bp->g = bp->b = 0;
			bImage_type *sp = bp - bim->width -1;
			for( k = 0; k < 3 ; k++, sp += bim->width-3 ){
				for( n = 0; n < 3 ; n++, sp++ ){
					if( sp == bp || sp->n == 0 || sp->n & 0x10000 )
						continue;

					bp->n += sp->n;
					bp->r += sp->n*sp->r;
					bp->g += sp->n*sp->g;
					bp->b += sp->n*sp->b;
				}
			}


			if( bp->n == 0 )
				continue;

			bp->r /= bp->n;
			bp->g /= bp->n;
			bp->b /= bp->n;
			bp->n = 0x10001;

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



int
	bImage_fill_row( image_type *bim )
{
	int	i,	j,	k,	no;
	bImage_type *bp;



	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += 2 ){


		for( j = 1 ; j < bim->width-1 ; j++, bp++ ){
			if( bp->n > 0 )		continue;

			int j0 = j-1;
			bImage_type *bp0 = bp-1;
			for( ; j < bim->width-1 ; j++, bp++ )
				if( bp->n >  0 )		break;
			

			bImage_type *bp1 = bp;
			if( j >= bim->width-1 )
				bp1 = bp0;

			if( j0 == 0 )
				bp0 = bp1;

			bImage_type *cp;
			for( k = j0 + 1, cp = bp0+1 ; k < j ; k++, cp++ ){
				float f = (k - j0)/(float)( j - j0 ); 
				cp->r = (1-f) * bp0->r + f* bp1->r;
				cp->g = (1-f) * bp0->g + f* bp1->g;
				cp->b = (1-f) * bp0->b + f* bp1->b;
				cp->n = 1;

			}
		}
	}


	return( no );
}



image_type *
bImage_to_image( image_type *bim, image_type *im )
{
	int	i,	j;

	
	im = image_realloc( im, bim->width-2, bim->height-2, 3, IMAGE_TYPE_U8, 1 );



	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	u_char *tp = im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){
		for( j = 0 ; j < im->width ; j++, bp++ ){
			
			*tp++ = bp->r;
			*tp++ = bp->g;
			*tp++ = bp->b;
		}
	}

	return( im );
}



image_type *
bImage_diff( image_type *sim, box2i_type *b, int N, image_type *bim, int T, image_type *im )
{
	int	i,	j,	k,	n,	i0;
	u_char	*sp,	*tp;
	bImage_type *bp,	*bp0;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= sim->height )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );
			tp = IMAGE_PIXEL( im, i0, b->x0 );


			for( j = 1, bp = bp0 ; j < bim->width-1 ; j++, bp++ ){
				for( n = 0 ; n < N ; n++, tp++ ){
	
					int dr = *sp++ - bp->r;
					int dg = *sp++ - bp->g;
					int db = *sp++ - bp->b;

					int d = ABS(dr);
					if( dg < 0 )	dg = -dg;
					if( d < dg )	d = dg;

					if( db < 0 )	db = -db;
					if( d < db )	d = db;


					//if( d < T )
					//	*tp = 0;
					//else *tp = 255;

					*tp = d;
				}
			}
		}

	}

	return( im );
}


void
bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix )
{
#ifdef _DUMP
	image_type *im = bImage_to_image(  bim, NULL );

	IMAGE_DUMP_DUP( im, N, 1,prefix, index, suffix );

	image_destroy( im, 1 );
#endif
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




