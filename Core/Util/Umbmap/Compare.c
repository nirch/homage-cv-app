
/****************************************************************
*								*
*               Module:  Compare.c				*
*								*
****************************************************************/

#include        <stdio.h>
#include        <math.h>
#include        "Uigp/igp.h"

#include        "mbmap.h"

float
mbmap_variability( mbmap bmap1, mbmap bmap2, int x0,int y0,int row,int column )
{
float	sum = 0,	tmp,	var;
u_char	*sp1,	*sp2;
int	i,	j;

	for( i = 0 ; i < row ; i++ ){

		sp1 = MBMAP_PIXEL_ADRESS( bmap1, x0+i, y0 );
		sp2 = MBMAP_PIXEL_ADRESS( bmap2, x0+i, y0 );

		for( j = 0 ; j < column ; j++ ){
			tmp = (float)(*sp1++) - (float)(*sp2++);
			sum += tmp*tmp;
		}
	}
	var = sqrt( sum/(row*column) );
	return( var );
}


float
mbmap_average( mbmap bmap1, mbmap bmap2, int x0,int y0,int row,int column )
{
float	sum, 	tmp,	average;
u_char	*sp1,	*sp2;
int	i,	j;

	sum = 0;
	for( i = 0 ; i < row ; i++ ){

		sp1 = MBMAP_PIXEL_ADRESS( bmap1, x0+i, y0 );
		sp2 = MBMAP_PIXEL_ADRESS( bmap2, x0+i, y0 );

		for( j = 0 ; j < column ; j++ )
			sum += (float)(*sp1++) - (float)(*sp2++);
	}
	average = sum/(row*column);
	return( average );
}

