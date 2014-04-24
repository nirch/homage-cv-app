/************************
 ***   ImageCTool.c   ***
 ************************/
#include	<math.h>
#include	"ImageType/ImageType.h"


image_type *
imageC_from_image6( image_type *im, int d, image_type *imc )
{
short	*sp;
float	*tp;
int	i,	j;


	imc = image_realloc( im, im->width, im->height, 1, IMAGE_TYPE_C, 1 );


	sp = (short *)im->data;
	sp += d;

	tp = (float *)imc->data;

	for( i = 0; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			*tp++ = *sp / 16.0;
			*tp++ = 0;
		}

	return( imc );
}


image_type *
imageC_from_image1( image_type *sim, int d, image_type *imc )
{
	u_char	*sp;
	float	*tp;
	int	i,	j;


	imc = image_realloc( imc, sim->width, sim->height, 1, IMAGE_TYPE_C, 1 );


	sp = sim->data;
	sp += d;

	tp = (float *)imc->data;

	for( i = 0; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++, sp += sim->channel ){
			*tp++ = *sp;
			*tp++ = 0;
		}

	return( imc );
}




image_type *
imageC_mult( image_type *im0, image_type *im1, image_type *im )
{
float	*tp,	*sp0,	*sp1;
int	i,	no;
float	a,	b,	c,	d;


//	if( im == NULL )
//		im = image_create( im0->row, im0->column, 8, 1, NULL );
	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_C, 1 );

	tp = (float *)im->data;




	sp0 = (float *)im0->data;


	sp1 = (float *)im1->data;



//	for( i = 0 ; i < im->row ; i++ )
//		for( j = 0 ; j < im->column ; j++ ){
	no = im->row * im->column;
	for( i = 0 ; i < no ; i++ ){
			a = *sp0++;
			b = *sp0++;
			c = *sp1++;
			d = *sp1++;

			*tp++ = (a*c - b*d);///t;
			*tp++ = (a*d + b*c);///t;
		}

	return( im );
}

image_type *
imageC_mult_F( image_type *im0, image_type *im1, image_type *im )
{
float	*tp,	*sp0;
float	*sp1;
int	i,	j;
float	a,	b,	c;


//	if( im == NULL )
//		im = image_create( im0->row, im0->column, 8, 1, NULL );
	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_C, 1 );

	tp = (float *)im->data;




	sp0 = (float *)im0->data;


	sp1 = (float *)im1->data;



	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			a = *sp0++;
			b = *sp0++;

			c = *sp1++;

			*tp++ = (a*c);///t;
			*tp++ = (b*c);///t;
		}

	return( im );
}

image_type *
imageC_mult_6( image_type *im0, image_type *im1, image_type *im )
{
float	*tp,	*sp0;
short	*sp1;
int	i,	j;
float	a,	b,	c;


//	if( im == NULL )
//		im = image_create( im0->row, im0->column, 8, 1, NULL );
	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_C, 1 );

	tp = (float *)im->data;




	sp0 = (float *)im0->data;


	sp1 = (short *)im1->data;



	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			a = *sp0++;
			b = *sp0++;

			c = *sp1++ / 16.0;

			*tp++ = (a*c);///t;
			*tp++ = (b*c);///t;
		}

	return( im );
}


image_type *
imageC_abs( image_type *sim, image_type *im )
{
float	*tp,	*sp;
int	i,	j;
float	a,	b;


//	if( im == NULL )
//		im = image_create( sim->row, sim->column, 4, 1, NULL );
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	tp = (float *)im->data;




	sp = (float *)sim->data;


	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++ ){

			a = *sp++;
			b = *sp++;

			*tp++ = hypot( a, b );
		}

	return( im );
}



image_type *
imageC_r( image_type *sim, image_type *im )
{
float	*tp,	*sp;
int	i,	j;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	tp = (float *)im->data;




	sp = (float *)sim->data;


	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++ ){

			*tp++ = *sp++;
			sp++;

		}

	return( im );
}

image_type *
imageC_i( image_type *sim, image_type *im )
{
	float	*tp,	*sp;
	int	i,	j;



	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	tp = (float *)im->data;




	sp = (float *)sim->data;


	for( i = 0 ; i < sim->row ; i++ )
		for( j = 0 ; j < sim->column ; j++ ){
			sp++;
			*tp++ = *sp++;


		}

		return( im );
}
