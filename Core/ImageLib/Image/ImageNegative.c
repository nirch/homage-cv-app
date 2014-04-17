#include	<stdio.h>


#include	"ImageType/ImageType.h"

static void		image4_negative( image_type *im );

static void		image1_negativeO( image_type *im );



void 
image_negative( image_type *im )
{
	if ( im->depth == 4 )
		image4_negative( im );
	else
		image1_negativeO( im );
}


static void 
image4_negative( image_type *im )
{
u_int	*dp;
int		row, col, i, j;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	dp = (u_int *)im->data;
	for( i = 0; i < row; i++ )
	for( j = 0; j < col; j++ )
		*dp++ =  0x00FFFFFF - (*dp);
}


static void 
image1_negativeO( image_type *im )
{
u_char	*dp;
int		row, col, i, j;

	row = IMAGE_ROW(im); 
	col = IMAGE_COLUMN(im); 

	dp = (u_char *)im->data;
	for( i = 0; i < row; i++ )
	for( j = 0; j < col; j++ )
		*dp++ = 0xFF - (*dp);
}


