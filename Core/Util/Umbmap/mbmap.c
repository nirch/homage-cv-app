/****************************************************************
*								*
*		Module:  mbmap_io.c				*
*								*
****************************************************************/

#include	<stdio.h>
#include	"Uigp/igp.h"

#include	"mbmap.h"

mbmap
mbmap_copy_sub_mbmap( mbmap bmap,int x0,int y0,int row,int column)
{
mbmap	sbmap;
u_char	*sp,	*tp;
int	i,	j;
int	algin;

	MBMAP_CREATE( sbmap, row, column );

	tp = MBMAP_PIXEL_ADRESS( sbmap, 0, 0 );
	sp = MBMAP_PIXEL_ADRESS( bmap, x0, y0 );
	algin = MBMAP_COLUMN( bmap ) - column;

	for( i = 0 ; i < row ; i++, sp += algin )
		for( j = 0 ; j < column ; j++ )
			*tp++ = *sp++;

	return( sbmap );
}



void
mbmap_const( mbmap bmap, u_char val )
{
u_char	*sp;
int	i,	j;


	sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );

	for( i = 0 ; i < MBMAP_ROW(bmap) ; i++ )
		for( j = 0 ; j < MBMAP_COLUMN(bmap) ; j++ )
			*sp++ = val;

}

mbmap
mbmap_dup_mbmap( mbmap bmap,int dup)
{
  return(mbmap_duprc_mbmap(bmap, dup, dup));
}


mbmap
mbmap_duprc_mbmap( mbmap bmap,int row_dup, int col_dup)
{
  mbmap	dbmap;
  int	row,	column;
  u_char	*sp,	*tp,	*tp0;
  int	i;

  MBMAP_CREATE(dbmap,
	       row_dup*MBMAP_ROW( bmap ),
	       col_dup*MBMAP_COLUMN( bmap ) );

  tp = MBMAP_PIXEL_ADRESS( dbmap, 0, 0 );
  sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0);

  for( row = 0 ; row < MBMAP_ROW(bmap) ; row++ ){

    tp0 = tp;

    for( column = 0 ; column < MBMAP_COLUMN(bmap) ; column++ ){
      
      memset((char *)tp, *sp++, col_dup);
      tp += col_dup;
    }

    for(i = 1; i < row_dup; i++){
      memcpy((char *)tp, (char *)tp0, MBMAP_COLUMN(dbmap));
      tp += MBMAP_COLUMN(dbmap);
    }
  }

  return( dbmap );
}


mbmap
mbmap_sampling(mbmap bmap,int dup)
{
  return(mbmap_rc_sampling(bmap, dup, dup));
}


mbmap
mbmap_rc_sampling(mbmap bmap,int row_dup, int col_dup)
{
  mbmap		dbmap;
  int		row,	column, drow, dcolumn;
  int		i,	j;
  u_char	*sp,	*tp,	*sp0;
  float		dup2 = 1.0F / (row_dup*col_dup);
  long  	sum;

  MBMAP_CREATE( dbmap, MBMAP_ROW(bmap)/row_dup,
	       MBMAP_COLUMN(bmap)/col_dup );
  
  tp = MBMAP_PIXEL_ADRESS( dbmap, 0, 0 );
  for( row = drow = 0 ; drow < MBMAP_ROW(dbmap) ; drow++, row += row_dup ){
    sp0 = MBMAP_PIXEL_ADRESS(bmap, row, 0);

    for( column = dcolumn = 0 ; dcolumn < MBMAP_COLUMN(dbmap) ;
	dcolumn++, column += col_dup, sp0 += col_dup ){

      sum =0;
      sp = sp0;
      for( i = 0; i < row_dup ; i++, sp += MBMAP_COLUMN(bmap) - col_dup ){
	for( j = 0 ; j < col_dup ; j++ )
	  sum += *sp++;
      }

      *tp++ = (u_char)( sum * dup2 + 0.5 );
    }
  }
	

  return( dbmap );
}


