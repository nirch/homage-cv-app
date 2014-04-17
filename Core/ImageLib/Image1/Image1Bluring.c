  /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"




image_type *
image1_bluring( image_type *sim, int nx, int ny, image_type *im )
{
image_type *tim;

	tim  = image1_bluring_x( sim, nx );

	im  = image1_bluring_y( tim, ny, im );

	image_destroy( tim, 1 );
	return( im );
}


image_type *
image1_bluring_x( image_type *sim, int N )
{
u_char	*sp, *tp, *sp2;
int	i,	j, sum, N2;
float	N2_inv;
image_type * im;

	N2 = 2*N+1;
	N2_inv = 1.0/N2;
	im = image_create( sim->row, sim->column, 1, 1, NULL );

	for( i = 0 ; i < im->row ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );
		sp = IMAGE_PIXEL( sim, i, 0 );


		for( j = 0, sum = 0; j < N; j++, sp++, tp++){
			*tp = *sp;
			sum += *sp;
		}

		for( ; j < N2 ; j++, sp++){
			sum += *sp;
		}



		*tp++ = sum *N2_inv; 


		for( sp2 = sp - N2 ; j < im->column ; j++, sp++, sp2++, tp++){
			sum += (*sp - *sp2);
			*tp = sum *N2_inv; 
		}



		// copy source
		sp = IMAGE_PIXEL( sim, i, sim->column - N );
		for( j = 0; j < N ; j++, tp++, sp++ ){
			*tp = *sp;
		}


	}
	return( im );
}



image_type *
image1_bluring_y( image_type *sim, int N, image_type *im )
{
	u_char	*sp, *tp, *sp2;
	int	i,	j, sum, N2;
	float	N2_inv;
	int	width;

	width = sim->width;

	N2 = 2*N+1;
	N2_inv = 1.0/N2;
	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	for( j = 0 ; j < im->width ; j++ ){
		tp = IMAGE_PIXEL( im, 0, j );
		sp = IMAGE_PIXEL( sim, 0, j );


		for( i = 0, sum = 0; i < N; i++, sp += width, tp += width){
			*tp = *sp;
			sum += *sp;
		}

		for( ; i < N2 ; i++, sp += width ){
			sum += *sp;
		}



		*tp = sum *N2_inv; 
		tp += width;


		for( sp2 = sp - N2*width ; i < im->height ; i++, sp += width, sp2 += width, tp += width){
			sum += (*sp - *sp2);
			*tp = sum *N2_inv; 
		}



		// copy source
		sp = IMAGE_PIXEL( sim, sim->height - N, j );
		for( i = 0; i < N ; i++, tp += width, sp += width ){
			*tp = *sp;
		}


	}
	return( im );
}






image_type *
image1_bluring_x_bw( image_type *sim, int thresh )
{
	//thresh = 5;
	u_char	*sp0, *sp1, *sp2;
	int	i,	j, sum, k, count;
	image_type * im = image_create( sim->row, sim->column, 1, 1, NULL );

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0, sum = 0, count = 0; j < (2*thresh + 1); j++){
			sp0 = IMAGE_PIXEL( sim, i, j );
			sum += *sp0;
			if((*sp0)!=0){
				count++;
			}
		}

		k = thresh;
		sp1 = IMAGE_PIXEL( im, i, k++ );
		*sp1 = sum/(2*thresh + 1); 

		for( ; j < im->column ; j++ ){
			sp0 = IMAGE_PIXEL( sim, i, j );
			sp2 = IMAGE_PIXEL( sim, i, j-(2*thresh + 1) );
			sum += *sp0 - *sp2;
			sp1 = IMAGE_PIXEL( im, i, k++ ); 
			if((*sp0)!=0){
				count++;
			}

			if((*sp2)!=0){
				count--;
			}

			if(sum!=0){
				*sp1 = sum/count;
			}else{
				*sp1 = 0;
			}
		}

		for( ; k < im->column ; k++ ){
			//sp0 = IMAGE_PIXEL( sim, i, k );
			sp1 = IMAGE_PIXEL( im, i, k );
			*sp1 = 0;
		}
		for( j = 0; j < thresh; j++){
			//sp0 = IMAGE_PIXEL( sim, i, j );
			sp1 = IMAGE_PIXEL( im, i, j );
			*sp1 = 0;
		}

	}
	return( im );
}

image_type *
image1_bluring_y_bw( image_type *sim, int thresh )
{
	u_char	*sp0, *sp1, *sp2;
	int	i,	j, sum, k, count;
	image_type * im = image_create( sim->row, sim->column, 1, 1, NULL );

	for( i = 0 ; i < im->column ; i++ ){
		for( j = 0, sum = 0, count = 0; j < (2*thresh + 1); j++){
			sp0 = IMAGE_PIXEL( sim, j, i );
			sum += *sp0;
			if((*sp0)!=0){
				count++;
			}
		}

		k = thresh;
		sp1 = IMAGE_PIXEL( im, k++ ,i);
		*sp1 = sum/(2*thresh + 1); 

		for( ; j < im->row ; j++ ){
			sp0 = IMAGE_PIXEL( sim, j, i );
			sp2 = IMAGE_PIXEL( sim, j-(2*thresh + 1), i );
			sum += *sp0 - *sp2;
			sp1 = IMAGE_PIXEL( im, k++ ,i);
			if((*sp0)!=0){
				count++;
			}

			if((*sp2)!=0){
				count--;
			}

			if(sum!=0){
				*sp1 = sum/count; 
			}else{
				*sp1 = 0;
			}
		}

		for( ; k < im->row ; k++ ){
			//sp0 = IMAGE_PIXEL( sim, k, i );
			sp1 = IMAGE_PIXEL( im, k, i );
			*sp1 = 0;
		}

		for( j = 0; j < thresh; j++){
			//sp0 = IMAGE_PIXEL( sim, j, i );
			sp1 = IMAGE_PIXEL( im, j, i );
			*sp1 = 0;
		}

	}
	return( im );
}


image_type *
image1_blure14( image_type *sim, image_type *im )
{
	u_char	*tp,	*sp,	*sp0,	*sp1;
	int	i,	j,	sum;

	im = image_reallocL( sim, im );


	tp = im->data;
	for( j = 0 ; j < im->width ; j++ )
		*tp++ = 0;

	for( i = 1 ; i < im->height-1 ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 1 );
		sp0 = sp - im->width;
		sp1 = sp + im->width;

		*tp ++ = 0;


		for( j = 1 ; j < im->width-1 ; j++, sp++, sp1++, sp0++ ){

			sum = *(sp0-1) + *(sp0) + *(sp0+1) + 
				*(sp) + *(sp+1) + 
				*(sp1-1) + *(sp1) + *(sp1+1);

			sum += 4* *sp;

			*tp++ = sum >> 3;
		}

		*tp++ = 0;
	}

	for( j = 0 ; j < im->width ; j++ )
		*tp++ = 0;



	return( im );
}