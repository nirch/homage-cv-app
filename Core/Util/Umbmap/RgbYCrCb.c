/*
 * MbRgbYCrCb.c
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
#include	"mbmap.h"


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

/* We allocate one big table and divide it up into eight parts, instead of
 * doing eight alloc_small requests.  This lets us use a single table base
 * address, which can be held in a register in the inner loops on many
 * machines (more than can hold all eight addresses, anyway).
 */

/*
 * Initialize for colorspace conversion.
 */

static int	Init_flag = 0;

static	long	R_Y_tab[256],	G_Y_tab[256],	B_Y_tab[256];
static	long	R_Cb_tab[256],	G_Cb_tab[256],	B_Cb_tab[256];
static	long	G_Cr_tab[256],	B_Cr_tab[256];

void
mbmap_Rgb_YCrCb_init ()
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


void
mbmap_Rgb_YCrCb( mbmap R, mbmap G, mbmap B, int row, int col,
			mbmap *Y, mbmap *Cr, mbmap *Cb, int alloc_flag )
{
register int r, g, b;
register u_char *r_ptr, *g_ptr, *b_ptr;
register u_char *y_ptr, *cb_ptr, *cr_ptr;
long	algin_r,	algin_g,	algin_b;
long	algin_y,	algin_cr,	algin_cb;
int	i,	j;

	if( Init_flag == 0 ){ 
		mbmap_Rgb_YCrCb_init(); 
		Init_flag = 1; 
	} 


	if( alloc_flag == 0 ){
		MBMAP_CREATE( *Y, row, col );
		MBMAP_CREATE( *Cr, row, col );
		MBMAP_CREATE( *Cb, row, col );
	}

	algin_r = MBMAP_COLUMN( R ) - col;
	algin_g = MBMAP_COLUMN( G ) - col;
	algin_b = MBMAP_COLUMN( B ) - col;

	algin_y = MBMAP_COLUMN( *Y ) - col;
	algin_cr = MBMAP_COLUMN( *Cr ) - col;
	algin_cb = MBMAP_COLUMN( *Cb ) - col;
		
	r_ptr = MBMAP_DATA_ADRESS( R );
	g_ptr = MBMAP_DATA_ADRESS( G );
	b_ptr = MBMAP_DATA_ADRESS( B );

	y_ptr = MBMAP_DATA_ADRESS( *Y );
	cr_ptr = MBMAP_DATA_ADRESS( *Cr );
	cb_ptr = MBMAP_DATA_ADRESS( *Cb );



	for( i = 0 ; i < row ; i++ ){
		for( j = 0 ; j < col ; j++ ){

			r = *r_ptr++;
			g = *g_ptr++;
			b = *b_ptr++;

			*y_ptr++ = RGB_Y( r, g, b );

			*cb_ptr++ = RGB_Cb( r, g, b );

			*cr_ptr++ = RGB_Cr( r, g, b );
		}

		r_ptr += algin_r;
		g_ptr += algin_g;
		b_ptr += algin_b;

		y_ptr += algin_y;
		cr_ptr += algin_cr;
		cb_ptr += algin_cb;
	}
}


void
mbmap_Rgb_YCrCb411( mbmap R, mbmap G, mbmap B, int row, int col,
			mbmap *Y, mbmap *Cr, mbmap *Cb, int alloc_flag )
{
register int r, g, b;
register u_char *r_ptr, *g_ptr, *b_ptr;
register u_char *r1_ptr, *g1_ptr, *b1_ptr;

register u_char *y_ptr, *cb_ptr, *cr_ptr;
register u_char *y1_ptr;

long	cr,	cb;

long	algin_r,	algin_g,	algin_b;
long	algin_y,	algin_cr,	algin_cb;
int	i,	j;

	if( Init_flag == 0 ){ 
		mbmap_Rgb_YCrCb_init(); 
		Init_flag = 1; 
	} 


	if( alloc_flag == 0 ){
		row -= row%2;
		col -= col%2;
		MBMAP_CREATE( *Y, row, col );
		MBMAP_CREATE( *Cr, row>>1, col>>1 );
		MBMAP_CREATE( *Cb, row>>1, col>>1 );
	}

	algin_r = 2*MBMAP_COLUMN( R ) - col;
	algin_g = 2*MBMAP_COLUMN( G ) - col;
	algin_b = 2*MBMAP_COLUMN( B ) - col;

	algin_y = 2*MBMAP_COLUMN( *Y ) - col;
	algin_cr = MBMAP_COLUMN( *Cr ) - (col>>1);
	algin_cb = MBMAP_COLUMN( *Cb ) - (col>>1);
		


	r_ptr = MBMAP_DATA_ADRESS( R );
	r1_ptr = r_ptr + MBMAP_COLUMN( R );

	g_ptr = MBMAP_DATA_ADRESS( G );
	g1_ptr = g_ptr + MBMAP_COLUMN( G );

	b_ptr = MBMAP_DATA_ADRESS( B );
	b1_ptr = b_ptr + MBMAP_COLUMN( B );



	y_ptr = MBMAP_DATA_ADRESS( *Y );
	y1_ptr = y_ptr + MBMAP_COLUMN( *Y );
	cr_ptr = MBMAP_DATA_ADRESS( *Cr );
	cb_ptr = MBMAP_DATA_ADRESS( *Cb );



	row >>= 1;
	col >>= 1;
	for( i = 0 ; i < row ; i++ ){
		for( j = 0 ; j < col ; j++ ){

			r = *r_ptr++;	g = *g_ptr++;	b = *b_ptr++;
			*y_ptr++ = RGB_Y( r, g, b );
			cb =	  RGB_Cb( r, g, b );
			cr =	  RGB_Cr( r, g, b );

			r = *r_ptr++;	g = *g_ptr++;	b = *b_ptr++;
			*y_ptr++ = RGB_Y( r, g, b );
			cb +=	  RGB_Cb( r, g, b );
			cr +=	  RGB_Cr( r, g, b );


			r = *r1_ptr++;	g = *g1_ptr++;	b = *b1_ptr++;
			*y1_ptr++ = RGB_Y( r, g, b );
			cb +=	  RGB_Cb( r, g, b );
			cr +=	  RGB_Cr( r, g, b );

			r = *r1_ptr++;	g = *g1_ptr++;	b = *b1_ptr++;
			*y1_ptr++ = RGB_Y( r, g, b );
			cb +=	  RGB_Cb( r, g, b );
			cr +=	  RGB_Cr( r, g, b );


			*cb_ptr++ = (u_char)(( cb+2)>>2);

			*cr_ptr++ = (u_char)(( cr+2)>>2);
		}

		r_ptr += algin_r;
		g_ptr += algin_g;
		b_ptr += algin_b;

		r1_ptr += algin_r;
		g1_ptr += algin_g;
		b1_ptr += algin_b;

		y_ptr += algin_y;
		y1_ptr += algin_y;
		cr_ptr += algin_cr;
		cb_ptr += algin_cb;
	}
}
