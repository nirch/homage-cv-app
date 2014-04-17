#include <memory.h>
#include <stdlib.h>
#include <math.h>

#include "ImageType/ImageType.h"
#include "ImageDithering.h"

/* based on algorithm described in www.student.dtu.dk/~c971592/image/specs/dither.txt 
 * Error dispersion
 */

#define RGB_TO_Y(y) (PUSH_TO_RANGE((u_int)(IMAGE4_RED(y)*0.29900 + \
								0.58700 * IMAGE4_GREEN(y) + \
								0.11400 * IMAGE4_BLUE(y) + 0.5), 0, 255 ) )

#define STEP 10

/*
			X	3
			3	2
*/
image_type * 
image_dithering_error_simpler_exp( image_type *im, image_type *new_im )
{
u_int	*sp, *dp;
int		gray;
int		row, col, i, j;
int		cerror, cerror_3_8, cerror_2_8; 
div_t	div_result;

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

		div_result = div( i, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
				*(dp + 1) += (u_int)cerror_3_8;
			if ( i < row - 1 )
			{
				*(dp + col ) = 0;
				if ( j < col - 1 )
					*(dp + col + 1) = 0;
			}

			sp++;
			dp++;
			continue;
		}

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
image_dithering_error_floyd_exp( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 
div_t	div_result;

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

		div_result = div( i, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
				*(dp + 1) += (u_int)cerror_7_16;
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 = 0;
				if ( j < col - 1 )
					*(dp1 + 1) = 0;
				if ( j > 0 )
					*(dp1 - 1) = 0;
			}

			sp++;
			dp++;
			continue;
		}

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
image_dithering_error_burke_exp( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_2_32, cerror_4_32, cerror_8_32; 
div_t	div_result;

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

		div_result = div( i, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) += (u_int)cerror_8_32;
				if ( j < col - 2 )
					*(dp + 2) += (u_int)cerror_4_32;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 = 0;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
				}
				if ( j > 0 )
				{
					*(dp1 - 1) = 0;
					if ( j > 1 )
						*(dp1 - 2) = 0;
				}
			}

			sp++;
			dp++;
			continue;
		}

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
image_dithering_error_jarvis_exp( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_48, cerror_3_48, cerror_5_48, cerror_7_48;
div_t	div_result;

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

		div_result = div( i, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) += (u_int)cerror_7_48;
				if ( j < col - 2 )
					*(dp + 2) += (u_int)cerror_5_48;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 = 0;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
				}
				if ( j > 0 )
				{
					*(dp1 - 1) = 0;
					if ( j > 1 )
						*(dp1 - 2) = 0;
				}
			}
			if ( i < row - 2 )
			{
				dp2 = dp1 + col;
				*dp2 = 0;
				if ( j < col - 1 )
				{
					*(dp2 + 1) = 0;
					if ( j < col - 2 )
						*(dp2 + 2) = 0;
				}
				if ( j > 0 )
				{
					*(dp2 - 1) = 0;
					if ( j > 1 )
						*(dp2 - 2) = 0;
				}
			}

			sp++;
			dp++;
			continue;
		}

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
image_dithering_error_stucki_exp( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_42, cerror_2_42, cerror_4_42, cerror_8_42;
div_t	div_result;

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

		div_result = div( i, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) += (u_int)cerror_8_42;
				if ( j < col - 2 )
					*(dp + 2) += (u_int)cerror_4_42;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 = 0;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
				}
				if ( j > 0 )
				{
					*(dp1 - 1) = 0;
					if ( j > 1 )
						*(dp1 - 2) = 0;
				}
			}
			if ( i < row - 2 )
			{
				dp2 = dp1 + col;
				*dp2 = 0;
				if ( j < col - 1 )
				{
					*(dp2 + 1) = 0;
					if ( j < col - 2 )
						*(dp2 + 2) = 0;
				}
				if ( j > 0 )
				{
					*(dp2 - 1) = 0;
					if ( j > 1 )
						*(dp2 - 2) = 0;
				}
			}

			sp++;
			dp++;
			continue;
		}

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



