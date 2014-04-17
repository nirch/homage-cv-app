/*************************
 ***	Image24To8.c   ***
 *************************/
#include	<malloc.h>
#include	"Uigp/igp.h"
#include	"Uvec/BitVector.h"

#include "ImageType/ImageType.h"

/*
image_type *
image_24to8_simple(image_type * im, box2i * pBox)
{
int i, j, row, column;
u_int * p;
u_char red, green, blue;
u_char	*tp,	*pp;

int bottom, top, left, right;
image_type * result;

	p = (u_int *)im->data;
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
	for (j = bottom; j >= top; j--)
	{
		for (i = left; i <= right; i++)
		{
			pp = (unsigned char *)(p + j * im->column + i);
			red = pp[2];
			green = pp[1];
			blue = pp[0];
			*tp++ = ((blue + 50) / 51) * 36 + ((green + 50) / 51) * 6 + (red + 50) / 51;
		}
	}
	return result;
}

image_type *
image_24to8(image_type * im, palette_type * palette, box2i * pBox)
{

int i, j, row, column;
//unsigned long * p;
int red, green, blue;
u_char *tp;
int bottom, top, left, right;
image_type * result;
u_int	*sp;

//	p = (unsigned long *)im->data;
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

	for( i = bottom; i >= top; i--){
		sp = IMAGE4_PIXEL(im, i, left );
		for (j = left; j <= right; j++){

			red = IMAGE4_RED(*sp);
			green = IMAGE4_GREEN(*sp);
			blue = IMAGE4_BLUE(*sp);
			sp++;
			*tp++ = palette_index(palette, red, green, blue);
		}
		
	}
	return result;
}
*/

image_type *
image_24to8(image_type * im, palette_type * palette, box2i * pBox)
{
image_type * result;


	switch( palette->type ){
	case PALETTE_HALFTONE:
		result = image_24to8_HALFTONE(im, palette, pBox);
		break;

	case PALETTE_222:
		result = image_24to8_222(im, palette, pBox);
		break;

	case PALETTE_GRAY:
		result = image_24to8_gray(im, palette, pBox);
		break;

	case PALETTE_SMART:
		result = image_24to8_smart(im, palette, pBox);
		break;

	case PALETTE_BW:
		result = image_24to8_BW(im, palette, pBox);
		break;

	case PALETTE_WB:
		result = image_24to8_WB(im, palette, pBox);
		break;

	case PALETTE_UNKNOWN:
		result = image_24to8_UNKNOWN(im, palette, pBox);
		break;

	default:
		result = image_24to8_UNKNOWN(im, palette, pBox);
		break;
	}	
	return result;
}


image_type *
image_24to8_UNKNOWN(image_type * im, palette_type * palette, box2i * pBox)
{

int i, j, row, column;
int red, green, blue;
u_char *tp;
int bottom, top, left, right;
image_type * result;
u_int	*sp;
int k,	l,	m,	r;
int	rt,	bt,	gt;

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

			red = IMAGE4_RED(*sp);
			green = IMAGE4_GREEN(*sp);
			blue = IMAGE4_BLUE(*sp);
			sp++;

			for (k = 0, l = 0xff; k < palette->colorsNumber; k++){
				if( (rt = (int)palette->data[k].Red - red) < 0 )		rt = -rt;
				if( (gt = (int)palette->data[k].Green - green) < 0 )	gt = -gt;
				if( (bt = (int)palette->data[k].Blue - blue) < 0 )		bt = -bt;
				
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
		}		
	}
	return result;
}


image_type *
image_24to8_smart(image_type * im, palette_type * palette, box2i * pBox)
{

int i, j, row, column;
int red, green, blue;
u_char *tp;
int bottom, top, left, right;
image_type * result;
u_int	*sp;
int	sh1;
int	k;

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

	for( i = top; i <= bottom; i++){
		sp = IMAGE4_PIXEL(im, i, left );
		for (j = left; j <= right; j++){



			red = IMAGE4_RED(*sp);
			green = IMAGE4_GREEN(*sp);
			blue = IMAGE4_BLUE(*sp);
			sp++;


			k = ((((red >> sh1) << NBITS) + (green >> sh1)) << NBITS) + (blue >> sh1);
			*tp++ = palette->table[k];
		}		
	}

	return result;
}



image_type * 
image_24to8_222(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right, row, column;
u_char	*tp;
u_int	*sp;
int	r,	g,	b;
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
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );
			sp++;
			
			r = r >> 6;
			g = g >> 6;
			b = b >> 6;
		
			*tp++ = (((r)<<4) | ((g)<<2) | (b) );
		}
	}
	return( result );
}


image_type * 
image_24to8_gray(image_type * im, palette_type * p, box2i * pBox)
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
image_24to8_HALFTONE(image_type * im, palette_type * p, box2i * pBox)
{
image_type *result;
int bottom, top, left, right, row, column;
u_char	*tp;
u_int	*sp;
int	r,	g,	b,	r1,	g1,	b1;
int	i,	j;
int	index[256];

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

	for( i = top; i <= bottom; i++){
		sp = IMAGE4_PIXEL(im, i, left );
		for (j = left; j <= right; j++){

			r = IMAGE4_RED( *sp );
			g = IMAGE4_GREEN( *sp );
			b = IMAGE4_BLUE( *sp );
			sp++;
			
			r1 = index[r];
			g1 = index[g];
			b1 = index[b];
			*tp++ = b1*36 + g1*6 + r1;
		}
	}
	return( result );
}


image_type * 
image_24to8_BW(image_type * im, palette_type * p, box2i * pBox)
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
image_24to8_WB(image_type * im, palette_type * p, box2i * pBox)
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


void image_difference(image_type * im1, image_type * im2, box2i * b)
{
BitVector * pBVx, * pBVy;
int i, j, k;
unsigned long * p1, * p2;

	pBVx = createBitVector((unsigned short)im1->column);
	pBVy = createBitVector((unsigned short)im1->row);
	
	p1 = (unsigned long *)im1->data;
	p2 = (unsigned long *)im2->data;
	
	for (j = 0, k = 0; j < im1->row; j++, k += im1->column)
	{
		for (i = 0; i < im1->column; i++)
		{
			if (p1[k + i] != p2[k + i])
			{
				setBitVector(pBVx, i);
				setBitVector(pBVy, j);	
			}
		}
	}
	if ((b->x0 = firstSetBitOfVector(pBVx)) >= 0)
		b->x1 = lastSetBitOfVector(pBVx);
	else 
		b->x1 = -1;
	if ((b->y0 = firstSetBitOfVector(pBVy)) >= 0)
		b->y1 = lastSetBitOfVector(pBVy);
	else 
		b->y1 = -1;
	deleteBitVector(pBVx);
	deleteBitVector(pBVy);
}

