//  Defines the entry point for the console application.
//
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include "Ulog/Log.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageMorphology/ImageMorphology.h"
#include "Imagelabel/BwLabel.h"
#include "ImageLabel/ImageLabel.h"
#define DIFF_THRESH 16// 12


#include	"BackgroundRemoval.h"

#include	"BnImage/BnImage.h"


static image_type *	dbSilhouette_test( image_type *sim, image_type *bim, float tVal0, image_type *mim );


static void	dbSilhouette_sub( image_type *sim, image_type *bim, image_type *mim, image_type *im );

static void	dbSilhouette_sub_rgb( image_type *sim, image_type *bim, image_type *mim, image_type *im );


static int	dbSilhouette_removeB( image_type *im, image_type *mim );


static void	dbSilhouette_update_mask( image_type *im, image_type *mim, int i0, int i1, int j0, int j1 );




static void		dbSilhouette_remove_blob( image_type *sim, int nT, int value );

static int		dbSilhouette_clear( image_type *im, image_type *mim );
static int		dbSilhouette_clear_8( image_type *im, image_type *mim );




static void image1_blob_filter( image_type *sim, int nT );


static int	dbSilhouette_pixel_remove( image_type *im, image_type *mim, int T );


static int	dbSilhouette_closeB( image_type *im, image_type *mim );


static void		dbSilhouette_fill_blob( image_type *sim, int nP, int nT, int value );

static int		dbSilhouette_fill( image_type *im, image_type *mim );


static int iFrame;


CBackgroundRemoval::CBackgroundRemoval()
{
	m_tVal0 = 4;

	m_rgb = 1;

	m_rgbBim = NULL;
	m_bim = NULL;
	m_bim4 = NULL;	
	
	m_yim = NULL;

	m_silhouetteIm = NULL;

}

CBackgroundRemoval::~CBackgroundRemoval()
{

}

void CBackgroundRemoval::SetBackground( image_type *sim )
{

	m_rgbBim = image_make_copy( sim, m_rgbBim );

	m_bim = image_to_y( sim, m_bim );
	
	m_bim4 = bnImage_average4( m_bim, NULL );
}



image_type *
CBackgroundRemoval::Process( image_type *sim, int iFrame )
{
image_type	*yim4;

	iFrame = m_iFrame;

	m_yim = image_to_y( sim, m_yim );

	yim4 = bnImage_average4( m_yim, NULL );

	m_silhouetteIm = Process( sim, m_yim, yim4, m_silhouetteIm );

	image_destroy( yim4, 1 );

	return( m_silhouetteIm );
}



image_type *
CBackgroundRemoval::Process( image_type *rgbSim, image_type *sim, image_type *sim4, image_type *im )
{
image_type	*mim;

	iFrame = m_iFrame;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	IMAGE_DUMP( sim, "im", m_iFrame, NULL );


	mim = dbSilhouette_test( sim4, m_bim4, m_tVal0, NULL );



	// remove blobe with less then 5 block 
	image1_blob_filter( mim, 5 );
//	IMAGE_DUMP_DUP( mim, 8, 255, "mim", m_iFrame, "T" );




	if( m_rgb != 0 && m_rgbBim != NULL && rgbSim != NULL )
		dbSilhouette_sub_rgb( rgbSim, m_rgbBim, mim, im );
	else
		dbSilhouette_sub( sim, m_bim, mim, im );

	//IMAGE_DUMP( im, "sub", m_iFrame, "a" );


	// remove block if for the max FOREGROUND point of all each neighbors is less then 32
	if( dbSilhouette_removeB( im, mim ) >  0 )
		dbSilhouette_removeB( im, mim );


	//IMAGE_DUMP( im, "sub", m_iFrame, "b" );
	//IMAGE_DUMP_DUP( mim, 8, 4, "mim", m_iFrame, "sub" );






	dbSilhouette_remove_blob( mim, 50, 255 );
	dbSilhouette_clear_8( im, mim );


	IMAGE_DUMP( im, "sub", m_iFrame, "d" );
	//IMAGE_DUMP_DUP( mim, 8, 4, "sub", m_iFrame, "b-mim" );


	// fill blob
	dbSilhouette_fill_blob( mim, 48, 50, 255 );
	dbSilhouette_fill( im, mim );		// fill image



	IMAGE_DUMP( im, "sub", m_iFrame, "c" );
	//IMAGE_DUMP_DUP( mim, 8, 4, "sub", m_iFrame, "c-mim" );



//	IMAGE_DUMP( im, "sh", m_iFrame, NULL );
//	IMAGE_DUMP_DUP( mim, 8, 1, "sh", m_iFrame, "m" );
	dbSilhouette_update_mask( im, mim, 0, im->height-1, 0, im->width-1 );
//	IMAGE_DUMP_DUP( mim, 8, 1, "sh", m_iFrame, "m1" );

	dbSilhouette_closeB( im, mim );
	dbSilhouette_closeB( im, mim );



//	IMAGE_DUMP( im, "sh", m_iFrame, NULL );




	dbSilhouette_pixel_remove( im, mim, 2 );


	int no = image1_nPixel_nonzero( mim );
//	GPLOG( ("SL: %d", no ));


	image_destroy( mim, 1 );


	return( im );
}








