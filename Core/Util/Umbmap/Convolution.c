#include	<stdio.h>
#include	"Uigp/igp.h"
#include	"mbmap.h"


void
mbmap_convol_3x3( mbmap bmap, float Matr_conv[3][3],
		int row, int col, mbmap *bmap_conv, int alloc_flag )
{
u_char	*tp,	*sp,	*p;
int	algin_conv,	algin_sp,	algin_tp;
int	i,	j,	n,	k;
float	temp;

	if( alloc_flag == 0 )
		MBMAP_CREATE( *bmap_conv, row, col );

	sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
	tp = MBMAP_PIXEL_ADRESS( *bmap_conv, 0, 0 );

	algin_tp = MBMAP_COLUMN( *bmap_conv )- col;
	algin_sp = MBMAP_COLUMN( bmap ) - col;

	algin_conv = MBMAP_COLUMN( bmap ) - 3;

	for ( i = 0; i < row ; i++, sp += algin_sp, tp += algin_tp )
		for ( j = 0; j < col ; j++, tp++, sp++ ) {

			if( i == 0 || i == row-1 || j == 0 || j == col-1 ){
				*tp = *sp;
				continue;
			}


			p = sp - MBMAP_COLUMN( bmap ) - 1;
			temp = 0;
			for( n = 0 ; n < 3 ; n++, p += algin_conv )
				for( k = 0 ; k < 3 ; k++, p++ )
					temp += (*p)*Matr_conv[n][k];

			*tp = (u_char)PUSH_TO_RANGE( temp, 0, 255 );
		}
}



void
mbmap_convolution( mbmap bmap, float *Matr_conv, int Mno,
		int row, int column, mbmap *bmap_conv, int alloc_flag )
{
u_char	*tp,	*sp,	*p;
int	algin_conv,	algin_sp,	algin_tp;
int	i,	j,	n,	k,	mh;
float	*m,	temp,	w;

	if( alloc_flag == 0 )
		MBMAP_CREATE( *bmap_conv, row, column );

	sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
	tp = MBMAP_PIXEL_ADRESS( *bmap_conv, 0, 0 );

	algin_tp = MBMAP_COLUMN( *bmap_conv )- column;
	algin_sp = MBMAP_COLUMN( bmap ) - column;

	algin_conv = MBMAP_COLUMN( bmap ) - Mno;

	mh = Mno/2;

	for ( i = 0; i < row ; i++, sp += algin_sp, tp += algin_tp )
		for ( j = 0; j < column ; j++, tp++, sp++ ) {


			p = sp - mh*MBMAP_COLUMN( bmap ) - mh;
			m = Matr_conv;
			temp = 0;
			w = 0;

			for( n = i-mh ; n <= i+mh ; n++, p += algin_conv ){

				if( n < 0 || n >= row )	continue;

				for( k = j-mh ; k <= j+mh ; k++, p++, m++ ){
					if( k < 0 || k >= column )
						continue;
					temp += (*p)*(*m);
					w += *m;
				}
			}
	
			if( w != 1.0 )	temp /= w;

			*tp = (u_char)PUSH_TO_RANGE( temp, 0, 255 );
		}
}
