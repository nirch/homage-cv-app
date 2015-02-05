/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"Image3Tool.h"




image_type *
image3_bluring_y( image_type *sim )
{
u_char	*sp0, *sp1, *sp2;
int	i,	j, sumR, sumG, sumB, k;
image_type * im = image_create( sim->row, sim->column, 3, 1, NULL );

	for( i = 0 ; i < im->column ; i++ ){
		for( j = 0, sumR = 0, sumB = 0, sumG = 0; j < 11; j++){
			sp0 = IMAGE_PIXEL( sim, j, i );
			sumR += sp0[2]; sumG += sp0[1]; sumB += sp0[0];
		}

		k = 5;
		sp1 = IMAGE_PIXEL( im, k++ ,i);
		sp1[2] = sumR/11; sp1[1] = sumG/11; sp1[0] = sumB/11; 

		for( ; j < im->row ; j++ ){
			sp0 = IMAGE_PIXEL( sim, j, i );
			sp2 = IMAGE_PIXEL( sim, j-11, i );
			sumR += sp0[2] - sp2[2]; sumG += sp0[1] - sp2[1]; sumB += sp0[0] - sp2[0];
			sp1 = IMAGE_PIXEL( im, k++ ,i);
			sp1[2] = sumR/11; sp1[1] = sumG/11; sp1[0] = sumB/11; 
		}

		for( ; k < im->row ; k++ ){
			sp0 = IMAGE_PIXEL( sim, k, i );
			sp1 = IMAGE_PIXEL( im, k, i );
			sp1[2] = sp0[2]; sp1[1] = sp0[1]; sp1[0] = sp0[0];
		}

		for( j = 0; j < 5; j++){
			sp0 = IMAGE_PIXEL( sim, j, i );
			sp1 = IMAGE_PIXEL( im, j, i );
			sp1[2] = sp0[2]; sp1[1] = sp0[1]; sp1[0] = sp0[0];
		}

	}
	return( im );
}


image_type *
image3_bluring_x( image_type *sim )
{
u_char	*sp0, *sp1, *sp2;
int	i,	j, sumR, sumG, sumB, k;
image_type * im = image_create( sim->row, sim->column, 3, 1, NULL );

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0, sumR = 0, sumB = 0, sumG = 0; j < 11; j++){
			sp0 = IMAGE_PIXEL( sim, i, j );
			sumR += sp0[2]; sumG += sp0[1]; sumB +=sp0[0];
			/*if(j-5>=0)
			{
				sp1 = IMAGE_PIXEL( im, i, j - 5 );
				sp1[2] = sumR/(j+1); sp1[1] = sumG/(j+1); sp1[0] = sumB/(j+1);
			}*/
		}

		k = 5;
		sp1 = IMAGE_PIXEL( im, i, k++ );
		sp1[2] = sumR/11; sp1[1] = sumG/11; sp1[0] = sumB/11; 

		for( ; j < im->column ; j++ ){
			sp0 = IMAGE_PIXEL( sim, i, j );
			sp2 = IMAGE_PIXEL( sim, i, j-11 );
			sumR += sp0[2] - sp2[2]; sumG += sp0[1] - sp2[1]; sumB += sp0[0] - sp2[0];
			sp1 = IMAGE_PIXEL( im, i, k++ );
			sp1[2] = sumR/11; sp1[1] = sumG/11; sp1[0] = sumB/11; 
		}
		
		//fill all values ,that is not touched by the convolution.

		/*for( ; k < im->column ; k++ ){
			sp0 = IMAGE_PIXEL( sim, i, k - 5 );
			sp1 = IMAGE_PIXEL( im, i, k );
			sumR -= sp0[2]; sumG -= sp0[1]; sumB -= sp0[0];
			sp1[2] = sumR/11; sp1[1] = sumG/11; sp1[0] = sumB/11;
		}*/

		for( ; k < im->column ; k++ ){
			sp0 = IMAGE_PIXEL( sim, i, k );
			sp1 = IMAGE_PIXEL( im, i, k );
			sp1[2] = sp0[2]; sp1[1] = sp0[1]; sp1[0] = sp0[0];
		}
		for( j = 0; j < 5; j++){
			sp0 = IMAGE_PIXEL( sim, i, j );
			sp1 = IMAGE_PIXEL( im, i, j );
			sp1[2] = sp0[2]; sp1[1] = sp0[1]; sp1[0] = sp0[0];
		}

	}
	return( im );
}
