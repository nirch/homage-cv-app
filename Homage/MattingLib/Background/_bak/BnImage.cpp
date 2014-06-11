/*********************************
 ***   BnImage.cpp   ***
 *********************************/

#include	<string.h>
#include	<math.h>

#include	"Ulog/log.h"
#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"

#include	"ImageLabel/BwLabel.h"

#include	"ImageMorphology/ImageMorphology.h"



#include "BnImage.h"


static image_type *image1_avrage4( image_type *sim, image_type *im );

static image_type *image3_avrage4( image_type *sim, image_type *im );

static image_type *	imageS3_abs_diff_image1( image_type *im0, image_type *im1, image_type *im );

static image_type *	image1_avrage4_1( image_type *sim, image_type *im );


void
bnImage_init( image_type *sim )
{
	int	i,	j;
	bn_type	*gv;

	gv = ( bn_type*)sim->data;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++ ){

			gv->no = 0;
		}
	}
}


int
bnImage_no( image_type *sim )
{
	int	i,	j;
	bn_type	*gv;
	int	no;

	no = 0;
	gv = ( bn_type*)sim->data;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++ ){
			if( gv->flag != 0 )
				no++;
		}
	}

	return( no );
}



image_type *
bnImage_average4( image_type *sim, image_type *im )
{
	if( sim->depth == 1 ){
		im = image1_avrage4( sim, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_avrage4( sim, im );
		return( im );
	}

	return( NULL );
}

extern image_type * image1_avrage4_1onSPU ( image_type *sim, image_type *im );

static image_type *
image1_avrage4( image_type *sim, image_type *im )
{
//u_char	*sp;
short	*tp;
int	i,	j;
int	sum;
//int	no;

//#define _SPU_AVARAGE
#ifdef _SPU_AVARAGE

	im = image1_avrage4_1onSPU( sim,im );
#else
	im = image1_avrage4_1( sim, im );
#endif

	for( i = 1 ; i < im->height-1 ; i++ ){
		tp = (short *)IMAGE_PIXEL( im, i, 1 );
		short *tp1 = tp + im->width;
		for( j = 1 ; j < im->width-1 ; j++, tp++, tp1++ ){
			sum = *(tp) + *(tp+1) + *(tp1) + *(tp1+1);
			*tp = sum;
		}
	}


	return( im );
}


//static image_type *
//image1_avrage4( image_type *sim, image_type *im )
//{
//u_char	*sp;
//int	i,	j;
//int	sum;
//int	no;
//
//
//	image_type *im1 = im;
//	im = image_recreate( im, sim->height/4 + 2, sim->width/4+2, 2, 1 );
//
//	if( im != im1 )
//		image2_const( im, 0 );
//
//	short	*tp;
//	no = 0;
//	for( i = 0 ; i+4 < sim->row ; i += 4 ){
//		tp = (short *)IMAGE_PIXEL( im, i/4+1, 1 );
//
//		sp = IMAGE_PIXEL( sim, i, 0 );
//		u_char *sp1 = sp + sim->width;
//		u_char *sp2 = sp1 + sim->width;
//		u_char *sp3 = sp2 + sim->width;
//
//
//
//		for( j = 0 ; j+4 < sim->column ; j += 4, tp++ ){
//
//			sum = *sp + *(sp+1) + *(sp+2 ) + *(sp+3 ) +
//					*sp1 + *(sp1+1) + *(sp1+2 ) + *(sp1+3 ) +
//					*sp2 + *(sp2+1) + *(sp2+2 ) + *(sp2+3 ) +
//					*sp3 + *(sp3+1) + *(sp3+2 ) + *(sp3+3 );
//			sp  += 4;
//			sp1 += 4;
//			sp2 += 4;
//			sp3 += 4;
//
//			*tp = sum;
//		}
//
//		
//	}
//
//
//
//	for( i = 1 ; i < im->height-1 ; i++ ){
//		tp = (short *)IMAGE_PIXEL( im, i, 1 );
//		short *tp1 = tp + im->width;
//		for( j = 1 ; j < im->width-1 ; j++, tp++, tp1++ ){
//			sum = *(tp) + *(tp+1) + *(tp1) + *(tp1+1);
//			*tp = sum;
//		}
//	}
//
//
//	return( im );
//}



static image_type *
image1_avrage4_1( image_type *sim, image_type *im )
{
u_char	*sp;
int	i,	j;
int	sum;
int	no;


	image_type *im1 = im;
	im = image_recreate( im, sim->height/4 + 2, sim->width/4+2, IMAGE_FORMAT_S, 1 );

	if( im != im1 )
		image2_const( im, 0 );

	short	*tp;
	no = 0;
	for( i = 0 ; i+4 < sim->row ; i += 4 ){
		tp = (short *)IMAGE_PIXEL( im, i/4+1, 1 );

		sp = IMAGE_PIXEL( sim, i, 0 );
		u_char *sp1 = sp + sim->width;
		u_char *sp2 = sp1 + sim->width;
		u_char *sp3 = sp2 + sim->width;



		for( j = 0 ; j+4 < sim->column ; j += 4, tp++ ){

			sum = *sp + *(sp+1) + *(sp+2 ) + *(sp+3 ) +
					*sp1 + *(sp1+1) + *(sp1+2 ) + *(sp1+3 ) +
					*sp2 + *(sp2+1) + *(sp2+2 ) + *(sp2+3 ) +
					*sp3 + *(sp3+1) + *(sp3+2 ) + *(sp3+3 );
			sp  += 4;
			sp1 += 4;
			sp2 += 4;
			sp3 += 4;

			*tp = sum;
		}

		
	}

	return( im );
}




static image_type *
image3_avrage4( image_type *sim, image_type *im )
{
u_char	*sp;
int	i,	j;
int	no;


	image_type *im1 = im;
	im = image_recreate( im, sim->height/4 + 2, sim->width/4+2, 6, 1 );

	if( im != im1 )
		image6_const( im, 0, 0, 0 );

	short	*tp;
	no = 0;
	for( i = 0 ; i+4 < sim->row ; i += 4 ){
		tp = (short *)IMAGE_PIXEL( im, i/4+1, 1 );

		sp = IMAGE_PIXEL( sim, i, 0 );
		u_char *sp1 = sp + 3*sim->width;
		u_char *sp2 = sp1 + 3*sim->width;
		u_char *sp3 = sp2 + 3*sim->width;

		int	r,	g,	b;
		r = g = b  = 0;
		for( j = 0 ; j+4 < sim->column ; j += 4){

			r  = *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			g  = *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			b  = *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);

			r += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			g += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			b += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);

			r += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			g += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			b += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);

			r += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			g += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);
			b += *(sp++) + *(sp1++) + *(sp2++) + *(sp3++);

	

			*tp++ = r;
			*tp++ = g;
			*tp++ = b;


		}


	}


	for( i = 1 ; i < im->height-1 ; i++ ){
		tp = (short *)IMAGE_PIXEL( im, i, 1 );
		short *tp1 = tp + 3*im->width;
		for( j = 1 ; j < im->width-1 ; j++ ){
			*tp = *(tp) + *(tp+3) + *(tp1) + *(tp1+3);
			tp++;
			tp1++;

			*tp = *(tp) + *(tp+3) + *(tp1) + *(tp1+3);
			tp++;
			tp1++;

			*tp = *(tp) + *(tp+3) + *(tp1) + *(tp1+3);
			tp++;
			tp1++;


		}
	}




	return( im );
}



