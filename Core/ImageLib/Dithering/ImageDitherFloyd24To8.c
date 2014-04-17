/*************************
 ***	Image24To8.c   ***
 *************************/
#include	<malloc.h>
#include	"Uigp/igp.h"
#include	"Uvec/BitVector.h"

#include	"ImageType/ImageType.h"


#define COLOR_RANGE_0_256(c) (((c) > 0xFF) ? 0xFF : \
									( ((c) < 0) ? 0 : (c) ))

/*
			X	7
		3	5	1
*/

image_type * 
image_dithering_floyd_color_222( image_type *im, image_type *new_im )
{
u_int	*sp, *dpn, *dp1;
u_int	*dp[SP_NO], *data[SP_NO];
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 
int		A;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dpn = (u_int *)new_im->data;

	for( A = SP_R; A < SP_NO; A++ )
	{
		dp[A] = data[A]= (u_int *)malloc( row*col*sizeof( int ) );
		memset( data[A], 0, row*col*4 );
	}

	for( i = 0; i < row; i++ )
	for( j = 0; j < col; j++ )
	{
		for( A = SP_R; A < SP_NO; A++ )
		{
			switch ( A )
			{
				case SP_R: gray = (*sp) >> 16; break;
				case SP_G: gray = ((*sp) >> 8) & 0xFF; break;
				case SP_B: gray = (*sp) & 0xFF; break;
			}

			gray += ((int)(*(dp[A])))/16;	//now in dp[A] summer of previouse error

			if ( gray < 0x40 )
			{
				*(dp[A]) = 0;
				cerror = gray;
			}
			else if ( gray < 0x80 )
			{
				*(dp[A]) = 0x60;
				cerror = gray-0x60;
			}
			else  if ( gray < 0xC0 )
			{
				*(dp[A]) = 0xA0;
				cerror = gray - 0xA0;
			}
			else
			{
				*(dp[A]) = 0xFF;
				cerror = gray - 0xFF;
			}

			cerror_1_16 = cerror;
			cerror_3_16 = cerror * 3;
			cerror_5_16 = cerror * 5;
			cerror_7_16 = cerror * 7;

			if ( j < col - 1 )
				*((dp[A]) + 1) += (u_int)cerror_7_16;
			if ( i < row - 1 )
			{
				dp1 = (dp[A]) + col;
				*dp1 += (u_int)cerror_5_16;
				if ( j < col - 1 )
					*(dp1 + 1) += (u_int)cerror_1_16;
				if ( j > 0 )
					*(dp1 - 1) += (u_int)cerror_3_16;
			}
		}

		*dpn = ((*(dp[SP_R]))<<16) | ((*(dp[SP_G]))<<8) | (*dp[SP_B]);
		for( A = SP_R; A < SP_NO; A++ )
			dp[A]++;

		sp++;
		dpn++;
	}

	for( A = SP_R; A < SP_NO; A++ )
		free( data[A] );

	return new_im;
}


image_type *
image_dithering_floyd_24to8(image_type * im, palette_type * palette, box2i * pBox)
{
image_type * result;


	switch( palette->type ){
	case PALETTE_HALFTONE:
		result = image_dithering_floyd_24to8_HALFTONE(
							im, palette, pBox);
		break;

	case PALETTE_222:
		result = image_dithering_floyd_24to8_222(
							im, palette, pBox);
		break;

	case PALETTE_GRAY:
		result = image_24to8_gray(im, palette, pBox);
		break;

	case PALETTE_SMART:
		result = image_dithering_floyd_24to8_smart(
							im, palette, pBox);
		break;

	case PALETTE_BW:
		result = image_24to8_BW(im, palette, pBox);
		break;

	case PALETTE_WB:
		result = image_24to8_WB(im, palette, pBox);
		break;

	case PALETTE_UNKNOWN:
		result = image_dithering_floyd_24to8_UNKNOWN(
							im, palette, pBox);
		break;

	default:
		result = image_dithering_floyd_24to8_UNKNOWN(
							im, palette, pBox);
		break;
	}	
	return result;
}


