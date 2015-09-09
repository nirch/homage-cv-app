#include <string.h>


#include "ImageType/ImageType.h"
#include "ImageDithering.h"

/* based on algorithm described in www.student.dtu.dk/~c971592/image/specs/dither.txt 
 * Error dispersion
 */

#define RGB_TO_Y(y) (PUSH_TO_RANGE((u_int)(IMAGE4_RED(y)*0.29900 + \
								0.58700 * IMAGE4_GREEN(y) + \
								0.11400 * IMAGE4_BLUE(y) + 0.5), 0, 255 ) )

/*
			X	3
			3	2
*/
image_type * 
image_dithering_error_simpler( image_type *im, image_type *new_im )
{
u_int	*sp, *dp;
int		gray;
int		row, col, i, j;
int		cerror, cerror_3_8, cerror_2_8; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)*dp)/8;	//now in dp summer of previouse error

		if ( gray < 128 )
		{
			*dp = 0;
			cerror = gray;
		}
		else
		{
			*dp = 0x00FFFFFF;
			cerror = gray - 255;
		}
		cerror_3_8 = cerror * 3;
		cerror_2_8 = cerror * 2;

		if ( j < col - 1 )
			*(dp + 1) += (u_int)cerror_3_8;
		if ( i < row - 1 )
		{
			*(dp + col ) += (u_int)cerror_3_8;
			if ( j < col - 1 )
				*(dp + col + 1) += (u_int)cerror_2_8;
		}

		sp++;
		dp++;
	}

	return new_im;
}

/*
			X	7
		3	5	1
*/
image_type * 
image_dithering_error_floyd( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)*dp)/16;	//now in dp summer of previouse error

		if ( gray < 128 )
		{
			*dp = 0;
			cerror = gray;
		}
		else
		{
			*dp = 0x00FFFFFF;
			cerror = gray - 255;
		}
		cerror_1_16 = cerror;
		cerror_3_16 = cerror * 3;
		cerror_5_16 = cerror * 5;
		cerror_7_16 = cerror * 7;

		if ( j < col - 1 )
			*(dp + 1) += (u_int)cerror_7_16;
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_int)cerror_5_16;
			if ( j < col - 1 )
				*(dp1 + 1) += (u_int)cerror_1_16;
			if ( j > 0 )
				*(dp1 - 1) += (u_int)cerror_3_16;
		}

		sp++;
		dp++;
	}

	return new_im;
}

/*
			X	8	4
	2	4	8	4	2
*/
image_type * 
image_dithering_error_burke( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_2_32, cerror_4_32, cerror_8_32; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)*dp)/32;	//now in dp summer of previouse error

		if ( gray < 128 )
		{
			*dp = 0;
			cerror = gray;
		}
		else
		{
			*dp = 0x00FFFFFF;
			cerror = gray - 255;
		}
		cerror_2_32 = cerror * 2;
		cerror_4_32 = cerror * 4;
		cerror_8_32 = cerror * 8;

		if ( j < col - 1 )
		{
			*(dp + 1) += (u_int)cerror_8_32;
			if ( j < col - 2 )
				*(dp + 2) += (u_int)cerror_4_32;
		}
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_int)cerror_8_32;
			if ( j < col - 1 )
			{
				*(dp1 + 1) += (u_int)cerror_4_32;
				if ( j < col - 2 )
					*(dp1 + 2) += (u_int)cerror_2_32;
			}
			if ( j > 0 )
			{
				*(dp1 - 1) += (u_int)cerror_4_32;
				if ( j > 1 )
					*(dp1 - 2) += (u_int)cerror_2_32;
			}
		}

		sp++;
		dp++;
	}

	return new_im;
}

