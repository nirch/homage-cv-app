/*
 * MbYCrCbRgb.c
 *
 * Copyright (C) 1991, 1992, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains output colorspace conversion routines.
 * These routines are invoked via the methods color_convert
 * and colorout_init/term.
 */

#include	"Uigp/igp.h"
#include	"mbmap.h"


/**************** YCbCr -> RGB conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *      R = Y                + 1.40200 * Cr
 *      G = Y - 0.34414 * Cb - 0.71414 * Cr
 *      B = Y + 1.77200 * Cb
 * where Cb and Cr represent the incoming values less MAXJSAMPLE/2.
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 * Notice that Y, being an integral input, does not contribute any fraction
 * so it need not participate in the rounding.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times Cb and Cr for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The Cr=>R and Cb=>B values can be rounded to integers in advance; the
 * values for the G calculation are left scaled up, since we must add them
 * together before rounding.
 */



#define SCALEBITS       16      /* speedier right-shift on some machines */

#define ONE_HALF        ((long) 1 << (SCALEBITS-1))

#define FIX(x)          ((long) ((x) * (1L<<SCALEBITS) + 0.5))


static int	Init_flag = 0;

static long Cr_r_tab[256];          /* => table for Cr to R conversion */
static long Cb_b_tab[256];          /* => table for Cb to B conversion */
static long Cr_g_tab[256];        /* => table for Cr to G conversion */
static long Cb_g_tab[256];        /* => table for Cb to G conversion */


static  u_char  *Gray,  Agray[384];
#define         GRAY_LEVEL( g )         ( Gray[ (int)g ] )


/*
 * Initialize for colorspace conversion.
 */

void
mbmap_YCrCb_Rgb_init()
{
int	i,	x2;

        Gray = &Agray[64];
        for( i = 0 ; i < 64 ; i++ )     Agray[i] = 0;
        for( i = 0 ; i < 256 ; i++ )    Gray[i] = i;
        for( i = 256 ; i < 256+64 ; i++ )       Gray[i] = 255;


	for (i = 0; i < 256; i++) {
		x2 = 2*i - 255;      /* twice x */

		Cr_r_tab[i] = ( FIX(1.40200/2) * x2 + ONE_HALF ) >> SCALEBITS;

		Cb_b_tab[i] = (FIX(1.77200/2) * x2 + ONE_HALF) >> SCALEBITS;

		Cr_g_tab[i] = (- FIX(0.71414/2)) * x2;

		Cb_g_tab[i] = (- FIX(0.34414/2)) * x2 + ONE_HALF;
	}
}


#define  CrCb_r( cr, cb )	((int)( Cr_r_tab[cr] ))
#define  CrCb_g( cr, cb ) \
			((int)( (Cb_g_tab[cb] + Cr_g_tab[cr]) >> SCALEBITS ))
#define  CrCb_b( cr, cb )	((int)( Cb_b_tab[cb] ))




