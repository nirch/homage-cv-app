/*************************
 ***	ImageExtend.c   ***
 *************************/

#include "ImageType/ImageType.h"


static image_type *	image1_extend( image_type *sim, int height, int width, int color, image_type *im );
static image_type *	image3_extend( image_type *sim, int height, int width, int color, image_type *im );
static image_type *	image4_extend( image_type *sim, int height, int width, int color, image_type *im );



//static void	image4_copy_inN( image_type *sim, image_type *im, int row0, int col0 );


/*
Image_extend: add to the image a colored framework
parameters:
row - new heigth of image
column - new width of image
*/
//
//image_type *
//image_extend( image_type *sim, int row, int column, int color, image_type *rim )
//{
//int	y0,	x0;
//
//	rim = image_recreate( rim, row, column, 4, 1 );
//
//	image4_const( rim, color );
//
//	y0 = ( row - sim->row )/2;
//	x0 = ( column - sim->column )/2;
//
//
//	image4_copy_in( sim, rim, y0, x0 );
//
//	return ( rim );
//}


image_type *
image_extend( image_type *sim, int height, int width, int color, image_type *im )
{

	if( sim->depth == 1){
		im = image1_extend( sim, height, width, color, im );
		return( im );
	}

	if( sim->depth == 3){
		im = image3_extend( sim, height, width, color, im );
		return( im );
	}

	if( sim->depth == 4){
		im = image4_extend( sim, height, width, color, im );
		return( im );
	}

	//if( sim->depth == 6){
	//	im = imageS3_crop( sim, x0, y0, width, height, im );
	//	return( im );
	//}

	return( NULL );
} 

static image_type *
image1_extend( image_type *sim, int height, int width, int color, image_type *im )
{
	int	y0,	x0;
	u_char	*tp,	*sp;
	int	i,	j,	algin;


	im = image_recreate( im, height, width, 1, 1 );

	image1_const( im, color );

	y0 = ( height - sim->height )/2;
	x0 = ( width - sim->width )/2;



	algin = im->width - sim->width;
	sp = sim->data;

	tp = IMAGE_PIXEL( im, y0, x0 );
	for( i = 0 ; i < sim->row ; i++, tp += algin ){
		for( j = 0 ; j < sim->column ; j++ ){
			*tp++ = *sp++;
		}
	}

	return ( im );
}

static image_type *
image3_extend( image_type *sim, int height, int width, int color, image_type *im )
{
	int	y0,	x0;
	u_char	*tp,	*sp;
	int	i,	j,	algin;


	im = image_recreate( im, height, width, 3, 1 );

	image3_const( im, IMAGE4_RED(color),IMAGE4_GREEN(color), IMAGE4_BLUE(color) );


	y0 = ( height - sim->height )/2;
	x0 = ( width - sim->width )/2;



	algin = 3*(im->width - sim->width);
	sp = sim->data;

	tp = IMAGE_PIXEL( im, y0, x0 );
	for( i = 0 ; i < sim->row ; i++, tp += algin ){
		for( j = 0 ; j < sim->column ; j++ ){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;
		}
	}

	return ( im );
}



image_type *
image4_extend( image_type *sim, int height, int width, int color, image_type *im )
{
int	y0,	x0;
u_int	*tp,	*sp;
int	i,	j,	algin;


	im = image_recreate( im, height, width, 4, 1 );

	image4_const( im, color );

	y0 = ( height - sim->height )/2;
	x0 = ( width - sim->width )/2;




	algin = im->column - sim->column;
	sp = (u_int *)sim->data;

	tp = IMAGE4_PIXEL( im, y0, x0 );
	for( i = 0 ; i < sim->row ; i++, tp += algin ){
		for( j = 0 ; j < sim->column ; j++ )
			*tp++ = *sp++;
	}

	return ( im );
}





//static void
//image4_copy_inN( image_type *sim, image_type *im, int y0, int x0 )
//{
//u_int	*p,	*sp;
//int	i,	j;
//int	row,	column;
//int	i0,	j0;
//
//	row = sim->row;
//	if( y0 + row > im->row )	row = im->row - y0;
//
//	column = sim->column;
//	if( x0 + column > im->column )	column = im->column - x0;
//
//
//
//	i0 = 0;
//	if( y0 < 0 )	i0 = -y0;
//	j0 = 0;
//	if( x0 < 0 )	j0 = -x0;
//
//	for( i = i0 ; i < row ; i++ ){
//		p = IMAGE4_PIXEL( im, i+y0, x0+j0 );
//		sp = IMAGE4_PIXEL( sim, i, j0 );
//		for( j = j0 ; j < column ; j++ )
//			*p++ = *sp++;
//	}
//}


/*
Image_border_rect: add to the image a colored framework, within its borders
parameters:
cycle: the depth of the framework of the image
r0, c0 - the top left point of the framework
row0, col0 - width and height of framework
*/
image_type *    
image_border_rect(image_type * im, int color, int cycle,
				  int r0, int c0, int row0, int col0)
{
int	col = IMAGE_COLUMN(im);
unsigned int *pixel;
int	i,	j;

	if(im == NULL)
		return NULL;
	

	for( i = 1; i <= cycle; i++ )
	{
		// rows
		pixel = IMAGE4_PIXEL( im, r0-i, c0-i);
		for (j = c0-i; j <= c0+col0+i; j++)
			*pixel++ = color;
		pixel = IMAGE4_PIXEL( im, r0+row0+i, c0-i);
		for (j = c0-i; j <= c0+col0+i; j++)
			*pixel++ = color;

		// columns
		pixel = IMAGE4_PIXEL( im, r0-i+1, c0-i );
		for (j = r0-i+1; j < r0+row0+i; j++){
			*pixel = color;
			pixel += col;
		}
		pixel = IMAGE4_PIXEL( im, r0-i+1, c0+col0+i );
		for (j = r0-i+1; j < r0+row0+i; j++){
			*pixel = color;
			pixel += col;
		}
	}


	return( im );
}