/*
			X	7	5
	3	5	7	5	7
	1	3	5	3	1
*/
image_type * 
image_dithering_error_jarvis( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_48, cerror_3_48, cerror_5_48, cerror_7_48;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)*dp)/48;	//now in dp summer of previouse error

		if ( gray < 128 )
		{
			*dp = 0;
			cerror = gray;
		}
		else
		{
			*dp = 0x00FFFFFF;
			cerror = gray - 255;
		}
		cerror_1_48 = cerror;
		cerror_3_48 = cerror * 3;
		cerror_5_48 = cerror * 5;
		cerror_7_48 = cerror * 7;

		if ( j < col - 1 )
		{
			*(dp + 1) += (u_int)cerror_7_48;
			if ( j < col - 2 )
				*(dp + 2) += (u_int)cerror_5_48;
		}
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_int)cerror_7_48;
			if ( j < col - 1 )
			{
				*(dp1 + 1) += (u_int)cerror_5_48;
				if ( j < col - 2 )
					*(dp1 + 2) += (u_int)cerror_3_48;
			}
			if ( j > 0 )
			{
				*(dp1 - 1) += (u_int)cerror_5_48;
				if ( j > 1 )
					*(dp1 - 2) += (u_int)cerror_3_48;
			}
		}
		if ( i < row - 2 )
		{
			dp2 = dp1 + col;
			*dp2 += (u_int)cerror_5_48;
			if ( j < col - 1 )
			{
				*(dp2 + 1) += (u_int)cerror_3_48;
				if ( j < col - 2 )
					*(dp2 + 2) += (u_int)cerror_1_48;
			}
			if ( j > 0 )
			{
				*(dp2 - 1) += (u_int)cerror_3_48;
				if ( j > 1 )
					*(dp2 - 2) += (u_int)cerror_1_48;
			}
		}

		sp++;
		dp++;
	}

	return new_im;
}

/*
			X	8 	4
	2	4	8	4	2
	1	2	4	2	1
*/
image_type * 
image_dithering_error_stucki( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_42, cerror_2_42, cerror_4_42, cerror_8_42;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)*dp)/42;	//now in dp summer of previouse error

		if ( gray < 128 )
		{
			*dp = 0;
			cerror = gray;
		}
		else
		{
			*dp = 0x00FFFFFF;
			cerror = gray - 255;
		}
		cerror_1_42 = cerror;
		cerror_2_42 = cerror * 2;
		cerror_4_42 = cerror * 4;
		cerror_8_42 = cerror * 8;

		if ( j < col - 1 )
		{
			*(dp + 1) += (u_int)cerror_8_42;
			if ( j < col - 2 )
				*(dp + 2) += (u_int)cerror_4_42;
		}
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_int)cerror_8_42;
			if ( j < col - 1 )
			{
				*(dp1 + 1) += (u_int)cerror_4_42;
				if ( j < col - 2 )
					*(dp1 + 2) += (u_int)cerror_2_42;
			}
			if ( j > 0 )
			{
				*(dp1 - 1) += (u_int)cerror_4_42;
				if ( j > 1 )
					*(dp1 - 2) += (u_int)cerror_2_42;
			}
		}
		if ( i < row - 2 )
		{
			dp2 = dp1 + col;
			*dp2 += (u_int)cerror_4_42;
			if ( j < col - 1 )
			{
				*(dp2 + 1) += (u_int)cerror_2_42;
				if ( j < col - 2 )
					*(dp2 + 2) += (u_int)cerror_1_42;
			}
			if ( j > 0 )
			{
				*(dp2 - 1) += (u_int)cerror_2_42;
				if ( j > 1 )
					*(dp2 - 2) += (u_int)cerror_1_42;
			}
		}

		sp++;
		dp++;
	}

	return new_im;
}


int index_table_16[] = 
	{   7,   23,   39,   55,   71,   87,   103,   119, 
	  135,  151,  167,  183,  199,  215,   231,   248, 0x00FFFFFF};
														//border

int gray_table_16[] = 
	{ 0,  15,   31,   47,   63,   79,    95,   111,   127, 
		 143, 159,   175,  191,  207,   223,   239,   255 };

int color_table_16[]  = { 
	0x00000000, 0x000F0F0F, 0x001F1F1F, 0x002F2F2F,  
	0x003F3F3F, 0x004F4F4F, 0x005F5F5F, 0x006F6F6F, 
	0x007F7F7F, 0x008F8F8F, 0x009F9F9F, 0x00AFAFAF, 
	0x00BFBFBF, 0x00CFCFCF, 0x00DFDFDF, 0x00EFEFEF, 0x00FFFFFF };