image_type *
image_dithering_floyd_24to8_UNKNOWN(image_type * im, palette_type * palette, box2i * pBox)
{

int i, j;
int row, column, column2;
u_char *tp;
int bottom, top, left, right;
image_type * result;
u_int	*sp;
int k,	l,	m,	r;
int	rt,	bt,	gt;

int r_sum, g_sum, b_sum;
int r_set, g_set, b_set;
int *r_er[2], *g_er[2], *b_er[2];
int *r_per[2], *g_per[2], *b_per[2];
int r_error, g_error, b_error;
char order;

	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );


	tp = result->data;

	column2 = column+2;

	r_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( r_er[0], 0, column2*sizeof(int) );
	r_er[1] = r_er[0] + column2;
	g_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( g_er[0], 0, column2*sizeof(int) );
	g_er[1] = g_er[0] + column2;
	b_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( b_er[0], 0, column2*sizeof(int) );
	b_er[1] = b_er[0] + column2;

	order = 0;

	for( i = top; i <= bottom; i++, order = !order){

		r_per[0] = r_er[order]+1;
		r_per[1] = r_er[!order]+1;
		memset( r_per[1], 0, column*sizeof(int) ); 
		g_per[0] = g_er[order]+1;
		g_per[1] = g_er[!order]+1;
		memset( g_per[1], 0, column*sizeof(int) ); 
		b_per[0] = b_er[order]+1;
		b_per[1] = b_er[!order]+1;
		memset( b_per[1], 0, column*sizeof(int) ); 

		sp = IMAGE4_PIXEL(im, i, left );

		for (j = left; j <= right; j++){

			if ( ABS(*(r_per[0])) > 0x800 ) *(r_per[0]) = *(r_per[0])/2;
			if ( ABS(*(g_per[0])) > 0x800 ) *(g_per[0]) = *(g_per[0])/2;
			if ( ABS(*(b_per[0])) > 0x800 ) *(b_per[0]) = *(b_per[0])/2;

			r_sum = IMAGE4_RED( *sp )   + ( (*(r_per[0])) >>4 );
			g_sum = IMAGE4_GREEN( *sp ) + ( (*(g_per[0])) >>4 );
			b_sum = IMAGE4_BLUE( *sp )  + ( (*(b_per[0])) >>4 );
			sp++;

			r_set = COLOR_RANGE_0_256(r_sum);
			g_set = COLOR_RANGE_0_256(g_sum);
			b_set = COLOR_RANGE_0_256(b_sum);

			for (k = 0, l = 0xff; k < palette->colorsNumber; k++){
				if( (rt = (int)palette->data[k].Red - r_set) < 0 )		rt = -rt;
				if( (gt = (int)palette->data[k].Green - g_set) < 0 )	gt = -gt;
				if( (bt = (int)palette->data[k].Blue - b_set) < 0 )		bt = -bt;
				
				m = MAX( rt, gt );
				if( bt > m )	m = bt;

				if (m == 0){
					r = k;
					break;
				}
				if( m < l){
					l = m;
					r = k;
				}
			}

			*tp++ = r;

			r_error = r_sum - palette->data[r].Red;  //r_real
			g_error = g_sum - palette->data[r].Green;//g_real
			b_error = b_sum - palette->data[r].Blue; //b_real
			
			*(r_per[0] + 1) += r_error*7;
			*(g_per[0] + 1) += g_error*7;
			*(b_per[0] + 1) += b_error*7;
			
			*(r_per[1]) += r_error*5;
			*(g_per[1]) += g_error*5;
			*(b_per[1]) += b_error*5;

			*(r_per[1] + 1) += r_error;
			*(g_per[1] + 1) += g_error;
			*(b_per[1] + 1) += b_error;

			*(r_per[1] - 1) += r_error*3;
			*(g_per[1] - 1) += g_error*3;
			*(b_per[1] - 1) += b_error*3;

			r_per[0]++;
			r_per[1]++;
			g_per[0]++;
			g_per[1]++;
			b_per[0]++;
			b_per[1]++;
		}		
	}

	free( r_er[0] );
	free( g_er[0] );
	free( b_er[0] );

	return result;
}