static image_type *
dbSilhouette_test( image_type *sim, image_type *bim, float tVal0, image_type *mim )
{
	u_char	*mp;
	short	*tp;
	int	i,	j;
	int	sum;


	image_type *tim = NULL;
	tim = bnImage_abs_diff( sim, bim, tim );
	image2_binary_down( tim, tVal0*64 );


	mim = image_recreate( mim, tim->height/2-1, tim->width/2-1, 1, 1 );

	mp = mim->data;
	for( i = 0 ; i < mim->height ; i++ ){
		tp = (short *)IMAGE_PIXEL( tim, 2*i+1, 1 );
		short *tp0 = tp - tim->width;
		short *tp1 = tp + tim->width;
		for( j = 0 ; j < mim->width ; j++, tp += 2, tp1 += 2, tp0 += 2, mp++ ){


			sum = *(tp0-1) + *(tp0) + *(tp0+1) +
				*(tp-1) + *(tp) + *(tp+1) +
				*(tp1-1) + *(tp1) + *(tp1+1);


			*mp = ( sum == 9 )? 0 : 1;
		}
	}

	image_destroy( tim, 1 );

	return( mim );
}




static void
dbSilhouette_sub( image_type *sim, image_type *bim, image_type *mim, image_type *im )
{
u_char	*sp,	*bp,	*mp;
int	i,	j;
int	k,	k1,	n,	n1,	align,	align2;

int	tmp;
int	no;


	align2 = (sim->width - 8)>>2;

	for( i = 0 ; i < sim->height ; i += 8 ){
		k1 = ( i+8 < sim->height)? 8 : sim->height-i;
		u_char *mmp = IMAGE_PIXEL( mim, i/8, 0 );

		for( j = 0 ; j < sim->width ; j += 8, mmp++ ){

			n1 = ( j+8 < sim->width)? 8 : sim->width-j;
			align = sim->width-n1;


			if( *mmp == 0 ){
				//mp = IMAGE_PIXEL( im, i, j );
				//for( k = 0 ; k < k1 ; k++, mp += align ){
				//	for( n = 0 ; n < n1 ; n++, mp++ ){
				//		*mp = 0;
				//	}
				//}
				u_int *tp = (u_int*)IMAGE_PIXEL( im, i, j );
				for( k = 0 ; k < k1 ; k++, tp += align2 ){
					*tp++ = 0;
					*tp++ = 0;
				}


				continue;
			}


			no = 0;
			sp = IMAGE_PIXEL( sim, i, j );
			bp = IMAGE_PIXEL( bim, i, j );
			mp = IMAGE_PIXEL( im, i, j );
			for( k = 0 ; k < k1 ; k++, mp += align, sp += align, bp += align   ){
				for( n = 0 ; n < n1 ; n++, sp++, bp++, mp++ ){

					int d = (int)*sp - (int)*bp;
					if( d < 0 )	d = -d;

					// difference less than threshold 
					if (d < DIFF_THRESH){
						tmp = 0;
					}
					else{
						tmp = 255;
						no++;
					}

					*mp = tmp;
				}
			}

			*mmp = no;
		}
	}
}



