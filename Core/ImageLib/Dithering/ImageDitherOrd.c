#include "ImageType/ImageType.h"
#include "ImageDithering.h"

#define RGB_TO_Y(y) (PUSH_TO_RANGE((u_int)(IMAGE4_RED(y)*0.29900 + \
								0.58700 * IMAGE4_GREEN(y) + \
								0.11400 * IMAGE4_BLUE(y) + 0.5), 0, 255 ) )

//index_table_2x2[j] = 255/5 *j
int index_table_2x2[] = { 51, 102, 153, 204, 255 };

//index_table_3x3[j] = 255/10 *j
int index_table_3x3[] = { 26, 51, 77, 102, 128, 153, 179, 204, 230, 255 };

//index_table_4x4[j] = 256/16 *j
int index_table_4x4[] = { 15, 30,  45,  60,  75,  90,  105, 120, 
						 135, 150, 165, 180, 195, 210, 225, 240, 255 };

char  dithering_ordered_2x2[]	={	1,	3,
									4,	2 };

char  dithering_ordered_3x3_1[]	={	8,	3,	4, 
									6,	1,	2,
									7,	5,	9 };

char  dithering_ordered_3x3_2[]	={	1,	7,	4, 
									5,	8,	3,
									6,	2,	9 };

char  dithering_ordered_4x4[]	={	1,	9,	3,	11,
									13,	5,	15,	7,
									4,	12,	2,	10,
									16,	8,	14,	6 };

/* based on algorithm described in www.student.dtu.dk/~c971592/image/specs/dither.txt 
 * ordered dither
 */
image_type * 
image_dithering_ordered_2x2( image_type *im, image_type *new_im )
{
int		l_rect = 2;
u_int	*sp, *dp;
int		gray;
int		row, col, i, j, ni, nj; 
char	ind;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );

	for( i = 0; i < row; i += ni )
    for( j = 0; j < col; j += nj )
	{
		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			sp = IMAGE4_PIXEL( im, i+ni, j+nj );
			dp = IMAGE4_PIXEL( new_im, i+ni, j+nj );
			gray = RGB_TO_Y( *sp );

			ind = 0;
			while ( gray > index_table_2x2[ind] )	
				ind++;

			if ( ind < dithering_ordered_2x2[ni * l_rect + nj] )
				*dp = 0;
			else
				*dp = 0x00FFFFFF;
		}
	}
	return new_im;
}

image_type * 
image_dithering_ordered_3x3_1( image_type *im, image_type *new_im )
{
int		l_rect = 3;
u_int	*sp, *dp;
int		gray;
int		row, col, i, j, ni, nj; 
char	ind;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );

	for( i = 0; i < row; i += ni )
    for( j = 0; j < col; j += nj )
	{
		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			sp = IMAGE4_PIXEL( im, i+ni, j+nj );
			dp = IMAGE4_PIXEL( new_im, i+ni, j+nj );
			gray = RGB_TO_Y( *sp );

			ind = 0;
			while ( gray > index_table_3x3[ind] )	
				ind++;

			if ( ind < dithering_ordered_3x3_1[ni * l_rect + nj] )
				*dp = 0;
			else
				*dp = 0x00FFFFFF;
		}
	}
	return new_im;
}

image_type * 
image_dithering_ordered_3x3_2( image_type *im, image_type *new_im )
{
int		l_rect = 3;
u_int	*sp, *dp;
int		gray;
int		row, col, i, j, ni, nj; 
char	ind;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );

	for( i = 0; i < row; i += ni )
    for( j = 0; j < col; j += nj )
	{
		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			sp = IMAGE4_PIXEL( im, i+ni, j+nj );
			dp = IMAGE4_PIXEL( new_im, i+ni, j+nj );
			gray = RGB_TO_Y( *sp );

			ind = 0;
			while ( gray > index_table_3x3[ind] )	
				ind++;

			if ( ind < dithering_ordered_3x3_2[ni * l_rect + nj] )
				*dp = 0;
			else
				*dp = 0x00FFFFFF;
		}
	}
	return new_im;
}

image_type * 
image_dithering_ordered_4x4( image_type *im, image_type *new_im )
{
int		l_rect = 4;
u_int	*sp, *dp;
int		gray;
int		row, col, i, j, ni, nj; 
char	ind;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );

	for( i = 0; i < row; i += ni )
    for( j = 0; j < col; j += nj )
	{
		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			sp = IMAGE4_PIXEL( im, i+ni, j+nj );
			dp = IMAGE4_PIXEL( new_im, i+ni, j+nj );
			gray = RGB_TO_Y( *sp );

			ind = 0;
			while ( gray > index_table_4x4[ind] )	
				ind++;

			if ( ind < dithering_ordered_4x4[ni * l_rect + nj] )
				*dp = 0;
			else
				*dp = 0x00FFFFFF;
		}
	}
	return new_im;
}

