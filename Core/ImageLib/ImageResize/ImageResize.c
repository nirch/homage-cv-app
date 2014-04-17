#include	<stdio.h>


#include	"ImageType/ImageType.h"


static image_type * 
image4_resize( image_type *imd, image_type *ims, int row, int col );

static image_type * 
image1_resize( image_type *imd, image_type *ims, int row, int col );



image_type * 
image_resize( image_type *imd, image_type *ims, int row, int col )
{
	if ( imd->depth == 4 )
		return image4_resize( imd, ims, row, col );
	else
		return image1_resize( imd, ims, row, col );
}


static image_type * 
image4_resize( image_type *imd, image_type *ims, int row, int col )
{
u_int	*p, *dp;
int		row_old, col_old;
int		i, j, is, js, shift;

	row_old = IMAGE_ROW(imd); 
	col_old = IMAGE_COLUMN(imd); 

	if ( ims == NULL )
		ims = image_create( row, col, 4, 1, NULL );
	dp = (u_int *)ims->data;
	for( i = 0; i < row; i++ )
	for( j = 0; j < col; j++ )
		*dp++ = 0x00FFFFFF;

	is = (row - row_old)/2;
	js = (col - col_old)/2;
	shift = col - col_old;

	p = (u_int *)imd->data;
	dp = (u_int *)ims->data + is*col +js;

	for( i = 0; i < row_old; i++ )
	{
		for( j = 0; j < col_old; j++ )
		{
			*dp = *p;
			p++;
			dp++;
		}
		dp += shift;
	}

	return( ims );	
}


static image_type * 
image1_resize( image_type *imd, image_type *ims, int row, int col )
{
u_char	*p, *dp;
int		row_old, col_old;
int		i, j, is, js, shift;

	row_old = IMAGE_ROW(imd); 
	col_old = IMAGE_COLUMN(imd); 

	if ( ims == NULL )
		ims = image_create( row, col, 1, 1, NULL );

	dp = (u_char *)ims->data;
	for( i = 0; i < row; i++ )
	for( j = 0; j < col; j++ )
		*dp++ = 0xFF;

	is = (row - row_old)/2;
	js = (col - col_old)/2;
	shift = col - col_old;

	p = (u_char *)imd->data;
	dp = (u_char *)ims->data + is*col +js;

	for( i = 0; i < row_old; i++ )
	{
		for( j = 0; j < col_old; j++ )
		{
			*dp = *p;
			p++;
			dp++;
		}
		dp += shift;
	}

	return( ims );	
}
