/**********************
 *** ImageDifference.c   ***
 **********************/

#include	"ImageType/ImageType.h"


void
image_difference_rectangle( image_type *im0, image_type *im1, int *i0, int *i1, int *j0, int *j1 )
{
u_int	*sp0,	*sp1;
int *row_arr,	*col_arr;
int	i,	j;

	if( im0->depth == 1 ){
		image1_difference_rectangle( im0, im1, i0, i1, j0, j1 );
		return;
	}

	row_arr = (int *)malloc((im0->row)*sizeof(int));
	col_arr = (int *)malloc((im0->column)*sizeof(int));

	for ( i = 0 ; i < im0->row ; i++ )
		row_arr[i] = 0;

	for ( i = 0 ; i < im0->column ; i++ )
		col_arr[i] = 0;

	sp0 = (u_int *)im0->data;
	sp1 = (u_int *)im1->data;

	for ( i = 0 ; i < im0->row ; i++ ) {
		for ( j = 0 ; j < im0->column ; j++ ) {
			if ( *sp0++ != *sp1++ ) {
				row_arr[i] = 1;
				col_arr[j] = 1;	
			}
		}
	}

	*i0 = *j0 = *i1 = *j1 = 0;

	for ( i = 0 ; i < im0->row ; i++ )
		if ( row_arr[i] == 1 ) {
			*i0 = i;
			break;
		}
	for ( i = im0->row - 1 ; i >= 0  ; i-- )
		if ( row_arr[i] == 1 ) {
			*i1 = i;
			break;
		}
	for ( j = 0 ; j < im0->column ; j++ )
		if ( col_arr[j] == 1 ) {
			*j0 = j;
			break;
		}
	for ( j = im0->column - 1 ; j >= 0 ; j-- )
		if ( col_arr[j] == 1 ) {
			*j1 = j;
			break;
		}

	free( row_arr );
	free( col_arr );
}


void
image1_difference_rectangle( image_type *im0, image_type *im1, int *i0, int *i1, int *j0, int *j1 )
{
u_char	*sp0,	*sp1;
int *row_arr,	*col_arr;
int	i,	j;



	row_arr = (int *)malloc((im0->row)*sizeof(int));
	col_arr = (int *)malloc((im0->column)*sizeof(int));

	for ( i = 0 ; i < im0->row ; i++ )
		row_arr[i] = 0;

	for ( i = 0 ; i < im0->column ; i++ )
		col_arr[i] = 0;

	sp0 = im0->data;
	sp1 = im1->data;

	for ( i = 0 ; i < im0->row ; i++ ) {
		for ( j = 0 ; j < im0->column ; j++ ) {
			if ( *sp0++ != *sp1++ ) {
				row_arr[i] = 1;
				col_arr[j] = 1;
			}
		}
	}

	for ( i = 0 ; i < im0->row ; i++ )
		if ( row_arr[i] == 1 ) {
			*i0 = i;
			break;
		}
	for ( i = im0->row - 1 ; i >= 0  ; i-- )
		if ( row_arr[i] == 1 ) {
			*i1 = i;
			break;
		}
	for ( j = 0 ; j < im0->column ; j++ )
		if ( col_arr[j] == 1 ) {
			*j0 = j;
			break;
		}
	for ( j = im0->column - 1 ; j >= 0 ; j-- )
		if ( col_arr[j] == 1 ) {
			*j1 = j;
			break;
		}

	free( row_arr );
	free( col_arr );
}




static void	image1_color_rectangle( image_type *im, int color,
								   int *i0, int *i1, int *j0, int *j1 );


void
image_color_rectangle( image_type *im, int color, int *i0, int *i1, int *j0, int *j1 )
{
u_int	*sp;
int *row_arr,	*col_arr;
int	i,	j;

	if( im->depth == 1 ){
		image1_color_rectangle( im, color, i0, i1, j0, j1 );
		return;
	}

	row_arr = (int *)malloc((im->row)*sizeof(int));
	col_arr = (int *)malloc((im->column)*sizeof(int));

	for ( i = 0 ; i < im->row ; i++ )
		row_arr[i] = 0;

	for ( i = 0 ; i < im->column ; i++ )
		col_arr[i] = 0;

	sp = (u_int *)im->data;

	for ( i = 0 ; i < im->row ; i++ ) {
		for ( j = 0 ; j < im->column ; j++ ) {
			if ( *sp++ != (u_int)color ) {
				row_arr[i] = 1;
				col_arr[j] = 1;	
			}
		}
	}

	*i0 = *j0 = *i1 = *j1 = 0;

	for ( i = 0 ; i < im->row ; i++ )
		if ( row_arr[i] == 1 ) {
			*i0 = i;
			break;
		}
	for ( i = im->row - 1 ; i >= 0  ; i-- )
		if ( row_arr[i] == 1 ) {
			*i1 = i;
			break;
		}
	for ( j = 0 ; j < im->column ; j++ )
		if ( col_arr[j] == 1 ) {
			*j0 = j;
			break;
		}
	for ( j = im->column - 1 ; j >= 0 ; j-- )
		if ( col_arr[j] == 1 ) {
			*j1 = j;
			break;
		}

	free( row_arr );
	free( col_arr );
}


static void
image1_color_rectangle( image_type *im, int color, int *i0, int *i1, int *j0, int *j1 )
{
u_char	*sp;
int *row_arr,	*col_arr;
int	i,	j;

	row_arr = (int *)malloc((im->row)*sizeof(int));
	col_arr = (int *)malloc((im->column)*sizeof(int));

	for ( i = 0 ; i < im->row ; i++ )
		row_arr[i] = 0;

	for ( i = 0 ; i < im->column ; i++ )
		col_arr[i] = 0;

	sp = (u_char *)im->data;

	for ( i = 0 ; i < im->row ; i++ ) {
		for ( j = 0 ; j < im->column ; j++ ) {
			if ( *sp++ != (u_char)color ) {
				row_arr[i] = 1;
				col_arr[j] = 1;	
			}
		}
	}

	*i0 = *j0 = *i1 = *j1 = 0;

	for ( i = 0 ; i < im->row ; i++ )
		if ( row_arr[i] == 1 ) {
			*i0 = i;
			break;
		}
	for ( i = im->row - 1 ; i >= 0  ; i-- )
		if ( row_arr[i] == 1 ) {
			*i1 = i;
			break;
		}
	for ( j = 0 ; j < im->column ; j++ )
		if ( col_arr[j] == 1 ) {
			*j0 = j;
			break;
		}
	for ( j = im->column - 1 ; j >= 0 ; j-- )
		if ( col_arr[j] == 1 ) {
			*j1 = j;
			break;
		}

	free( row_arr );
	free( col_arr );
}