image_type *
bnImage_abs_diff( image_type *sim, image_type *sim1, image_type *im )
{
	if(  sim->depth == 2 ){
		im = image2_abs_diff( sim, sim1, im );
		return( im );
	}


	if( sim->depth == 6 ){
		im = imageS3_abs_diff_image1( sim, sim1, im );
		return( im );
	}

	return( NULL );
	

}


static image_type *
imageS3_abs_diff_image1( image_type *im0, image_type *im1, image_type *im )
{
	short	*sp0,	*sp1,	*tp;
	int	i,	j;
	int	R,	G,	B;

	im = image_recreate( im, im0->height, im0->width, 2, 1 );

	sp0 = im0->data_s;
	sp1 = im1->data_s;

	tp = im->data_s;

	short	d;
	for( i = 0; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			R = *sp1++ - *sp0++;
			G = *sp1++ - *sp0++;
			B = *sp1++ - *sp0++;

			//if( (d = *sp1++ - *sp0++) < 0 )	d = -d;

			//if( (tmp = *sp1++ - *sp0++) < 0 )	tmp = -tmp;
			//if( tmp > d )	d = tmp;

			//if( (tmp = *sp1++ - *sp0++) < 0 )	tmp = -tmp;
			//if( tmp > d )	d = tmp;

			d = 0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
			if( d < 0 )	d = -d;
			*tp++ = d;
		}
	}



	return( im );
}