image_type *
image_dithering_floyd_24to8_smart(image_type * im, palette_type * palette, box2i * pBox)
{

int i, j;
int row, column, column2;
u_char *tp;
int bottom, top, left, right;
image_type * result;
u_int	*sp;
int	sh1;
int	k;

int r_sum, g_sum, b_sum;
int r_set, g_set, b_set;
int *r_er[2], *g_er[2], *b_er[2];
int *r_per[2], *g_per[2], *b_per[2];
int r_error, g_error, b_error;
char order;

	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );


	sh1 = 8 - NBITS;

	tp = result->data;

	column2 = column+2;

	r_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( r_er[0], 0, column2*sizeof(int) );
	r_er[1] = r_er[0] + column2;
	g_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( g_er[0], 0, column2*sizeof(int) );
	g_er[1] = g_er[0] + column2;
	b_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( b_er[0], 0, column2*sizeof(int) );
	b_er[1] = b_er[0] + column2;

	order = 0;

	for( i = top; i <= bottom; i++, order = !order){

		r_per[0] = r_er[order]+1;
		r_per[1] = r_er[!order]+1;
		memset( r_per[1], 0, column*sizeof(int) ); 
		g_per[0] = g_er[order]+1;
		g_per[1] = g_er[!order]+1;
		memset( g_per[1], 0, column*sizeof(int) ); 
		b_per[0] = b_er[order]+1;
		b_per[1] = b_er[!order]+1;
		memset( b_per[1], 0, column*sizeof(int) ); 

		sp = IMAGE4_PIXEL(im, i, left );
		
		for (j = left; j <= right; j++){

			if ( ABS(*(r_per[0])) > 0x800 ) *(r_per[0]) = *(r_per[0])/2;
			if ( ABS(*(g_per[0])) > 0x800 ) *(g_per[0]) = *(g_per[0])/2;
			if ( ABS(*(b_per[0])) > 0x800 ) *(b_per[0]) = *(b_per[0])/2;

			r_sum = IMAGE4_RED( *sp )   + ( (*(r_per[0])) >>4 );
			g_sum = IMAGE4_GREEN( *sp ) + ( (*(g_per[0])) >>4 );
			b_sum = IMAGE4_BLUE( *sp )  + ( (*(b_per[0])) >>4 );
			sp++;

			r_set = COLOR_RANGE_0_256(r_sum);
			g_set = COLOR_RANGE_0_256(g_sum);
			b_set = COLOR_RANGE_0_256(b_sum);

			k = ((((r_set >> sh1) << NBITS) + (g_set >> sh1)) << NBITS) + 
									(b_set >> sh1);
			*tp = palette->table[k];

			r_error = r_sum - palette->data[*tp].Red;  //r_real
			g_error = g_sum - palette->data[*tp].Green;//g_real
			b_error = b_sum - palette->data[*tp].Blue; //b_real

			tp++;
			
			*(r_per[0] + 1) += r_error*7;
			*(g_per[0] + 1) += g_error*7;
			*(b_per[0] + 1) += b_error*7;
			
			*(r_per[1]) += r_error*5;
			*(g_per[1]) += g_error*5;
			*(b_per[1]) += b_error*5;

			*(r_per[1] + 1) += r_error;
			*(g_per[1] + 1) += g_error;
			*(b_per[1] + 1) += b_error;

			*(r_per[1] - 1) += r_error*3;
			*(g_per[1] - 1) += g_error*3;
			*(b_per[1] - 1) += b_error*3;

			r_per[0]++;
			r_per[1]++;
			g_per[0]++;
			g_per[1]++;
			b_per[0]++;
			b_per[1]++;
		}		
	}

	free( r_er[0] );
	free( g_er[0] );
	free( b_er[0] );

	return result;
}