mbmap
mbmap_rc_sampling_new(mbmap bmap, int r0, int c0, int rows, int columns, int row_dup, int col_dup)
{
mbmap		dbmap;
int		row,	column, drow, dcolumn;
int		i,	j;
u_char	*sp,	*tp,	*sp0;
float		dup2 = 1.0F / (row_dup*col_dup);
long  	sum;

	if( rows == 0 )		rows = MBMAP_ROW(bmap) - r0;
	if( columns == 0 )		columns = MBMAP_COLUMN(bmap) - c0;

	MBMAP_CREATE( dbmap, rows/row_dup, columns/col_dup );
  
	tp = MBMAP_PIXEL_ADRESS( dbmap, 0, 0 );
	for( row = r0, drow = 0 ; drow < MBMAP_ROW(dbmap) ;
					drow++, row += row_dup){
	    sp0 = MBMAP_PIXEL_ADRESS(bmap, row, c0);

	    for( column = c0, dcolumn = 0 ; dcolumn < MBMAP_COLUMN(dbmap) ;
			dcolumn++, column += col_dup, sp0 += col_dup ){

		sum =0;
		sp = sp0;
		for( i = 0; i < row_dup ; i++,
					sp += MBMAP_COLUMN(bmap) - col_dup ){
			for( j = 0 ; j < col_dup ; j++ )
				sum += *sp++;
		}

		*tp++ = (u_char)( sum * dup2 + 0.5 );
	    }
	}
	

	return( dbmap );
}



mbmap
mbmap_22_sampling( mbmap bmap )
{
mbmap		tbmap;
int		row,	column,	algin;
u_char	*sp0,	*sp1,	*tp;

	MBMAP_CREATE( tbmap, MBMAP_ROW(bmap)/2, MBMAP_COLUMN(bmap)/2 );
  
	tp = MBMAP_PIXEL_ADRESS( tbmap, 0, 0 );
	sp0 = MBMAP_PIXEL_ADRESS(bmap, 0, 0);
	sp1 = MBMAP_PIXEL_ADRESS(bmap, 1, 0);

	algin = 2*MBMAP_COLUMN(bmap) - 2*MBMAP_COLUMN(tbmap);

	for( row = 0 ; row < MBMAP_ROW(tbmap) ;
					row++, sp0 += algin, sp1 += algin )
		for( column = 0 ; column < MBMAP_COLUMN(tbmap) ;
					column++, sp0 += 2, sp1 += 2 ){
			*tp++ = (u_char)(( (int)*(sp0) + (int)*(sp0+1) +
						(int)*(sp1) + (int)*(sp1+1) + 2 ) >> 2);
		}
	return( tbmap );
}



mbmap
mbmap_subtract( mbmap bmap1, mbmap bmap2 )
{
mbmap	sub;
int	row,	column;
u_char	*sp1,	*sp2,	*tp;
int	temp;

	MBMAP_CREATE( sub, MBMAP_ROW( bmap1 ), MBMAP_COLUMN( bmap1 ) );

	tp = MBMAP_PIXEL_ADRESS( sub, 0, 0 );
	sp1 = MBMAP_PIXEL_ADRESS( bmap1, 0, 0 );
	sp2 = MBMAP_PIXEL_ADRESS( bmap2, 0, 0 );

	for( row = 0 ; row < MBMAP_ROW(sub) ; row++ ){

		for( column = 0 ; column < MBMAP_COLUMN(sub) ; column++ ){
			temp = (int)(*sp1++) - (int)(*sp2++) + 128;
			*tp++ = (u_char)PUSH_TO_RANGE( temp, 0, 255 );
		}
	}
	

	return( sub );
}


mbmap
mbmap_affine_sum( mbmap bmap1, mbmap bmap2, double a0, double a1, double a2 )
{
mbmap	tbmap;
int	row,	column;
u_char	*sp1,	*sp2,	*tp;
int	temp;

	MBMAP_CREATE( tbmap, MBMAP_ROW( bmap1 ), MBMAP_COLUMN( bmap1 ) );

	tp = MBMAP_PIXEL_ADRESS( tbmap, 0, 0 );
	sp1 = MBMAP_PIXEL_ADRESS( bmap1, 0, 0 );
	sp2 = MBMAP_PIXEL_ADRESS( bmap2, 0, 0 );

	for( row = 0 ; row < MBMAP_ROW(tbmap) ; row++ ){

		for( column = 0 ; column < MBMAP_COLUMN(tbmap) ; column++ ){
			temp = (int)(a0 + a1*(float)(*sp1++) + a2*(float)(*sp2++));
			*tp++ = (u_char)PUSH_TO_RANGE( temp, 0, 255 );
		}
	}
	
	return( tbmap );
}


void
mbmap_minmax( mbmap bmap, int *min, int *max )
{
int	row,	column;
u_char	*sp;

	*min = 255;
	*max = 0;
	sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );
	for( row = 0 ; row < MBMAP_ROW(bmap) ; row++ )
		for( column = 0 ; column < MBMAP_COLUMN(bmap) ; column++ ){
			if( (int)(*sp) < *min )	*min = *sp;
			if( (int)(*sp) > *max )	*max = *sp;

			sp++;
		}
}


