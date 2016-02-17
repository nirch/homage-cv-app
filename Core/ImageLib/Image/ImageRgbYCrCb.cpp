/*
 * ImageRgbYCrCb.c
 *
 * Copyright (C) 1991, 1992, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains input colorspace conversion routines.
 * These routines are invoked via the methods get_sample_rows
 * and colorin_init/term.
 */

#include	"Uigp/igp.h"
#include	"ImageType/ImageType.h"

/**************** RGB -> YCbCr conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *      Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *      Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + MAXJSAMPLE/2
 *      Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + MAXJSAMPLE/2
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times R,G,B for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The MAXJSAMPLE/2 offsets and the rounding fudge-factor of 0.5 are included
 * in the tables to save adding them separately in the inner loop.
 */

#define SCALEBITS       16      /* speedier right-shift on some machines */

#define ONE_HALF        ((long) 1 << (SCALEBITS-1))

#define FIX(x)          ((long) ((x) * (1L<<SCALEBITS) + 0.5))


/*
 * Initialize for colorspace conversion.
 */

static int	Init_flag = 0;

static	long	R_Y_tab[256],	G_Y_tab[256],	B_Y_tab[256];
static	long	R_Cb_tab[256],	G_Cb_tab[256],	B_Cb_tab[256];
static	long	G_Cr_tab[256],	B_Cr_tab[256];


static void
image_Rgb_YCrCb_init()
{
int	i;

	for (i = 0; i < 256; i++){

		R_Y_tab[i] = FIX(0.29900) * i;
		G_Y_tab[i] = FIX(0.58700) * i;
		B_Y_tab[i] = FIX(0.11400) * i + ONE_HALF;

		R_Cb_tab[i] = (-FIX(0.16874)) * i;
		G_Cb_tab[i] = (-FIX(0.33126)) * i;
		B_Cb_tab[i] = FIX(0.50000) * i    + ONE_HALF*256;

		/* R_Cr_tab == B_Cb_tab */
		G_Cr_tab[i] = (-FIX(0.41869)) * i;
		B_Cr_tab[i] = (-FIX(0.08131)) * i;
	}
}



/*
 * Fetch some rows of pixels from get_input_row and convert to the
 * JPEG colorspace.
 */

#define	 RGB_Y( r, g, b )  \
		((u_char)((R_Y_tab[r] + G_Y_tab[g] + B_Y_tab[b]) >> SCALEBITS))

#define	 RGB_Cb( r, g, b )  \
		((u_char)((R_Cb_tab[r]+ G_Cb_tab[g]+ B_Cb_tab[b]) >> SCALEBITS))

#define	 RGB_Cr( r, g, b )  \
	 	((u_char)((B_Cb_tab[r]+ G_Cr_tab[g]+ B_Cr_tab[b]) >> SCALEBITS))





static image_type *	image3_Rgb_YCrCb( image_type *sim, image_type *im );
static image_type *	image4_Rgb_YCrCb( image_type *sim, image_type *im );
static image_type *	image1_Rgb_YCrCb( image_type *sim, image_type *im );



image_type *
image_Rgb_YCrCb( image_type *sim, image_type *im )
{
	if( IMAGE_TYPE( sim) == IMAGE_TYPE_U16 ){
		if( sim->channel == 1 ){
			im = image_make_copy( sim, im );
			return( im );
		}
	}

	if( sim->depth == 1 ){
		im = image1_Rgb_YCrCb( sim, im );
		return( im );
	}


	if( sim->depth == 3 ){
		im = image3_Rgb_YCrCb( sim, im );
		return( im );
	}


	if( sim->depth == 4 ){
		im = image4_Rgb_YCrCb( sim, im );
		return( im );
	}

	return( NULL );
}