image_type * 
image_dithering_floyd_24to8_222(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right;
int row, column, column2;
u_char	*tp;
u_int	*sp;
int	i,	j;

int r_sum, g_sum, b_sum;
int r_set, g_set, b_set;
int r_real, g_real, b_real;
int *r_er[2], *g_er[2], *b_er[2];
int *r_per[2], *g_per[2], *b_per[2];
int r_error, g_error, b_error;
char order;


	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );

	tp = result->data;

	column2 = column+2;

	r_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( r_er[0], 0, column2*sizeof(int) );
	r_er[1] = r_er[0] + column2;
	g_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( g_er[0], 0, column2*sizeof(int) );
	g_er[1] = g_er[0] + column2;
	b_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( b_er[0], 0, column2*sizeof(int) );
	b_er[1] = b_er[0] + column2;

	order = 0;

	for( i = top; i <= bottom; i++, order = !order ){

		r_per[0] = r_er[order]+1;
		r_per[1] = r_er[!order]+1;
		memset( r_per[1], 0, column*sizeof(int) ); 
		g_per[0] = g_er[order]+1;
		g_per[1] = g_er[!order]+1;
		memset( g_per[1], 0, column*sizeof(int) ); 
		b_per[0] = b_er[order]+1;
		b_per[1] = b_er[!order]+1;
		memset( b_per[1], 0, column*sizeof(int) ); 
			
		sp = IMAGE4_PIXEL(im, i, left );

		for (j = left; j <= right; j++){

			if ( ABS(*(r_per[0])) > 0x800 ) *(r_per[0]) = *(r_per[0])/2;
			if ( ABS(*(g_per[0])) > 0x800 ) *(g_per[0]) = *(g_per[0])/2;
			if ( ABS(*(b_per[0])) > 0x800 ) *(b_per[0]) = *(b_per[0])/2;

			r_sum = IMAGE4_RED( *sp )   + ( (*(r_per[0])) >>4 );
			g_sum = IMAGE4_GREEN( *sp ) + ( (*(g_per[0])) >>4 );
			b_sum = IMAGE4_BLUE( *sp )  + ( (*(b_per[0])) >>4 );
			sp++;

			r_set = COLOR_RANGE_0_256(r_sum);
			g_set = COLOR_RANGE_0_256(g_sum);
			b_set = COLOR_RANGE_0_256(b_sum);

			r_real = (r_set & 0xC0) + 0x20;
			g_real = (g_set & 0xC0) + 0x20;
			b_real = (b_set & 0xC0) + 0x20;

			r_error = r_sum - r_real;
			g_error = g_sum - g_real;
			b_error = b_sum - b_real;

			
			*tp++ = (((r_set & 0xC0) >> 2) | ((g_set & 0xC0) >> 4) | 
							((b_set & 0xC0) >> 6) );


			*(r_per[0] + 1) += r_error*7;
			*(g_per[0] + 1) += g_error*7;
			*(b_per[0] + 1) += b_error*7;
			
			*(r_per[1]) += r_error*5;
			*(g_per[1]) += g_error*5;
			*(b_per[1]) += b_error*5;

			*(r_per[1] + 1) += r_error;
			*(g_per[1] + 1) += g_error;
			*(b_per[1] + 1) += b_error;

			*(r_per[1] - 1) += r_error*3;
			*(g_per[1] - 1) += g_error*3;
			*(b_per[1] - 1) += b_error*3;

			r_per[0]++;
			r_per[1]++;
			g_per[0]++;
			g_per[1]++;
			b_per[0]++;
			b_per[1]++;
		}
	}

	free( r_er[0] );
	free( g_er[0] );
	free( b_er[0] );

	return( result );
}


image_type * 
image_dithering_floyd_24to8_gray(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right, row, column;
u_char	*tp;
u_int	*sp;
int	r,	g,	b,	y;
int	colors_index[256];
int	i,	j;

	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );

	tp = result->data;

	for ( i = 0 ; i < 256 ; i++ )
		colors_index[i] = ( i * ( p->colorsNumber - 1 )) / 255;

	for( i = top; i <= bottom; i++){
		sp = IMAGE4_PIXEL(im, i, left );
		for (j = left; j <= right; j++){

			r = IMAGE4_RED( *sp );
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );
			sp++;
			
			y  =  0.29900 * r + 0.58700 * g + 0.11400 * b + 0.5;

			*tp++ = colors_index[y];
		}
	}
	return( result );
}