/////////////////////////////////////////////////

/*
			X	3
			3	2
*/
image_type * 
image_dithering_error_simpler_exp1( image_type *im, image_type *new_im )
{
u_int	*sp, *dp;
int		gray;
int		row, col, i, j;
int		cerror, cerror_3_8, cerror_2_8; 
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
				*(dp + 1) = 0;
			if ( i < row - 1 )
			{
				*(dp + col ) += (u_int)cerror_3_8;
				if ( j < col - 1 )
					*(dp + col + 1) = 0;
			}

			sp++;
			dp++;
			continue;
		}

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
image_dithering_error_floyd_exp1( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
				*(dp + 1) = 0;
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 += (u_int)cerror_5_16;
				if ( j < col - 1 )
					*(dp1 + 1) = 0;
				if ( j > 0 )
					*(dp1 - 1) += (u_int)cerror_3_16;
			}

			sp++;
			dp++;
			continue;
		}

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
image_dithering_error_burke_exp1( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1;
int		gray;
int		row, col, i, j;
int		cerror, cerror_2_32, cerror_4_32, cerror_8_32; 
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) = 0;
				if ( j < col - 2 )
					*(dp + 2) = 0;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 += (u_int)cerror_8_32;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
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
			continue;
		}

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
image_dithering_error_jarvis_exp1( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_48, cerror_3_48, cerror_5_48, cerror_7_48;
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) = 0;
				if ( j < col - 2 )
					*(dp + 2) = 0;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 += (u_int)cerror_7_48;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
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
					*(dp2 + 1) = 0;
					if ( j < col - 2 )
						*(dp2 + 2) = 0;
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
			continue;
		}

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
image_dithering_error_stucki_exp1( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_42, cerror_2_42, cerror_4_42, cerror_8_42;
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) = 0;
				if ( j < col - 2 )
					*(dp + 2) = 0;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 += (u_int)cerror_8_42;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
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
					*(dp2 + 1) = 0;
					if ( j < col - 2 )
						*(dp2 + 2) = 0;
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
			continue;
		}

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




/////////////////////////////////////////////////

int pixel_not_equal( u_int *sp, u_int *sp1, int A )
{
int DELTA = 8;
int red_delta, grn_delta, blu_delta;
u_int y, y1;

	switch( A ) {
		case -1: 
			y = RGB_TO_Y((*sp));
			y1 = RGB_TO_Y((*sp1));
			return (ABS( (int)(y)-(int)(y1) ) > DELTA);
		case  SP_R: 
			red_delta = (((*sp) >> 16) & 0xFF) - (((*sp1) >> 16) & 0xFF);
			return (ABS( red_delta ) > DELTA);
		case  SP_G: 
			grn_delta = (((*sp) >> 8) & 0xFF) - (((*sp1) >> 8) & 0xFF);
			return (ABS( grn_delta ) > DELTA);
		case  SP_B: 
			blu_delta = ((*sp) & 0xFF) - ((*sp1) & 0xFF);
			return (ABS( blu_delta ) > DELTA);
	}

	return 0;
}

