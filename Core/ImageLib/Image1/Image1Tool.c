 /**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"

void
image1_const( image_type *im, int color )
{
u_char	*p;
int	i,	j;

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ )
			*p++ = color;
}



float 
image1_accumulate( image_type *sim1, image_type *sim0,
						int x0, int y0, int width, int height )
{
u_char	*sp0,	*sp1;
int	row,	col,	i,	j;
float	sum;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	sum = 0;
	for( i = 0 ; i < height ; i++ ){
		sp0 = IMAGE_PIXEL( sim0, i+y0, x0 );
		sp1 = IMAGE_PIXEL( sim1, i+y0, x0 );
		for( j = 0 ; j < width ; j++ ){

			sum += (*sp1) * (*sp0);

			sp0++;
			sp1++;
		}
	}

	return( sum/( width * height) );
}




void 
image1_color( image_type *im, int color,
						int x0, int y0, int width, int height )
{
u_char	*sp;
int	i,	j;


	if( y0 + height > im->row   )	height = im->row - y0; 
	if( x0 + width > im->column )	width = im->column - x0; 



	for( i = 0 ; i < height ; i++ ){

		sp = IMAGE_PIXEL( im, i+y0, x0 );

		for( j = 0 ; j < width ; j++ )
			*sp++ = color;

	}
}


image_type *
image1_to_image4( image_type *sim, image_type *im )
{
u_char	*sp;
u_int	*p;
int	row,	col,	i,	j;
int	y;

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	if( im == NULL )
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



void
image1_color_map( image_type *im, u_char map[] )
{
u_char	*p;
int	i,	j;

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, p++ ){
			*p = map[*p];
		}
}





void
image1_minmax( image_type *im, int *min, int *max )
{
u_char	*p;
int	i,	j;

	*min = *max = 0;

	p = (u_char *)im->data;

	*min = *max = *p;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, p++ ){
			if( *p < *min )	*min = *p;
			else	if( *p > *max )	*max = *p;
		}
}

void
image1_average( image_type *im, int x0, int y0, int width, int height, float *av )
{
u_char	*sp;
float	sum;
int	i,	j,	n;


	sum = 0;

	for( i = 0 ; i < height ; i++ ){
		sp = IMAGE_PIXEL( im, i+y0, x0 );
		for( j = 0 ; j < width ; j++, sp++ ){
			sum += *sp;
		}
	}


	n = height * width;
	*av = sum / n;
}

void
image1_var( image_type *im, int x0, int y0, int width, int height, float *av, float *var )
{
u_char	*sp;
float	sum,	sum2;
int	i,	j,	n;

	

	sum = sum2 = 0;

	for( i = 0 ; i < height ; i++ ){
		sp = IMAGE_PIXEL( im, i+y0, x0 );
		for( j = 0 ; j < width ; j++, sp++ ){
			sum += *sp;

			sum2 += *sp * *sp;
		}
	}


	n = height * width;
	*av = sum / n;
	*var = sum2/ n - (*av) * (*av);
}



void
image1_var0( image_type *im, int x0, int y0, int width, int height, float av, float *var )
{
u_char	*sp;
float	sum,	tmp;
int	i,	j,	n;


	sum = 0;

	for( i = 0 ; i < height ; i++ ){
		sp = IMAGE_PIXEL( im, i+y0, x0 );
		for( j = 0 ; j < width ; j++, sp++ ){
			tmp = (int)*sp - av;
			if( tmp < 0 )	tmp = -tmp;
			sum += tmp;
		}
	}


	n = height * width;
	*var = sum / n;
}




image_type *
image1_negative( image_type *sim, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;
	

	if( im == NULL )
		im = image_create( sim->height, im->width, 1, 1, NULL );


	sp = (u_char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){

			*tp = 255 - *sp;
		}
	}

	return( im );
}



image_type *
image1_add_scalar( image_type *sim, int a, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j,	tmp;


	if( im == NULL )
		im = image_create( sim->height, im->width, 1, 1, NULL );


	sp = (u_char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = *sp + a;

			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}


image_type *
image1_stretch( image_type *sim, image_type *im )
{
int	m0,	m1;

	image1_minmax( sim, &m0, &m1 );

	im = image1_contrast( sim, (float)255/m1, 0, im );

	return( im );
}


image_type *
image1_axb( image_type *sim, float a, float b, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j,	tmp;


	im = image_reallocL( sim, im );
	//	if( im == NULL )
	//		im = image_create( sim->height, im->width, 1, 1, NULL );


	sp = (u_char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a * *sp + b;

			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}



image_type *
image1S_axb( image_type *sim, float a, float b, image_type *im )
{
	char	*sp;
	u_char	*tp;
	int	i,	j,	tmp;


	im = image_reallocL( sim, im );


	sp = (char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a * *sp + b;

			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}


image_type *
image1S_abs_128( image_type *sim, image_type *im )
{
	char	*sp;
	u_char	*tp;
	int	i,	j,	tmp;


	im = image_reallocL( sim, im );


	sp = (char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = ABS(*sp);

			

			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}



image_type *
image1_contrast( image_type *sim, float a, float b, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j,	tmp;


	if( im == NULL )
		im = image_create( sim->height, sim->width, 1, 1, NULL );


	sp = (u_char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a* ( (*sp) - b ) + b;

			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}

	return( im );
}


image_type *
image1_color_table_transform( image_type *sim, u_char table[], image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;



	im = image_recreate( im, sim->height, sim->width, 1, 1 );


	sp = (u_char *)sim->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			*tp = table[*sp];
		}
	}

	return( im );
}



void
image1_max( image_type *sim, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;


	sp = sim->data;
	tp = im->data;
	for( i = 0 ; i < sim->row ; i++){
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ ){

			if( *sp < *tp )	*sp = *tp;
		}
	}

}


image_type *
image1_abs_diff( image_type *im0, image_type *im1, image_type *im )
{
	u_char	*sp0,	*sp1,	*tp;
	int	i,	j;
	int	tmp;

	im = image_recreate( im, im0->height, im0->width, 1, 1 );

	sp0 = im0->data;
	sp1 = im1->data;

	tp = im->data;


	for( i = 0; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp0++, sp1++, tp++ ){
			tmp = (int)*sp1 - (int)*sp0;
			if( tmp < 0 )	tmp = -tmp;

			*tp = tmp;
		}
	}



	return( im );
}


image_type *
image1_local_max( image_type *sim, image_type *im )
{
	u_char	*sp,	*sp0,	*sp1;
	u_char	*tp;
	int	i,	j,	max;



	if( im == NULL ){
		im = image_create( sim->row, sim->column, 1, 1, NULL );
		image1_const( im, 0 );
	}


	sp = (u_char *)sim->data;
	tp = (u_char *)im->data;


	sp += sim->column+1;
	tp += sim->column+1;

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 2 ){

		sp0 = sp - sim->column;
		sp1 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp1++ ){

			max = *(sp0-1);
			if( max < *(sp0) )		max = *sp0;
			if( max < *(sp0+1) )	max = *(sp0+1);


			if( max < *(sp-1) )	max = *(sp-1);
			if( max < *(sp+1) )	max = *(sp+1);


			if( max < *(sp1-1) )	max = *(sp1-1);
			if( max < *(sp1) )		max = *sp1;
			if( max < *(sp1+1) )	max = *(sp1+1);



			*tp++ = ( *sp > max - 10 )? *sp : 0;
		}
	}

	return( im );
}


image_type *
	image1_sub( image_type *im0, image_type *im1, image_type *im )
{
	u_char	*sp0,	*sp1,	*tp;
	int	i,	j;
	int	tmp;

	im = image_recreate( im, im0->height, im0->width, 1, 1 );

	sp0 = im0->data;
	sp1 = im1->data;

	tp = im->data;


	for( i = 0; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp0++, sp1++, tp++ ){
			tmp = (int)*sp1 - (int)*sp0;

			*tp = PUSH_TO_RANGE( tmp, 0, 255 );
		}
	}



	return( im );
}