static void
dbSilhouette_sub_rgb( image_type *sim, image_type *bim, image_type *mim, image_type *im )
{
	u_char	*sp,	*bp,	*mp;
	int	i,	j;
	int	k,	k1,	n,	n1,	align,	align2;

	int	tmp;
	int	no;

	align2 = (sim->width - 8 )>>2;;

	for( i = 0 ; i < sim->height ; i += 8 ){
		k1 = ( i+8 < sim->height)? 8 : sim->height-i;
		u_char *mmp = IMAGE_PIXEL( mim, i/8, 0 );

		for( j = 0 ; j < sim->width ; j += 8, mmp++ ){

			n1 = ( j+8 < sim->width)? 8 : sim->width-j;
			align = sim->width-n1;


			if( *mmp == 0 ){
				//mp = IMAGE_PIXEL( im, i, j );
				//for( k = 0 ; k < k1 ; k++, mp += align ){
				//	for( n = 0 ; n < n1 ; n++, mp++ ){
				//		*mp = 0;
				//	}
				//}

				u_int *tp = (u_int*)IMAGE_PIXEL( im, i, j );
				for( k = 0 ; k < k1 ; k++, tp += align2 ){
					*tp++ = 0;
					*tp++ = 0;
				}


				continue;
			}


			no = 0;
			sp = IMAGE_PIXEL( sim, i, j );
			bp = IMAGE_PIXEL( bim, i, j );
			mp = IMAGE_PIXEL( im, i, j );
			for( k = 0 ; k < k1 ; k++, mp += align, sp += 3*align, bp += 3*align   ){
				for( n = 0 ; n < n1 ; n++, mp++ ){

					int	d,	d1;
					d = (int)*sp++ - (int)*bp++;
					if( d < 0 )	d = -d;

					d1 = (int)*sp++ - (int)*bp++;
					if( d1 < 0 )	d1 = -d1;
					if( d1 > d )	d = d1;

					d1 = (int)*sp++ - (int)*bp++;
					if( d1 < 0 )	d1 = -d1;
					if( d1 > d )	d = d1;



					// difference less than threshold 
					if (d < DIFF_THRESH){
						tmp = 0;
					}
					else{
						tmp = 255;
						no++;
					}

					*mp = tmp;
				}
			}

			*mmp = no;
		}
	}
}




static void 
image1_blob_filter( image_type *sim, int nT )
{
	image_type *im;
	bwLabel_type *abw;
	int nBw; 
	int	i,	j;


	im = image_create( sim->height, sim->width, 2, 1, NULL );

	u_char *sp = sim->data;
	short *bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp == 0 )? 0 : 1;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

//	image2_bwLabel_set_direction( im, abw, nBw );
	imageLabelUS_set_id_B( im, abw );



	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < nT )
			abw[i].id = -1;
	}



	sp = sim->data;
	bp = im->data_s;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *sp == 0 )	continue;
			if( abw[*bp].id < 0 )
				*sp = 0;
		}

		free( abw );
		image_destroy( im, 1 );
}



static void 
dbSilhouette_remove_blob( image_type *sim, int nT, int value )
{
	image_type *im;
	bwLabel_type *abw;
	int nBw; 
	int	i,	j;


	im = image_create( sim->height, sim->width, 2, 1, NULL );

	u_char *sp = sim->data;
	short *bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = *sp;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

	imageLabelUS_set_id_B( im, abw );



	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < nT )
			abw[i].id = -1;
	}



	sp = sim->data;
	bp = im->data_s;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *sp == 0 )	continue;
			if( abw[*bp].id < 0 )
				*sp = value;
		}

		free( abw );
		image_destroy( im, 1 );
}





static void 
dbSilhouette_fill_blob( image_type *sim, int nP, int nT, int value )
{
	image_type *im;
	bwLabel_type *abw;
	int nBw; 
	int	i,	j;


	im = image_create( sim->height, sim->width, 2, 1, NULL );

	u_char *sp = sim->data;
	short *bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < nP )? 1 : 0;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

	imageLabelUS_set_id_B( im, abw );



	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < nT )
			abw[i].id = -1;
	}



	sp = sim->data;
	bp = im->data_s;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *sp == 0 )	continue;
			if( abw[*bp].id < 0 )
				*sp = value;
		}

		free( abw );
		image_destroy( im, 1 );
}