/*
			X	7
		3	5	1
*/
image_type * 
image_dithering_error_gray16( image_type *im, image_type *new_im )
{
u_int	*sp, *dp;
int		gray;
int		row, col, i, j, ind;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		ind = 0;
		while ( gray > index_table_16[ind] )
			ind++;
		*dp = color_table_16[ind];
		sp++;
		dp++;
	}

	return new_im;
}

image_type * 
image_dithering_error_floyd_gray16( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray, gray_from_table;
int		row, col, i, j, ind;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)*dp)/16;	//now in dp summer of previouse error
		ind = 0;
		while ( gray > index_table_16[ind] )
			ind++;
		gray_from_table = gray_table_16[ind];
		*dp = color_table_16[ind];
		cerror = gray_from_table - gray;

		cerror_1_16 = cerror;
		cerror_3_16 = cerror * 3;
		cerror_5_16 = cerror * 5;
		cerror_7_16 = cerror * 7;

		if ( j < col - 1 )
			*(dp + 1) += (u_int)cerror_7_16;
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_int)cerror_5_16;
			if ( j < col - 1 )
				*(dp1 + 1) += (u_int)cerror_1_16;
			if ( j > 0 )
				*(dp1 - 1) += (u_int)cerror_3_16;
		}

		sp++;
		dp++;
	}

	return new_im;
}


/*
			X	7
		3	5	1
*/
image_type * 
image_dithering_error_floyd_grey2b( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dp = (u_int *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray = RGB_TO_Y( *sp );
		gray += ((int)(*dp))/16;	//now in dp summer of previouse error

		if ( gray < 0x40 )
		{
			*dp = 0;
			cerror = gray;
		}
		else if ( gray < 0x80 )
		{
			*dp = 0x00606060;
			cerror = gray-0x60;
		}
		else  if ( gray < 0xC0 )
		{
			*dp = 0x00A0A0A0;
			cerror = gray - 0xA0;
		}
		else
		{
			*dp = 0x00FFFFFF;
			cerror = gray - 0xFF;
		}

		cerror_1_16 = cerror;
		cerror_3_16 = cerror * 3;
		cerror_5_16 = cerror * 5;
		cerror_7_16 = cerror * 7;

		if ( j < col - 1 )
			*(dp + 1) += (u_int)cerror_7_16;
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_int)cerror_5_16;
			if ( j < col - 1 )
				*(dp1 + 1) += (u_int)cerror_1_16;
			if ( j > 0 )
				*(dp1 - 1) += (u_int)cerror_3_16;
		}

		sp++;
		dp++;
	}

	return new_im;
}

/*
image_type * 
image_dithering_error_floyd_bit8( image_type *im, image_type *new_im )
{
u_char	*sp, *dp, *dp1;
int		row, col, i, j;
char	gray;
char	cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 1, 1, NULL );
	memset( new_im->data, 0, row*col );

	sp = (u_char *)im->data;
	dp = (u_char *)new_im->data;

	for( i = 0; i < row; i++ )
    for( j = 0; j < col; j++ )
	{
		gray += ((char)*dp)/16;	//now in dp summer of previouse error

		if ( gray < 128 )
		{
			*dp = 0;
			cerror = gray;
		}
		else
		{
			*dp = 0xFF;
			cerror = gray - 255;
		}
		cerror_1_16 = cerror;
		cerror_3_16 = cerror * 3;
		cerror_5_16 = cerror * 5;
		cerror_7_16 = cerror * 7;

		if ( j < col - 1 )
			*(dp + 1) += (u_char)cerror_7_16;
		if ( i < row - 1 )
		{
			dp1 = dp + col;
			*dp1 += (u_char)cerror_5_16;
			if ( j < col - 1 )
				*(dp1 + 1) += (u_char)cerror_1_16;
			if ( j > 0 )
				*(dp1 - 1) += (u_char)cerror_3_16;
		}

		sp++;
		dp++;
	}

	return new_im;
}
*/

