#include <memory.h>

#include	"Uigp/igp.h"
#include "ImageType/ImageType.h"


static void	pixRowDup(unsigned char *array, int rows, int columns,
          		int fromRow, int toRow, int copyRows);

static void	pixColDup(unsigned char *array, int rows, int columns,
          		int fromCol, int toCol, int copyCol);



void
image4_cut_uarray( image_type *im,
			int x0, int y0, int row, int column,
			u_char *aY,
			u_char *aQ,
			u_char *aA,
			int acolumn)
{
//u_int	*sp;
u_char	*pY,	*pQ,	*pA;
int     i,	j,	no,	algin;
int	x1,	x2,	y1,	y2;


	pY = aY;
	pQ = aQ;
	pA = aA;

	if( (x1 = x0) < 0 ){
		pY = aY + (-x1)*acolumn;
		pQ = aQ + (-x1)*acolumn;
		pA = aA + (-x1)*acolumn;
		x1 = 0;
	}

	if( (x2 = x0 + row) > IMAGE_ROW( im) )
		x2 = IMAGE_ROW( im);

	if( ( y1 = y0 ) < 0 ){
		pY += (-y1);
		pQ += (-y1);
		pA += (-y1);
		y1 = 0;
	}

	if( (y2 = y0 + column) > IMAGE_COLUMN( im) )
		y2 = IMAGE_COLUMN( im);

	

	algin = acolumn - ( y2 - y1 );

	if( im->depth == 4 ){
		for(i = x1; i < x2; i++ ){
			u_int *sp = IMAGE4_PIXEL( im, i, y1 );

			for( j = y1 ; j < y2 ; j++ ){

				*pY++ = IMAGE4_RED(*sp);
				*pQ++ = IMAGE4_GREEN(*sp);
				*pA++ = IMAGE4_BLUE(*sp);
				sp++;
			}

			pY += algin;
			pQ += algin;
			pA += algin;
		}
	}
	else 
		if( im->depth == 3 ){
			for(i = x1; i < x2; i++ ){
				u_char *sp = IMAGE_PIXEL( im, i, y1 );

				for( j = y1 ; j < y2 ; j++ ){

					*pY++ = *sp++;
					*pQ++ = *sp++;
					*pA++ = *sp++;
				}

				pY += algin;
				pQ += algin;
				pA += algin;
			}
		}




	if( y0 < 0 ){
		pixColDup( aY, row, acolumn, -y0, 0, -y0 );
		pixColDup( aQ, row, acolumn, -y0, 0, -y0 );
		pixColDup( aA, row, acolumn, -y0, 0, -y0 );
	}


	no = y0 + column - IMAGE_COLUMN(im);
	if( no > 0 ){
		pixColDup( aY, row, acolumn,
			column - no-1, column - no, no );
		pixColDup( aQ, row, acolumn,
			column - no-1, column - no, no );
		pixColDup( aA, row, acolumn,
			column - no-1, column - no, no );
	}


	if( x0 < 0 ){
		pixRowDup( aY, row, acolumn, -x0, 0, -x0 );
		pixRowDup( aQ, row, acolumn, -x0, 0, -x0 );
		pixRowDup( aA, row, acolumn, -x0, 0, -x0 );
	}

	no = x0 + row - IMAGE_ROW(im);
	if( no > 0 ){
		pixRowDup( aY, row, acolumn, row-no -1, row - no, no );
		pixRowDup( aQ, row, acolumn, row-no -1, row - no, no );
		pixRowDup( aA, row, acolumn, row-no -1, row - no, no );
	}
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