mbmap
mbmap_stretch( mbmap bmap, double a, double b )
{
mbmap	tbmap;
int	row,	column;
u_char	*sp,	*tp;
int	temp;


	MBMAP_CREATE( tbmap, MBMAP_ROW( bmap ), MBMAP_COLUMN( bmap ) );



	tp = MBMAP_PIXEL_ADRESS( tbmap, 0, 0 );
	sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );

	for( row = 0 ; row < MBMAP_ROW(tbmap) ; row++ )

		for( column = 0 ; column < MBMAP_COLUMN(tbmap) ; column++ ){
			temp = (int)(a*( (float)(*sp++) - b ) + b);
			*tp++ = (u_char)PUSH_TO_RANGE( temp, 0, 255 );
		}
	

	return( tbmap );
}


mbmap
mbmap_mask( mbmap bmap, int mask )
{
mbmap	tbmap;
int	row,	column;
u_char	*sp,	*tp;
int	temp;


	MBMAP_CREATE( tbmap, MBMAP_ROW( bmap ), MBMAP_COLUMN( bmap ) );



	tp = MBMAP_PIXEL_ADRESS( tbmap, 0, 0 );
	sp = MBMAP_PIXEL_ADRESS( bmap, 0, 0 );

	for( row = 0 ; row < MBMAP_ROW(tbmap) ; row++ )

		for( column = 0 ; column < MBMAP_COLUMN(tbmap) ; column++ ){
			*tp++ = (*sp++) & mask;
		}
	

	return( tbmap );
}



void
mbmap_copy_sub_mbmap_to_uarray(mbmap bmap,int x0,int y0,int row,int column,
				u_char *arry,int acolumn)
{
u_char	*sp,	*tp;
int	i,	j;
int	algin,	a_algin;

	tp = arry;
	a_algin = acolumn - column;

	sp = MBMAP_PIXEL_ADRESS( bmap, x0, y0 );
	algin = MBMAP_COLUMN( bmap ) - column;

	for( i = 0 ; i < row ; i++, sp += algin, tp += a_algin )
		for( j = 0 ; j < column ; j++ )
			*tp++ = *sp++;

}


void
mbmap_ecopy_sub_mbmap_to_uarray(mbmap bmap,
                        int x0,int y0,int row,int column,
                        u_char *arry,int acolumn)
{
u_char   *sp,    *tp;
int     i,      j,      x;
int     col;
 
	tp = arry;
 
	col = MBMAP_COLUMN( bmap ) - y0;
	if( col > column )      col = column;
 
	for( i = 0 ; i < row ; i++, tp += acolumn ){
		x = x0+i;
		if( x >= MBMAP_ROW( bmap ) )    x= MBMAP_ROW( bmap )-1;
		sp = MBMAP_PIXEL_ADRESS( bmap, x, y0 );
 
		for( j = 0 ; j < col ; j++ )
			tp[j] = *sp++;
 
		if( col < column )
			for( j = col ; j < column ; j++ )
				tp[j] = tp[col-1];
	}
}



void
mbmap_copy_uarry_to_sub_mbmap(u_char *arry,int acolumn,
			mbmap bmap,int x0,int y0,int row,int column)
{
u_char	*sp,	*tp;
int	i,	j;
int	algin,	a_algin;

	sp = arry;
	a_algin = acolumn - column;

	tp = MBMAP_PIXEL_ADRESS( bmap, x0, y0 );
	algin = MBMAP_COLUMN( bmap ) - column;

	for( i = 0 ; i < row ; i++, tp += algin, sp += a_algin )
		for( j = 0 ; j < column ; j++ )
			*tp++ = *sp++;
}



void
mbmap_put_mbmap_in_mbmap(mbmap sbmap,mbmap tbmap,int row,int column)
{
u_char	*sp,	*tp;
int	i,	j;
int	algin;

	sp = MBMAP_PIXEL_ADRESS( sbmap, 0, 0 );
	tp = MBMAP_PIXEL_ADRESS( tbmap, row, column );

	algin = MBMAP_COLUMN( tbmap ) - MBMAP_COLUMN( sbmap );

	for( i = 0 ; i < MBMAP_ROW( sbmap ) ; i++, tp += algin )
		for( j = 0 ; j < MBMAP_COLUMN( sbmap ) ; j++ )
			*tp++ = *sp++;
}


mbmap
mbmap_transpose( mbmap bmap )
{
mbmap  t_bmap;
int    r,c;


   MBMAP_CREATE( t_bmap, MBMAP_COLUMN( bmap ), MBMAP_ROW( bmap ) );

   for ( r = 0 ; r < MBMAP_ROW( t_bmap ) ; r++ )
     for ( c = 0 ; c < MBMAP_COLUMN( t_bmap ) ; c++ )
       MBMAP_PIXEL( t_bmap, r, c ) = MBMAP_PIXEL( bmap, c, r );

   return( t_bmap );

}       