static int
dbSilhouette_removeB( image_type *im, image_type *mim )
{
u_char	*sp,	*sp0,	*sp1;
u_int	*tp;
int	i,	j,	k,	align,	m;
int	no;


	align = (im->width - 8)>>2;

	no = 0;
	for( i = 0 ; i < mim->height ; i++ ){

		sp = IMAGE_PIXEL( mim, i, 1 );


//#define DD	24
		sp0 = sp - mim->width;
		sp1 = sp + mim->width;

		for( j = 1 ; j < mim->width-1 ; j++, sp++, sp0++, sp1++ ){
			if( *sp == 0 )	continue;

//			if( *sp > 32 )	continue;

			m  = *(sp-1);
			if( m < *(sp+1))	m = *(sp+1);

			if( i > 0 ){
				if( m < *(sp0+1))	m = *(sp0+1);
				if( m < *sp0 )	m = *sp0;
				if( m < *(sp0+1))	m = *(sp0+1);
			}


			if( i < mim->height-1){
				if( m < *(sp1-1))	m = *(sp1-1);
				if( m < *sp1 )	m = *sp1;
				if( m < *(sp1+1))	m = *(sp1+1);
			}


			if( m > 32 )	continue;





			//tp = IMAGE_PIXEL( im, i*8, j*8 );
			//for( k = 0 ; k < 8 ; k++, tp += align ){
			//	for( n = 0 ; n < 8 ; n++, tp++ ){
			//		*tp = 0;
			//	}
			//}

			tp = (u_int*)IMAGE_PIXEL( im, i*8, j*8 );
			for( k = 0 ; k < 8 ; k++, tp += align ){
				*tp++ = 0;
				*tp++ = 0;
			}


			*sp = 0;
			no++;
		}
	}

	return( no );
}

static int
dbSilhouette_clear_8( image_type *im, image_type *mim )
{
	u_char	*mp,	*tp0;
	u_int	*tp;
	int	i,	j,	k,	align;
	int	no;


	align = im->width>>2;

	no = 0;
	for( i = 0 ; i < mim->height ; i++ ){

		mp = IMAGE_PIXEL( mim, i, 0 );


		tp0 = IMAGE_PIXEL( im, i*8, 0 );
		for( j = 0 ; j < mim->width ; j++, mp++, tp0 += 8 ){
			if( *mp != 255 )	continue;

			tp = (u_int *)tp0;
			for( k = 0 ; k < 8 ; k++, tp += align ){
				tp[0] = 0;
				tp[1] = 0;
			}

			*mp = 0;
			no++;
		}
	}

	return( no );
}


static int
dbSilhouette_clear( image_type *im, image_type *mim )
{
u_char	*mp,	*tp,	*tp0;
int	i,	j,	n,	k,	align;
int	no;


	align = im->width - 8;

	no = 0;
	for( i = 0 ; i < mim->height ; i++ ){

		mp = IMAGE_PIXEL( mim, i, 0 );


		tp0 = IMAGE_PIXEL( im, i*8, 0 );
		for( j = 0 ; j < mim->width ; j++, mp++, tp0 += 8 ){
			if( *mp != 255 )	continue;

			tp = tp0;
			for( k = 0 ; k < 8 ; k++, tp += align ){
				for( n = 0 ; n < 8 ; n++, tp++ ){
					*tp = 0;
				}
			}

			*mp = 0;
			no++;
		}
	}

	return( no );
}


static int
dbSilhouette_fill( image_type *im, image_type *mim )
{
u_char	*mp,	*tp0;
u_int	*tp;
int	i,	j,	k,	align;
int	no;


	align = im->width>>2;

	no = 0;
	for( i = 0 ; i < mim->height ; i++ ){

		mp = IMAGE_PIXEL( mim, i, 0 );


		tp0 = IMAGE_PIXEL( im, i*8, 0 );
		for( j = 0 ; j < mim->width ; j++, mp++, tp0 += 8 ){
			if( *mp != 255 )	continue;

			tp = (u_int *)tp0;
			for( k = 0 ; k < 8 ; k++, tp += align ){
				tp[0] = 0xFFFFFFFF;
				tp[1] = 0xFFFFFFFF;
			}


			*mp = 0;
			no++;
		}
	}

	return( no );
}



static void
dbSilhouette_update_mask( image_type *im, image_type *mim, int i0, int i1, int j0, int j1 )
{
	int	i,	j,	k;
	u_char	*mp;
	u_int	*sp;
	int	sum,	align;


	i0 = i0 >> 3;
	i1 = ( i1 + 7 )/8;
	j0 = j0 >> 3;
	j1 = ( j1+7)/8;

	align = im->width>>2;


	for( i = i0 ; i < i1 ; i++ ){
		mp = IMAGE_PIXEL( mim, i, j0 );

		for( j = j0 ; j < j1 ; j++, mp++ ){

			sum = 0;
			sp = (u_int *)IMAGE_PIXEL( im, i*8, j*8 );
			for( k = 0 ; k < 8 ; k++, sp += align ){
				sum += sp[0] & 0x01010101;
				sum += sp[1] & 0x01010101;
			}

			sum = (sum>>24) + (sum>>16) + (sum>>8) + sum;
			*mp = sum & 0xFF;
		}

	}
}

