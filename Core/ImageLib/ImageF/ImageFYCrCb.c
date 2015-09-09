/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"



#define MEP_FLOAT

 //
 //      Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 //      Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
 //      Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128
 



void
imageF3_to_YCrCb( image_type *im, image_type **imY, image_type **imCr, image_type **imCb )
{
float	*sp;
float	*pY,	*pCr,	*pCb;
float	R,	G,	B;
int	i,	j;


	*imY  = image_create( im->height, im->width, 4, 1, NULL );
	*imCr = image_create( im->height, im->width, 4, 1, NULL );
	*imCb = image_create( im->height, im->width, 4, 1, NULL );



	sp =  im->data_f;

	pY =  (*imY)->data_f;
	pCr = (*imCr)->data_f;
	pCb = (*imCb)->data_f;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;


			*pY++   =  0.29900 * R   + 0.58700 * G  + 0.11400 * B;
			*pCr++  =      0.5 * R   - 0.41869 * G  - 0.08131 * B  + 128;
			*pCb++  = -0.16874 * R   - 0.33126 * G  +     0.5 * B  + 128;


		}
	}
}



image_type *
imageF3_YCrCb_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im )
{

float	*tp;
float	*pY,	*pCr,	*pCb;

int	i,	j;
float	R,	G,	B,	Y,	Cr,	Cb;



	im  = image_recreate( im, imY->height, imY->width, 12, 1 );


	tp =  im->data_f;

	pY =  imY->data_f;
	pCr = imCr->data_f;
	pCb = imCb->data_f;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			Y  = *pY++;
			Cr = *pCr++;
			Cb = *pCb++;


			R = 1.402*( Cr - 128) + Y;
			B = 1.772*(Cb - 128) + Y;
			G = 1.7035*( Y - 0.299*R - 0.114*B);

			*tp++ = R;
			*tp++ = G;
			*tp++ = B;
		}
	}


	return( im );
}

