#include <memory.h>

#include	"Uigp/igp.h"
#include	"Umbmap/mbmap.h"


static void	pixRowDup(unsigned char *array, int rows, int columns,
          		int fromRow, int toRow, int copyRows);

static void	pixColDup(unsigned char *array, int rows, int columns,
          		int fromCol, int toCol, int copyCol);

void
mbmap_dcopy_sub_mbmap_to_uarray(mbmap bmap,
			int x0, int y0, int row, int column,
			u_char *arry, int acolumn)
{
u_char	*sp,	*tp;
int     i,	no;
int	x1,	x2,	y1,	y2;
	
	tp = arry;

	if( (x1 = x0) < 0 ){
		tp = arry + (-x1)*acolumn;
		x1 = 0;
	}

	if( (x2 = x0 + row) > MBMAP_ROW( bmap) )
		x2 = MBMAP_ROW( bmap);

	if( ( y1 = y0 ) < 0 ){
		tp += (-y1);
		y1 = 0;
	}

	if( (y2 = y0 + column) > MBMAP_COLUMN( bmap) )
		y2 = MBMAP_COLUMN( bmap);

	


	for(i = x1; i < x2; i++, tp += acolumn) {
		sp = MBMAP_PIXEL_ADRESS(bmap, i, y1);
		memcpy( (char *)tp, (char *)sp, y2-y1);
	}



	if( y0 < 0 )
		pixColDup( arry, row, acolumn,
					-y0, 0, -y0 );

	no = y0 + column - MBMAP_COLUMN(bmap);
	if( no > 0 )
		pixColDup( arry, row, acolumn,
			column - no-1, column - no, no );

	if( x0 < 0 )
		pixRowDup( arry, row, acolumn,
					-x0, 0, -x0 );

	no = x0 + row - MBMAP_ROW(bmap);
	if( no > 0 )
		pixRowDup( arry, row, acolumn,
			row-no -1, row - no, no );
}




/*
 * pixRowDup - copies a row number "fromRow" of 2D array "char
 * array[rows][columns];" to "copyRows" consecuent rows of the same array
 * starting from the row "toRow"
 * 
 * Note: if (toRow+copyRows > rows ) number of copied strings will be (rows -
 * toRow)
 */

static void
pixRowDup(unsigned char *array, int rows, int columns,
	  int fromRow, int toRow, int copyRows)
{
	int     i, last;
	unsigned char *fromP, *toP;

	last = MIN(toRow + copyRows, rows);

	fromP = array + columns * fromRow;

	for (i = toRow, toP = array + columns * toRow; i < last;
	     i++, toP += columns) {
		if (i != fromRow)
			memcpy((char *)toP, (char *)fromP, columns);
	}
}

static void
pixColDup(unsigned char *array, int rows, int columns,
	  int fromCol, int toCol, int copyCol)
{
	int     i, last;
	unsigned char *fromP, *toP;

	last = MIN(toCol + copyCol, columns);

	fromP = array + fromCol;

/*
	for(i = toCol, toP = array + toCol; i < rows;
*/
	for(i = 0, toP = array + toCol; i < rows;
	     i++, toP += columns, fromP += columns)
		memset((char *)toP, (int) *fromP, copyCol);

}
