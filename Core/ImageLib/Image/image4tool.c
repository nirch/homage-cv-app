/**************************
 ***	Image4Tool.c	***
 *************************/

#include "ImageType/ImageType.h"




image_type *
image4_copy_dup( image_type *sim, int x0, int y0, int dw, int dh, int width, int height, image_type *im )
{
u_int	*sp,	*p,	*sp0;
int	i,	j,	i1,	j1,	y,	x;

	if( im == NULL )
		im = image_create( height, width, 4, 1, NULL );


	p = (u_int *)im->data;
	for( i = 0, y = y0 ; i < im->height  ; y++ ){

		if( y < 0 || y >= sim->height ){
			for( j = 0 ; j < im->width ; j++ )
				*p++ = 0XFFFFFF;

			i += dh;
			continue;
		}


		sp0 = (u_int *)IMAGE_PIXEL(sim, y, x0);


		for( i1 = 0 ; i1 < dh && i < im->height ; i1++, i++ ){
			sp = sp0;
			for( j = 0, x = x0 ; j < im->width ; x++, sp++ ){
				int color = ( x >= 0 && x < sim->width )? *sp : 0xFFFFFF;

				for( j1 = 0 ; j1 < dw && j < im->width ; j1++, j++ )
					*p++ = color;//*sp;

			}
		}
	}

	return( im );
}




void
image4_mark( image_type *im, int row0, int col0, int row, int col, int color )
{
int	i,	j;

	for( i = 0 ; i < row ; i++ ){
		*IMAGE4_PIXEL( im, row0 + i, col0 ) = color;
		*IMAGE4_PIXEL( im, row0 + i, col0+col ) = color;
	}


	for( j = 0 ; j < col ; j++ ){
		*IMAGE4_PIXEL( im, row0, col0 + j ) = color;
		*IMAGE4_PIXEL( im, row0 + row, col0 + j ) = color;
	}
}



void
image4_const( image_type *im, int color )
{
u_int	*p;
int	i,	j;

	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
			*p++ = color;
}