static image_type *
image3_Rgb_YCrCb( image_type *sim, image_type *im )
{
u_char	*sp,	*tp;
int	r,	g,	b;
int	i,	j;


	if( Init_flag == 0 ){ 
		image_Rgb_YCrCb_init(); 
		Init_flag = 1; 
	} 


	im = image_recreate( im, sim->height, sim->width, 3, 1 );



	sp = sim->data;
	tp = im->data;



	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){

			r = *sp++;
			g = *sp++;
			b = *sp++;

			*tp++ = RGB_Y( r, g, b );

			*tp++ = RGB_Cr( r, g, b );

			*tp++ = RGB_Cb( r, g, b );
		}
	}

	return( im );
}



static image_type *
image4_Rgb_YCrCb( image_type *sim, image_type *im )
{
u_int	*sp,	*tp;
int	r,	g,	b;
int	y,	q,	a;
int	i,	j;


	if( Init_flag == 0 ){ 
		image_Rgb_YCrCb_init(); 
		Init_flag = 1; 
	} 



	im = image_recreate( im, sim->height, sim->width, 4, 1 );



	sp = sim->data_ui;
	tp = im->data_ui;



	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){

			r = IMAGE4_RED( *sp );
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );

			sp++;

			y = RGB_Y( r, g, b );

			q = RGB_Cr( r, g, b );

			a = RGB_Cb( r, g, b );

			*tp = IMAGE4_RGB( y, q, a );

			tp++;
		}
	}

	return( im );
}


static image_type *
image1_Rgb_YCrCb( image_type *sim, image_type *im )
{
u_char	*sp,	*tp;
int	y;
int	i,	j;


	if( Init_flag == 0 ){ 
		image_Rgb_YCrCb_init(); 
		Init_flag = 1; 
	} 


	im = image_recreate( im, sim->height, sim->width, 3, 1 );



	sp = sim->data;
	tp = im->data;



	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){

			y = *sp++;

			*tp++ = y;

			*tp++ = 128;

			*tp++ = 128;
		}
	}

	return( im );
}




static image_type *
imageUS_Rgb_YCrCb( image_type *sim, image_type *im )
{
	int	r,	g,	b;
	int	i,	j;


	if( Init_flag == 0 ){ 
		image_Rgb_YCrCb_init(); 
		Init_flag = 1; 
	} 

	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U16, 1 );
	



	u_short *sp = sim->data_us;
	u_short *tp = im->data_us;



	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){

			r = *sp++;
			g = *sp++;
			b = *sp++;

			*tp++ = RGB_Y( r, g, b );

			*tp++ = RGB_Cr( r, g, b );

			*tp++ = RGB_Cb( r, g, b );
		}
	}

	return( im );
}


//image_type *
//image_Rgb_YCrCbO( image_type *im, int Falloc )
//{
//image_type	*tim;
//u_int	*sp,	*tp;
//int	row,	col;
//int	r,	g,	b;
//int	y,	q,	a;
//int	i,	j;
//
//	if( Init_flag == 0 ){ 
//		image_Rgb_YCrCb_init(); 
//		Init_flag = 1; 
//	} 
//
//	row = IMAGE_ROW(im);
//	col = IMAGE_COLUMN(im);
//
//
//	if( Falloc == 1 )
//		tim = image_create( row, col, 4, 1, NULL );
//	else	tim = im;
//
//
//	sp = IMAGE4_DATA( im );
//	tp = IMAGE4_DATA( tim );
//
//
//
//	for( i = 0 ; i < row ; i++ ){
//		for( j = 0 ; j < col ; j++ ){
//
//			r = IMAGE4_RED( *sp );
//			g = IMAGE4_GREEN( *sp );
//			b = IMAGE4_BLUE( *sp );
//
//			sp++;
//
//			y = RGB_Y( r, g, b );
//
//			q = RGB_Cr( r, g, b );
//
//			a = RGB_Cb( r, g, b );
//
//			*tp = IMAGE4_RGB( y, q, a );
//
//			tp++;
//		}
//	}
//
//	return( tim );
//}