void
mbmap_YCrCb_Rgb( mbmap Y, mbmap Cr, mbmap Cb, int row, int col,
			mbmap *R, mbmap *G, mbmap *B, int alloc_flag )
{
register int y, cr, cb;
register u_char *r_ptr, *g_ptr, *b_ptr;
register u_char *y_ptr, *cb_ptr, *cr_ptr;
long	algin_r,	algin_g,	algin_b;
long	algin_y,	algin_cr,	algin_cb;
int	i,	j,	tmp;

	if( Init_flag == 0 ){
		mbmap_YCrCb_Rgb_init();
		Init_flag = 1;
	}

	if( alloc_flag == 0 ){
		MBMAP_CREATE( *R, row, col );
		MBMAP_CREATE( *G, row, col );
		MBMAP_CREATE( *B, row, col );
	}

	algin_r = MBMAP_COLUMN( *R ) - col;
	algin_g = MBMAP_COLUMN( *G ) - col;
	algin_b = MBMAP_COLUMN( *B ) - col;

	algin_y = MBMAP_COLUMN( Y ) - col;
	algin_cr = MBMAP_COLUMN( Cr ) - col;
	algin_cb = MBMAP_COLUMN( Cb ) - col;
		
	r_ptr = MBMAP_DATA_ADRESS( *R );
	g_ptr = MBMAP_DATA_ADRESS( *G );
	b_ptr = MBMAP_DATA_ADRESS( *B );

	y_ptr = MBMAP_DATA_ADRESS( Y );
	cr_ptr = MBMAP_DATA_ADRESS( Cr );
	cb_ptr = MBMAP_DATA_ADRESS( Cb );



	for( i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {

			y = *y_ptr++;
			cr = *cr_ptr++;
			cb = *cb_ptr++;

			tmp = y + CrCb_r( cr, cb );
/*
			*r_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );
*/
			*r_ptr++ = GRAY_LEVEL( tmp );

			tmp = y + CrCb_g( cr, cb );
			*g_ptr++ = GRAY_LEVEL( tmp );
/*
			*g_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );
*/

			tmp = y + CrCb_b( cr, cb );
			*b_ptr++ = GRAY_LEVEL( tmp );
/*
			*b_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );
*/
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
mbmap_YCrCb411_Rgb( mbmap Y, mbmap Cr, mbmap Cb, int row, int col,
			mbmap *R, mbmap *G, mbmap *B, int alloc_flag )
{
register int y, cr, cb;
register u_char *r_ptr, *g_ptr, *b_ptr;
register u_char *r1_ptr, *g1_ptr, *b1_ptr;

register u_char *y_ptr, *cb_ptr, *cr_ptr;
register u_char *y1_ptr;

long	algin_r,	algin_g,	algin_b;
long	algin_y,	algin_cr,	algin_cb;
int	i,	j,	tmp;
int	crcb_r,	crcb_g,	crcb_b;

	if( Init_flag == 0 ){
		mbmap_YCrCb_Rgb_init();
		Init_flag = 1;
	}

	if( alloc_flag == 0 ){
		MBMAP_CREATE( *R, row, col );
		MBMAP_CREATE( *G, row, col );
		MBMAP_CREATE( *B, row, col );
	}


        algin_r = 2*MBMAP_COLUMN( *R ) - col;
        algin_g = 2*MBMAP_COLUMN( *G ) - col;
        algin_b = 2*MBMAP_COLUMN( *B ) - col;

        algin_y = 2*MBMAP_COLUMN( Y ) - col;
        algin_cr = MBMAP_COLUMN( Cr ) - (col>>1);
        algin_cb = MBMAP_COLUMN( Cb ) - (col>>1);


        r_ptr = MBMAP_DATA_ADRESS( *R );
        r1_ptr = r_ptr + MBMAP_COLUMN( *R );

        g_ptr = MBMAP_DATA_ADRESS( *G );
        g1_ptr = g_ptr + MBMAP_COLUMN( *G );

        b_ptr = MBMAP_DATA_ADRESS( *B );
        b1_ptr = b_ptr + MBMAP_COLUMN( *B );



        y_ptr = MBMAP_DATA_ADRESS( Y );
        y1_ptr = y_ptr + MBMAP_COLUMN( Y );
        cr_ptr = MBMAP_DATA_ADRESS( Cr );
        cb_ptr = MBMAP_DATA_ADRESS( Cb );

		


	for( i = 0; i < row>>1; i++) {
		for(j = 0; j < col>>1; j++) {

			cr = *cr_ptr++;
			cb = *cb_ptr++;

			crcb_r = CrCb_r( cr, cb );
			crcb_g = CrCb_g( cr, cb );
			crcb_b = CrCb_b( cr, cb );


			y = *y_ptr++;
			tmp = y + crcb_r;
			*r_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_g;
			*g_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_b;
			*b_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );


			y = *y_ptr++;
			tmp = y + crcb_r;
			*r_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_g;
			*g_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_b;
			*b_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );



			y = *y1_ptr++;
			tmp = y + crcb_r;
			*r1_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_g;
			*g1_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_b;
			*b1_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );


			y = *y1_ptr++;
			tmp = y + crcb_r;
			*r1_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_g;
			*g1_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = y + crcb_b;
			*b1_ptr++ = (u_char)PUSH_TO_RANGE( tmp, 0, 255 );
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