/*
			X	3
			3	2
*/
image_type * 
image_dithering_error_simpler_exp2( image_type *im, image_type *new_im )
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
			if ( pixel_not_equal( sp, sp+1, -1 ) )
				*(dp + 1) = 0;
			else
				*(dp + 1) += (u_int)cerror_3_8;

		if ( i < row - 1 )
		{
			if ( pixel_not_equal( sp, sp+col, -1 ) )
				*(dp + col ) = 0;
			else
				*(dp + col ) += (u_int)cerror_3_8;

			if ( j < col - 1 )
				if ( pixel_not_equal( sp, sp+col+1, -1 ) )
					*(dp + col + 1) = 0;
				else
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
image_dithering_error_floyd_exp2( image_type *im, image_type *new_im )
{
u_int	*sp, *sp1, *dp, *dp1;
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
			if ( pixel_not_equal( sp, sp+1, -1 ) )
				*(dp + 1) = 0;
			else
				*(dp + 1) += (u_int)cerror_7_16;

		if ( i < row - 1 )
		{
			sp1 = sp + col;
			dp1 = dp + col;

			if ( pixel_not_equal( sp, sp1, -1 ) )
				*dp1 = 0;
			else
				*dp1 += (u_int)cerror_5_16;

			if ( j < col - 1 )
				if ( pixel_not_equal( sp, sp1+1, -1 ) )
					*(dp1 + 1) = 0;
				else
					*(dp1 + 1) += (u_int)cerror_1_16;

			if ( j > 0 )
				if ( pixel_not_equal( sp, sp1-1, -1 ) )
					*(dp1 - 1) = 0;
				else
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
image_dithering_error_burke_exp2( image_type *im, image_type *new_im )
{
u_int	*sp, *sp1, *dp, *dp1;
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
			if ( pixel_not_equal( sp, sp+1, -1 ) )
				*(dp + 1) = 0;
			else
				*(dp + 1) += (u_int)cerror_8_32;

			if ( j < col - 2 )
				if ( pixel_not_equal( sp, sp+2, -1 ) )
					*(dp + 2) = 0;
				else
					*(dp + 2) += (u_int)cerror_4_32;
		}

		if ( i < row - 1 )
		{
			sp1 = sp + col;
			dp1 = dp + col;

			if ( pixel_not_equal( sp, sp1, -1 ) )
				*dp1 = 0;
			else
				*dp1 += (u_int)cerror_8_32;

			if ( j < col - 1 )
			{
				if ( pixel_not_equal( sp, sp1+1, -1 ) )
					*(dp1 + 1) = 0;
				else
					*(dp1 + 1) += (u_int)cerror_4_32;

				if ( j < col - 2 )
					if ( pixel_not_equal( sp, sp1+2, -1 ) )
						*(dp1 + 2) = 0;
					else
						*(dp1 + 2) += (u_int)cerror_2_32;
			}

			if ( j > 0 )
			{
				if ( pixel_not_equal( sp, sp1-1, -1 ) )
					*(dp1 - 1) = 0;
				else
					*(dp1 - 1) += (u_int)cerror_4_32;

				if ( j > 1 )
					if ( pixel_not_equal( sp, sp1-2, -1 ) )
						*(dp1 - 2) = 0;
					else
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
image_dithering_error_jarvis_exp2( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_48, cerror_3_48, cerror_5_48, cerror_7_48;
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) = 0;
				if ( j < col - 2 )
					*(dp + 2) = 0;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 += (u_int)cerror_7_48;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
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
					*(dp2 + 1) = 0;
					if ( j < col - 2 )
						*(dp2 + 2) = 0;
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
			continue;
		}

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
image_dithering_error_stucki_exp2( image_type *im, image_type *new_im )
{
u_int	*sp, *dp, *dp1, *dp2;
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_42, cerror_2_42, cerror_4_42, cerror_8_42;
div_t	div_result;

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

		div_result = div( j, STEP );
		if ( div_result.rem == 0 ) 
		{
			if ( j < col - 1 )
			{
				*(dp + 1) = 0;
				if ( j < col - 2 )
					*(dp + 2) = 0;
			}
			if ( i < row - 1 )
			{
				dp1 = dp + col;
				*dp1 += (u_int)cerror_8_42;
				if ( j < col - 1 )
				{
					*(dp1 + 1) = 0;
					if ( j < col - 2 )
						*(dp1 + 2) = 0;
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
					*(dp2 + 1) = 0;
					if ( j < col - 2 )
						*(dp2 + 2) = 0;
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
			continue;
		}

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



/////////////////////////////////////////////////
/*
image_type * 
image_dithering_error_floyd_color6b_exp( image_type *im, image_type *new_im )
{
u_int	*sp, *sp1, *dpn, *dp1;
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
		dp[A] = data[A]= malloc( row*col*sizeof( int ) );
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
				if ( pixel_not_equal( sp, sp+1, A ) )
					*((dp[A]) + 1) = 0;
				else
					*((dp[A]) + 1) += (u_int)cerror_7_16;
			if ( i < row - 1 )
			{
				sp1 = sp + col;
				dp1 = (dp[A]) + col;
				if ( pixel_not_equal( sp, sp1, A ) )
					*dp1 = 0;
				else
					*dp1 += (u_int)cerror_5_16;

				if ( j < col - 1 )
					if ( pixel_not_equal( sp, sp1+1, A ) )
						*(dp1 + 1) = 0;
					else
						*(dp1 + 1) += (u_int)cerror_1_16;

				if ( j > 0 )
					if ( pixel_not_equal( sp, sp1-1, A ) )
						*(dp1 - 1) = 0;
					else
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
*/
/////////////////////////////////////////////////

image_type * 
image_dithering_error_floyd_color( image_type *im, image_type *new_im,
									   int bpp)
{
u_int	*sp, *dpn, *dp1;
u_int	*dp[SP_NO], *data[SP_NO];
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 
int		A;
int		level_dn, level_up, level_2, k;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dpn = (u_int *)new_im->data;

	level_dn = 0x100 >> bpp;
	level_2  = level_dn >> 1;
	level_up = level_dn*( (1<<bpp) - 1 );

	for( A = SP_R; A < SP_NO; A++ )
	{
		dp[A] = data[A]= malloc( row*col*sizeof( int ) );
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


			if ( gray < level_dn )
			{
				*(dp[A]) = 0;
				cerror = gray;
			}
			else if  ( gray >= level_up )
			{
				*(dp[A]) = 0xFF;
				cerror = gray - 0xFF;
			}
			else
			{
				k = 1;
				while ( gray >= k*level_dn ) 
					k++;

				*(dp[A]) = k*level_dn-level_2;
				cerror = gray - (*(dp[A]));
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
image_dithering_error_floyd_color_exp( image_type *im, image_type *new_im,
									   int bpp)
{
u_int	*sp, *sp1, *dpn, *dp1;
u_int	*dp[SP_NO], *data[SP_NO];
int		gray;
int		row, col, i, j;
int		cerror, cerror_1_16, cerror_3_16, cerror_5_16, cerror_7_16; 
int		A;
int		level_dn, level_up, level_2, k;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	if (new_im == NULL)
		new_im = image_create( row, col, 4, 1, NULL );
	memset( new_im->data, 0, row*col*4 );

	sp = (u_int *)im->data;
	dpn = (u_int *)new_im->data;

	level_dn = 0x100 >> bpp;
	level_2  = level_dn >> 1;
	level_up = level_dn*( (1<<bpp) - 1 );

	for( A = SP_R; A < SP_NO; A++ )
	{
		dp[A] = data[A]= malloc( row*col*sizeof( int ) );
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


			if ( gray < level_dn )
			{
				*(dp[A]) = 0;
				cerror = gray;
			}
			else if  ( gray >= level_up )
			{
				*(dp[A]) = 0xFF;
				cerror = gray - 0xFF;
			}
			else
			{
				k = 1;
				while ( gray >= k*level_dn ) 
					k++;

				*(dp[A]) = k*level_dn-level_2;
				cerror = gray - (*(dp[A]));
			}


			cerror_1_16 = cerror;
			cerror_3_16 = cerror * 3;
			cerror_5_16 = cerror * 5;
			cerror_7_16 = cerror * 7;

			if ( j < col - 1 )
				if ( pixel_not_equal( sp, sp+1, A ) )
					*((dp[A]) + 1) = 0;
				else
					*((dp[A]) + 1) += (u_int)cerror_7_16;
			if ( i < row - 1 )
			{
				sp1 = sp + col;
				dp1 = (dp[A]) + col;
				if ( pixel_not_equal( sp, sp1, A ) )
					*dp1 = 0;
				else
					*dp1 += (u_int)cerror_5_16;

				if ( j < col - 1 )
					if ( pixel_not_equal( sp, sp1+1, A ) )
						*(dp1 + 1) = 0;
					else
						*(dp1 + 1) += (u_int)cerror_1_16;

				if ( j > 0 )
					if ( pixel_not_equal( sp, sp1-1, A ) )
						*(dp1 - 1) = 0;
					else
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


