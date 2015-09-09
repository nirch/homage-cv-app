#include <stdlib.h>
#include <time.h>

#include "ImageType/ImageType.h"
#include "ImageDithering.h"

#define RGB_TO_Y(y) (PUSH_TO_RANGE((u_int)(IMAGE4_RED(y)*0.29900 + \
								0.58700 * IMAGE4_GREEN(y) + \
								0.11400 * IMAGE4_BLUE(y) + 0.5), 0, 255 ) )

extern int index_table_3x3[]; //index_table_3x3[j] = 255/10 *j


u_int  dither4_patterns1[] = {
	0,			0,			0,			0,			0,			0,			0,			0,			0, 
	0,			0,			0,			0,			0x00FFFFFF, 0,			0,			0,			0, 
	0,			0,			0,			0,			0x00FFFFFF,	0,			0,			0x00FFFFFF, 0,
	0,			0,			0,			0x00FFFFFF, 0x00FFFFFF,	0,			0,			0x00FFFFFF, 0,
	0,			0,			0,			0x00FFFFFF, 0x00FFFFFF,	0,			0x00FFFFFF, 0x00FFFFFF, 0,
	0,			0,			0,			0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0,
	0,			0x00FFFFFF, 0,			0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0,
	0x00FFFFFF,	0x00FFFFFF, 0,			0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0,
	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0,
	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF};

u_int  dither4_patterns2[] = {
	0,			0,			0,			0,			0,			0,			0,			0,			0, 
	0x00FFFFFF, 0,			0,			0,			0,			0,			0,			0,			0, 
	0x00FFFFFF, 0,			0,			0,			0,			0,			0,			0x00FFFFFF,	0, 
	0x00FFFFFF, 0,			0,			0,			0,			0x00FFFFFF, 0,			0x00FFFFFF,	0, 
	0x00FFFFFF, 0,			0x00FFFFFF, 0,			0,			0x00FFFFFF, 0,			0x00FFFFFF,	0, 
	0x00FFFFFF, 0,			0x00FFFFFF, 0x00FFFFFF, 0,			0x00FFFFFF, 0,			0x00FFFFFF,	0, 
	0x00FFFFFF, 0,			0x00FFFFFF, 0x00FFFFFF, 0,			0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF,	0, 
	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0,			0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF,	0, 
	0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF,	0x00FFFFFF,	0, 
	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF};


/* based on algorithm described in www.student.dtu.dk/~c971592/image/specs/dither.txt 
 * random dither
 */
image_type * 
image_dithering_random( image_type *im, image_type *new_im )
{
u_int	*sp, *dp;
u_int	gray;
int		row, col, i, j;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if ( new_im == NULL )
		new_im = image_create( row, col, 4, 1, NULL );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	srand( (unsigned)time( NULL ) );

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		if ( gray > (u_int)((rand() << 8)/RAND_MAX) )
			*dp = 0x00FFFFFF;
		else
			*dp = 0x00000000;
		sp++;
		dp++;
	}

	return( new_im );	
}

/* based on algorithm described in www.student.dtu.dk/~c971592/image/specs/dither.txt 
 *(pattern dither)
 */
image_type * 
image_dithering_pattern1( image_type *im, image_type *new_im )
{
int		l_rect = 3;
u_int	*sp, *dp;
int		gray, ind;
u_int	*cursor;
int		row, col, i, j, ni, nj; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );

	for( i = 0; i < row; i += ni )
    for( j = 0; j < col; j += nj )
	{
		gray = 0;
		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			sp = IMAGE4_PIXEL( im, i+ni, j+nj );
			gray += RGB_TO_Y( *sp );
		}
		gray /= (ni*nj);

		ind = 0;
		while ( gray > index_table_3x3[ind] )	
			ind++;
		cursor = dither4_patterns1 + ind*9;

		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			dp = IMAGE4_PIXEL( new_im, i+ni, j+nj );
			*dp = *(cursor + ni * l_rect + nj);
		}
	}
	return new_im;
}


image_type * 
image_dithering_pattern2( image_type *im, image_type *new_im )
{
int		l_rect = 3;
u_int	*sp, *dp;
int		gray, ind;
u_int	*cursor;
int		row, col, i, j, ni, nj; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );

	for( i = 0; i < row; i += ni )
    for( j = 0; j < col; j += nj )
	{
		gray = 0;
		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			sp = IMAGE4_PIXEL( im, i+ni, j+nj );
			gray += RGB_TO_Y( *sp );
		}
		gray /= (ni*nj);

		ind = 0;
		while ( gray > index_table_3x3[ind] )	
			ind++;
		cursor = dither4_patterns2 + ind*9;

		for( ni = 0; ni < l_rect && i + ni < row; ni++ )
		for( nj = 0; nj < l_rect && j + nj < col; nj++ )
		{
			dp = IMAGE4_PIXEL( new_im, i+ni, j+nj );
			*dp = *(cursor + ni * l_rect + nj);
		}
	}
	return new_im;
}

