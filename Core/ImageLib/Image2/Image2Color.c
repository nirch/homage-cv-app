 /**************************
 ***	ImageColor.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"


void
image2_average( image_type *im, int x0, int y0, int width, int height, float *av )
{
short	*sp;
float	sum;
int	i,	j,	n;


	sum = 0;

	for( i = 0 ; i < height ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i+y0, x0 );
		for( j = 0 ; j < width ; j++, sp++ ){
			sum += *sp;
		}
	}


	n = height * width;
	*av = sum / ( n*16);
}




image_type *
image2_negative( image_type *sim, image_type *im )
{
short	*sp,	*tp;
int	i,	j;
	

	im = image_recreate( im, sim->height, sim->width, 2, 1 );


	sp = sim->data_s;
	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){

			*tp = 255*16 - *sp;
		}
	}

	return( im );
}



image_type *
image2_add_scalar( image_type *sim, int a, image_type *im )
{
short	*sp,	*tp;
int	i,	j,	tmp;


	
	im = image_recreate( im, sim->height, sim->width, 2, 1 );


	sp = sim->data_s;
	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = *sp + a;

			*tp = PUSH_TO_RANGE( tmp, 0, 4096 );
		}
	}

	return( im );
}


image_type *
image2_contrast( image_type *sim, float a, float b, image_type *im )
{
short	*sp,	*tp;
int	i,	j,	tmp;



	im = image_recreate( im, sim->height, sim->width, 2, 1 );


	sp = sim->data_s;
	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a* ( (*sp) - b ) + b;

			*tp = PUSH_TO_RANGE( tmp, 0, 4096 );
		}
	}

	return( im );
}


image_type *
image2_enhance( image_type *sim, float a, float b, float m, image_type *im )
{
	short	*sp,	*tp;
	int	i,	j,	tmp;



	im = image_recreate( im, sim->height, sim->width, 2, 1 );


	sp = sim->data_s;
	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){
			tmp = a* ( (*sp) - m ) + b;

			*tp = PUSH_TO_RANGE( tmp, 0, 4096 );
		}
	}

	return( im );
}
void
image2_var0( image_type *im, int x0, int y0, int width, int height, float av, float *var )
{
short	*sp;
float	sum,	tmp;
int	i,	j,	n;


	sum = 0;

	av *= 16;

	for( i = 0 ; i < height ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i+y0, x0 );
		for( j = 0 ; j < width ; j++, sp++ ){
			tmp = (int)*sp - av;
			if( tmp < 0 )	tmp = -tmp;
			sum += tmp;
		}
	}


	n = height * width;
	*var = sum / (16*n);
}



image_type *
image2_add( image_type *sim, image_type *im )
{
short	*sp,	*tp;
int	i,	j;


	if( im == NULL ){
		im = image_create( sim->row, sim->column, 2 , 1, NULL );
		image2_const( im, 0 );
	}


	sp = sim->data_s;
	tp = im->data_s;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ += *sp++;
		}
	}

	return( im );
}

image_type *    image2_interpolate( image_type *im0, image_type *im1, image_type *wim, image_type *im)
{
	short *sp0, *sp1, *tp;
	float *wp;
	int	i, j;


	im = image_recreate( im, im0->row, im1->column, 2 , 1 );

	sp0 = im0->data_s;
	sp1 = im1->data_s;
	wp = wim->data_f;
	tp = im->data_s;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp0++, sp1++, wp++, tp++ ){

			*tp = ((*sp0)*(*wp) + (*sp1)*(255.0-(*wp)))/255.0;
		}
	}

	return( im );
}
