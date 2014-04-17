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
image3_to_YCrCb( image_type *im, image_type **imY, image_type **imCr, image_type **imCb )
{
u_char	*sp;
u_char	*pY,	*pCr,	*pCb;

int	row,	col,	i,	j;
int	R,	G,	B;


	row = IMAGE_ROW(im);
	col = IMAGE_COLUMN(im);


	*imY  = image_create( row, col, 1, 1, NULL );
	*imCr = image_create( row, col, 1, 1, NULL );
	*imCb = image_create( row, col, 1, 1, NULL );



	sp =  (u_char *)im->data;

	pY =  (*imY)->data;
	pCr = (*imCr)->data;
	pCb = (*imCb)->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

#ifdef MEP_FLOAT
			*pY++   =  0.29900 * R   + 0.58700 * G  + 0.11400 * B ;	
			*pCr++  =      0.5 * R   - 0.41869 * G  - 0.08131 * B + 128;
			*pCb++  = -0.16874 * R   - 0.33126 * G  +     0.5 * B + 128;
#else
#endif
		}
	}
}



image_type *
image3_YCrCb_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im )
{
u_char	*tp;
u_char	*pY,	*pCr,	*pCb;
int	i,	j;
int	R,	G,	B,	Y,	Cr,	Cb;


	im = image_recreate( im, imY->height, imY->width, 3, 1 );


	tp =  im->data;

	pY =  imY->data;
	pCr = imCr->data;
	pCb = imCb->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			Y  = *pY++;
			Cr = *pCr++;
			Cb = *pCb++;

#ifdef MEP_FLOAT
			R = 1.402*( Cr - (128)) + Y;
			B = 1.772*(Cb - (128)) + Y;
			G = 1.7035*( Y - 0.299*R - 0.114*B);

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
#else
#endif
		}
	}


	return( im );
}




image_type *
image3_YCrCb421_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im )
{

u_char	*tp;
u_char	*pY,	*pCr,	*pCb;

int	i,	j;
int	R,	G,	B,	Y,	Cr,	Cb;


	im = image_recreate( im, imY->height, imY->width, 3, 1 );




	tp =  im->data;

	for( i = 0 ; i < im->row ; i++ ){
		pY = IMAGE_PIXEL( imY, i, 0 );
		pCr = IMAGE_PIXEL( imCr, i, 0 );
		pCb = IMAGE_PIXEL( imCb, i, 0 );
		for( j = 0 ; j < im->column/2 ; j++ ){


			Cr = *pCr++;
			Cb = *pCb++;
			Y  = *pY++;

#ifdef MEP_FLOAT
			R = 1.402*( Cr - (128)) + Y;
			B = 1.772*(Cb - (128)) + Y;
			G = 1.7035*( Y - 0.299*R - 0.114*B);

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
#else
#endif

			Y  = *pY++;

#ifdef MEP_FLOAT
			R = 1.402*( Cr - (128)) + Y;
			B = 1.772*(Cb - (128)) + Y;
			G = 1.7035*( Y - 0.299*R - 0.114*B);

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
#else
#endif
		}
	}


	return( im );
}

image_type *
image3_YCrCb422_to_RGB( image_type *imY, image_type *imCr, image_type *imCb, image_type *im )
{

	u_char	*tp;
	u_char	*pY,	*pCr,	*pCb;

	int	i,	j;
	int	R,	G,	B,	Y,	Cr,	Cb;


	im = image_recreate( im, imY->height, imY->width, 3, 1 );




	tp =  im->data;

	for( i = 0 ; i < im->row ; i++ ){
		pY = IMAGE_PIXEL( imY, i, 0 );
		pCr = IMAGE_PIXEL( imCr, i/2, 0 );
		pCb = IMAGE_PIXEL( imCb, i/2, 0 );
		for( j = 0 ; j < im->column/2 ; j++ ){


			Cr = *pCr++;
			Cb = *pCb++;
			Y  = *pY++;

#ifdef MEP_FLOAT
			R = 1.402*( Cr - (128)) + Y;
			B = 1.772*(Cb - (128)) + Y;
			G = 1.7035*( Y - 0.299*R - 0.114*B);

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
#else
#endif

			Y  = *pY++;

#ifdef MEP_FLOAT
			R = 1.402*( Cr - (128)) + Y;
			B = 1.772*(Cb - (128)) + Y;
			G = 1.7035*( Y - 0.299*R - 0.114*B);

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
#else
#endif
		}
	}


	return( im );
}