void
image4_Vflip( image_type *im )
{
u_int	*p0,	*p1,	tmp;
int	i,	j,	hrow;

	hrow = im->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE4_PIXEL( im, i, 0 );
		p1 = IMAGE4_PIXEL( im, im->row-1-i, 0 );
		for( j = 0 ; j < im->column ; j++, p0++, p1++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}


void
image4_Hflip( image_type *im)
{
u_int	*p0,	*p1,	tmp;
int	i,	j,	hcol;


	hcol = im->column / 2;
	for( i = 0 ; i < im->row  ; i++ ){

		p0 = IMAGE4_PIXEL( im, i, 0 );
		p1 = IMAGE4_PIXEL( im, i, im->column-1 );
		for( j = 0 ; j < hcol ; j++, p0++, p1-- ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}


image_type *
image4_Vflip2( image_type *sim, image_type *tim)
{
u_int	*p0,	*p1,	tmp;
int	i,	j,	hrow;

	if ( tim == NULL )
		tim = image_create( sim->row, sim->column, 4, 1, NULL );
	
	p0 = (u_int *)sim->data;
	p1 = (u_int *)tim->data;

	for ( i = 0 ; i < sim->row ; i++ )
		for ( j = 0 ; j < sim->column ; j++)
			*p1++ = *p0++;

	hrow = tim->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE4_PIXEL( tim, i, 0 );
		p1 = IMAGE4_PIXEL( tim, tim->row-1-i, 0 );
		for( j = 0 ; j < tim->column ; j++, p0++, p1++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
	return ( tim );
}


image_type *
image4_Hflip2( image_type *sim, image_type *tim)
{
u_int	*p0,	*p1,	tmp;
int	i,	j,	hcol;

	if ( tim == NULL )
		tim = image_create( sim->row, sim->column, 4, 1, NULL );
	
	p0 = (u_int *)sim->data;
	p1 = (u_int *)tim->data;
	
	for ( i = 0 ; i < sim->row ; i++ )
		for ( j = 0 ; j < sim->column ; j++)
			*p1++ = *p0++;

	hcol = tim->column / 2;
	for( i = 0 ; i < tim->row  ; i++ ){

		p0 = IMAGE4_PIXEL( tim, i, 0 );
		p1 = IMAGE4_PIXEL( tim, i, tim->column-1 );
		for( j = 0 ; j < hcol ; j++, p0++, p1-- ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
	return ( tim );
}


image_type *
image4_rotate90( image_type *sim, image_type *tim )
{
u_int	*sp,	*tp;
int	i,	j;

	if ( tim == NULL )
		tim = image_create( sim->column, sim->row, 4, 1, NULL );
	
	sp = (u_int *)sim->data;
	for ( i = 0 ; i < sim->row ; i++ ){
		tp = IMAGE4_PIXEL( tim, tim->row-1, i );

		for ( j = 0 ; j < sim->column ; j++){
			*tp = *sp++;
			tp -= IMAGE_COLUMN( tim );
		}
	}

	return( tim );

}


image_type *
image4_rotate180( image_type *sim, image_type *tim)
{
u_int	*p0,	*p1,	tmp;
int	i,	j;

	if ( tim == NULL )
		tim = image_create( sim->row, sim->column, 4, 1, NULL );
	
	p0 = (u_int *)sim->data;
	p1 = (u_int *)tim->data;

	for( i = 0 ; i < tim->row; i++ ){

		p1 = IMAGE4_PIXEL( tim, tim->row-i-1, tim->column-1 );
		for( j = 0 ; j < tim->column ; j++, p0++, p1-- ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
	return ( tim );
}


image_type *
image4_rotate270( image_type *sim, image_type *tim )
{
u_int	*sp,	*tp;
int	i,	j;

	if ( tim == NULL )
		tim = image_create( sim->column, sim->row, 4, 1, NULL );
	
	sp = (u_int *)sim->data;
	for ( i = 0 ; i < sim->row ; i++ ){
		//tp = IMAGE4_PIXEL( tim, 0, i );
		tp = IMAGE4_PIXEL( tim, 0, sim->row-i-1 );

		for ( j = 0 ; j < sim->column ; j++){
			*tp = *sp++;
			tp += IMAGE_COLUMN( tim );
		}
	}

	return( tim );

}


//void
//image4_copy_in( image_type *sim, image_type *im, int row0, int col0 )
//{
//u_int	*p,	*sp;
//int	i,	j,	algin;
//
//	
//	algin = im->column - sim->column;
//	sp = (u_int *)sim->data;
//	p = IMAGE4_PIXEL( im, row0, col0 );
//
//	for( i = 0 ; i < sim->row ; i++, p += algin ){
//		for( j = 0 ; j < sim->column ; j++ )
//			*p++ = *sp++;
//	}
//}

image_type * 
image4_copy( image_type *sim )
{
image_type	*im;
u_int	*p,	*sp;
int	i,	j;

	im = image_create( sim->row, sim->column, 4, 1, NULL );
	sp = (u_int *)sim->data;
	p = (u_int *)im->data;

	for( i = 0 ; i < sim->row ; i++){
		for( j = 0 ; j < sim->column ; j++ )
			*p++ = *sp++;
	}

	return ( im );
}



image_type *
image4_to_red( image_type *sim )
{
u_int	*sp;
u_char	*p;
image_type	*im;
int	row,	col,	i,	j;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );

	sp = (u_int *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*p++ = IMAGE4_RED(*sp);
			sp++;
		}
	}

	return( im );
}

image_type *
image4_to_green( image_type *sim )
{
u_int	*sp;
u_char	*p;
image_type	*im;
int	row,	col,	i,	j;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );

	sp = (u_int *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*p++ = IMAGE4_GREEN(*sp);
			sp++;
		}
	}

	return( im );
}

image_type *
image4_to_blue( image_type *sim )
{
u_int	*sp;
u_char	*p;
image_type	*im;
int	row,	col,	i,	j;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 1, 1, NULL );

	sp = (u_int *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*p++ = IMAGE4_BLUE(*sp);
			sp++;
		}
	}

	return( im );
}


//image_type *
//image4_to_y( image_type *sim )
//{
//u_int	*sp;
//u_char	*p;
//image_type	*im;
//int	row,	col,	i,	j;
//int	R,	G,	B,	Y;
//
//	row = IMAGE_ROW(sim);
//	col = IMAGE_COLUMN(sim);
//
//	im = image_create( row, col, 1, 1, NULL );
//
//	sp = (u_int *)sim->data;
//	p = (u_char *)im->data;
//	for( i = 0 ; i < im->row ; i++ ){
//		for( j = 0 ; j < im->column ; j++ ){
//			R = IMAGE4_RED(*sp);
//			G = IMAGE4_GREEN(*sp);
//			B = IMAGE4_BLUE(*sp);
//			sp++;
//
//			Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
//
//			*p++ = PUSH_TO_RANGE( Y, 0, 255 );
//
//		}
//	}
//
//	return( im );
//}


image_type *
image4_to_y( image_type *sim, image_type *im )
{
u_int	*sp;
u_char	*p;
int	row,	col,	i,	j;
int	R,	G,	B,	Y;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);


	im = image_recreate( im, row, col, 1, 1 );

	sp = (u_int *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = IMAGE4_RED(*sp);
			G = IMAGE4_GREEN(*sp);
			B = IMAGE4_BLUE(*sp);
			sp++;

			Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;

			*p++ = PUSH_TO_RANGE( Y, 0, 255 );

		}
	}

	return( im );
}


image_type *
image4_from_y( image_type *sim )
{
u_char	*sp;
u_int	*p;
image_type	*im;
int	row,	col,	i,	j;
int	y;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 4, 1, NULL );

	sp = (u_char *)sim->data;
	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			y = *sp++;
			*p++ = IMAGE4_RGB( y, y, y );
		}
	}

	return( im );
}



image_type *
image4_from_rgb( image_type *imR, image_type *imG, image_type *imB, image_type *im )
{
u_char	*rp,	*gp,	*bp;
u_int	*tp;
int	i,	j;
int	R,	G,	B;

	im = image_recreate( im, imR->height, imR->width, 4, 1 );

	rp = (u_char *)imR->data;
	gp = (u_char *)imG->data;
	bp = (u_char *)imB->data;


	tp = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			R = *rp++;
			G = *gp++;
			B = *bp++;
			*tp++ = IMAGE4_RGB( R, G, B );
		}
	}

	return( im );
}