static void
dbSilhouette_update_maskO( image_type *im, image_type *mim, int i0, int i1, int j0, int j1 )
{
int	i,	j,	k,	n;
u_char	*mp,	*sp;
int	sum,	align;


	i0 = i0 >> 3;
	i1 = ( i1 + 7 )/8;
	j0 = j0 >> 3;
	j1 = ( j1+7)/8;

	align = im->width - 8;


	for( i = i0 ; i < i1 ; i++ ){
		mp = IMAGE_PIXEL( mim, i, j0 );

		for( j = j0 ; j < j1 ; j++, mp++ ){
			
			sum = 0;
			sp = IMAGE_PIXEL( im, i*8, j*8 );
			for( k = 0 ; k < 8 ; k++, sp += align ){
				for( n = 0 ; n < 8 ; n++, sp++ ){
					sum += *sp & 0x01;
//					sum += (*sp != 0);
//



				}
			}

			*mp = sum;
		}

	}
}



static int
dbSilhouette_pixel_remove( image_type *im, image_type *mim, int T )
{
u_char	*mp,	*tp00,	*tp,	*tp0,	*tp1;
int	i,	j,	n,	k,	align;
int	no,	sno;
u_char	b[64],	*bp;


	align = im->width - 8;

	sno = 0;
	for( i = 1 ; i < mim->height-1 ; i++ ){

		mp = IMAGE_PIXEL( mim, i, 1 );


		tp00 = IMAGE_PIXEL( im, i*8, 1*8 );
		for( j = 1 ; j < mim->width-1 ; j++, mp++, tp00 += 8 ){
			if( *mp == 0  )	continue;




			no = 0;
			for( k = 0, tp = tp00, bp= b  ; k < 8 ; k++, tp += align ){
				tp0 = tp - im->width;
				tp1 = tp + im->width;


				for( n = 0 ; n < 8 ; n++, tp++, tp0++, tp1++, bp++ ){
					*bp = *tp;
					if( *tp == 0 )	continue;


					int t =  (*(tp0-1) & 0x01 ) +  (*(tp0) & 0x01 ) +   (*(tp0+1) & 0x01 ) +
						(*(tp-1) & 0x01 ) + (*(tp+1) & 0x01 ) +
						(*(tp1-1) & 0x01 ) +  (*(tp1) & 0x01 ) +   (*(tp1+1) & 0x01 );


					if( t <= T  ){
						*bp = 0;
						no++;
					}
				}
			}

			if( no == 0 )	continue;

			sno += no;


			no = 0;
			for( k = 0, tp = tp00, bp= b ; k < 8 ; k++, tp += align ){

				for( n = 0 ; n < 8 ; n++, tp++, bp++ ){
					*tp = *bp;
					no =+ *tp &0x01;
				}
			}

			*mp  = 0;
		}
	}

	return( sno );
}




static int
dbSilhouette_closeB( image_type *im, image_type *mim )
{
	u_char	*sp,	*sp0,	*sp1,	*tp;
	int	i,	j,	n,	k,	align,	m;
	int	no;


	align = im->width - 8;

	no = 0;
	for( i = 0 ; i < mim->height ; i++ ){

		sp = IMAGE_PIXEL( mim, i, 1 );


//#define DD	24
		sp0 = sp - mim->width;
		sp1 = sp + mim->width;

		for( j = 1 ; j < mim->width-1 ; j++, sp++, sp0++, sp1++ ){
			if( *sp == 64 )	continue;


			if( *sp == 0 )	continue;

			m = 0;
			if( *(sp-1) < 36 )	m++;

			if( *(sp+1) < 36 )	m++;


			if( i > 0 ){
				if(  *(sp0+1) < 36 )m++;

				if( *sp0 < 36 )	m++;

				if( *(sp0+1) < 36 )	m++;
			}


			if( i < mim->height-1){
				if( *(sp1-1) < 36 )	m++;

				if( *sp1 < 36 )	m++;
	
				if( *(sp1+1) < 36 )	m++;
			}





			if( m >= 2 )	continue;




			tp = IMAGE_PIXEL( im, i*8, j*8 );
			for( k = 0 ; k < 8 ; k++, tp += align ){
				for( n = 0 ; n < 8 ; n++, tp++ ){
					*tp = 255;
				}
			}

			*sp = 64;
			no++;
		}
	}

	return( no );
}