image_type * 
image_dithering_floyd_24to8_HALFTONE(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right;
int row, column, column2;
u_char	*tp;
u_int	*sp;
int	i,	j;
int	index[256];

int r_sum, g_sum, b_sum;
int r_set, g_set, b_set;
int r1, g1, b1;
int *r_er[2], *g_er[2], *b_er[2];
int *r_per[2], *g_per[2], *b_per[2];
int r_error, g_error, b_error;
char order;

	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );

	tp = result->data;

	for ( i = 0 ; i < 256 ; i++ )
		index[i] = (i + (0.5 * 51 )) / 51;


	column2 = column+2;

	r_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( r_er[0], 0, column2*sizeof(int) );
	r_er[1] = r_er[0] + column2;
	g_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( g_er[0], 0, column2*sizeof(int) );
	g_er[1] = g_er[0] + column2;
	b_er[0] = (int *)malloc( column2*sizeof(int) * 2 );
	memset( b_er[0], 0, column2*sizeof(int) );
	b_er[1] = b_er[0] + column2;

	order = 0;

	for( i = top; i <= bottom; i++, order = !order ){

		r_per[0] = r_er[order]+1;
		r_per[1] = r_er[!order]+1;
		memset( r_per[1], 0, column*sizeof(int) ); 
		g_per[0] = g_er[order]+1;
		g_per[1] = g_er[!order]+1;
		memset( g_per[1], 0, column*sizeof(int) ); 
		b_per[0] = b_er[order]+1;
		b_per[1] = b_er[!order]+1;
		memset( b_per[1], 0, column*sizeof(int) ); 

		sp = IMAGE4_PIXEL(im, i, left );

		for (j = left; j <= right; j++){

			if ( ABS(*(r_per[0])) > 0x800 ) *(r_per[0]) = *(r_per[0])/2;
			if ( ABS(*(g_per[0])) > 0x800 ) *(g_per[0]) = *(g_per[0])/2;
			if ( ABS(*(b_per[0])) > 0x800 ) *(b_per[0]) = *(b_per[0])/2;

			r_sum = IMAGE4_RED( *sp )   + ( (*(r_per[0])) >>4 );
			g_sum = IMAGE4_GREEN( *sp ) + ( (*(g_per[0])) >>4 );
			b_sum = IMAGE4_BLUE( *sp )  + ( (*(b_per[0])) >>4 );
			sp++;

			r_set = COLOR_RANGE_0_256(r_sum);
			g_set = COLOR_RANGE_0_256(g_sum);
			b_set = COLOR_RANGE_0_256(b_sum);

			r1 = index[r_set];
			g1 = index[g_set];
			b1 = index[b_set];

			*tp = b1*36 + g1*6 + r1;

			r_error = r_sum - p->data[*tp].Red;
			g_error = g_sum - p->data[*tp].Green;
			b_error = b_sum - p->data[*tp].Blue;

			tp++;

			*(r_per[0] + 1) += r_error*7;
			*(g_per[0] + 1) += g_error*7;
			*(b_per[0] + 1) += b_error*7;
			
			*(r_per[1]) += r_error*5;
			*(g_per[1]) += g_error*5;
			*(b_per[1]) += b_error*5;

			*(r_per[1] + 1) += r_error;
			*(g_per[1] + 1) += g_error;
			*(b_per[1] + 1) += b_error;

			*(r_per[1] - 1) += r_error*3;
			*(g_per[1] - 1) += g_error*3;
			*(b_per[1] - 1) += b_error*3;

			r_per[0]++;
			r_per[1]++;
			g_per[0]++;
			g_per[1]++;
			b_per[0]++;
			b_per[1]++;
		}
	}

	free( r_er[0] );
	free( g_er[0] );
	free( b_er[0] );

	return( result );
}


image_type * 
image_dithering_floyd_24to8_BW(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right, row, column;
u_char	*tp;
u_int	*sp;
int	r;
int	i,	j;

	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );

	tp = result->data;

	for( i = top; i <= bottom; i++){
		sp = IMAGE4_PIXEL(im, i, left );
		for (j = left; j <= right; j++){

			r = IMAGE4_RED( *sp );
			sp++;
			
			if ( r == 0 )
				*tp = 0;
			else
				*tp = 1;
			tp++;
		}
	}
	return( result );
}


image_type * 
image_dithering_floyd_24to8_WB(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right, row, column;
u_char	*tp;
u_int	*sp;
int	r;
int	i,	j;

	if (pBox)
	{
		bottom = pBox->y1;
		top = pBox->y0;
		left = pBox->x0;
		right = pBox->x1;
	}
	else
	{
		bottom = im->row - 1;
		top = 0;
		left = 0;
		right = im->column - 1;
	}
	row = bottom - top + 1;
	column = right - left + 1;
	result = image_create(row, column, 1, 1, NULL );

	tp = result->data;

	for( i = top; i <= bottom; i++){
		sp = IMAGE4_PIXEL(im, i, left );
		for (j = left; j <= right; j++){

			r = IMAGE4_RED( *sp );
			sp++;
			
			if ( r == 0 )
				*tp = 1;
			else
				*tp = 0;
			tp++;
		}
	}
	return( result );
}