image_type *
image3_ncc8( image_type *sim, image_type *bim, int tVal, image_type *nim )
{
	u_char	*sp,	*bp,	*np;
	int	i,	j,	n,	k,	align;

	int	av[3];

	align = 3*(sim->width - 8);


//	nim = image_recreate( nim, sim->height/8, sim->width/8, 1, 1 );


	for( i = 0 ; i < nim->height ; i++ ){

		np = IMAGE_PIXEL( nim, i, 0 );

		for( j = 0 ; j < nim->width ; j++, np++ ){

			if( *np != 0 )	continue;

			sp = IMAGE_PIXEL( sim, i*8, j*8 );
			bp = IMAGE_PIXEL( bim, i*8, j*8 );

			av[0] = av[1] = av[2] = 0;
			for( k = 0 ; k < 8 ; k++, sp += align, bp += align ){
				for( n = 0 ; n < 8 ; n++ ){
					av[0] += *sp++ - *bp++;
					av[1] += *sp++ - *bp++;
					av[2] += *sp++ - *bp++;
				}
			}

			if( av[0] < 0 )	av[0] = -av[0];
			if( av[1] < 0 )	av[1] = -av[1];
			if( av[2] < 0 )	av[2] = -av[2];


			av[0] = av[0] >> 6;
			av[1] = av[1] >> 6;
			av[2] = av[2] >> 6;

			if( av[0] > tVal || av[1] > tVal || av[2] > tVal )
				*np = 1;

		}
	}


	return( nim );
}



void
bnImage_dump(  image_type *sim, image_type *gvar, int ds, char *prefix, int iFrame, char *suffix, int flag )
{
#ifndef SN_TARGET_PS3
	image_type	*im;


	im = bnImage_dup( sim, gvar, ds, flag,  NULL );

	image_dump( im, prefix, iFrame, suffix );

	image_destroy( im, 1 );
#endif
}



image_type *
bnImage_dup(  image_type *sim, image_type *gvar, int ds, int flag, image_type *im )
{
u_char	*sp;
bn_type	*gv;
int	i,	j;
int	k,	k1,	n,	n1,	align;


	im = image_make_copy( sim, NULL );


	gv = ( bn_type*)gvar->data;


	for( i = 0 ; i < sim->row ; i += ds ){
		k1 = ( i+ds < sim->height)? ds : sim->height-i;
		for( j = 0 ; j < sim->column ; j += ds, gv++ ){

			if( (gv->flag & flag) == flag )	continue;
//			if( gv->flag == flag )	continue;



			n1 = ( j+ds < sim->width)? ds : sim->width-j;
			align = sim->width-n1;

			sp = (u_char *)IMAGE_PIXEL( im, i, j );

			for( k = 0 ; k < k1 ; k++, sp += align ){
				for( n = 0 ; n < n1 ; n++, sp++ ){
					*sp = 0;
				}
			}
		}
	}

	return( im );
}
