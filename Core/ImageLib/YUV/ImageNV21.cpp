/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/

#include	<string.h>
#include	"Uigp/igp.h"
#include	"Image2YUV.h"



#define SCALEBITS 8
#define ONE_HALF (1 << (SCALEBITS - 1))
#define FIX(x) ((int) ((x) * (1L<<SCALEBITS) + 0.5))


#ifdef _FIX
#define COLOR_YUV2RGB( y, u, v, R, G, B )	\
	{ \
			R = 1.402*( u - (128)) + Y; \
			B = 1.772*(v - (128)) + Y; \
			G = 1.7035*( Y - 0.299*R - 0.114*B); \
	}
#else	
#define COLOR_YUV2RGB( y, u, v, R, G, B )	\
	{ \
		R = ((FIX(1.402)*( u - (128)))>>SCALEBITS) + Y; \
		B = ((FIX(1.772)*(v - (128)))>>SCALEBITS) + Y; \
		G = (FIX(1.7035)* Y - FIX(1.7035*0.299)*R -FIX(1.7035* 0.114)*B )>>SCALEBITS; \
	}
#endif





image_type *
imageNV21_to_RGB( image_type *sim, image_type *im )
{
	u_char	*tp;
	u_char	*pY,	*pCr,	*pCb;

	int	i,	j;
	int	R,	G,	B,	Y,	Cr,	Cb;


//	im = image_recreate( im, sim->height, sim->width, 3, 1 );
	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );




	tp = im->data;
	pY = sim->data;
	pCr = sim->data + sim->width*sim->height;


	for( i = 0 ; i < im->row ; i++ ){
		pCb = pCr+1; 
		for( j = 0 ; j < im->column/2 ; j++ ){

			Cr = *pCr++;
			Cb = *pCr++;
	
			Y  = *pY++;


			COLOR_YUV2RGB( y, Cr, Cb, R, G, B )

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
			
			Y  = *pY++;

			COLOR_YUV2RGB( y, Cr, Cb, R, G, B )

			*tp++ = PUSH_TO_RANGE( R, 0, 255 );
			*tp++ = PUSH_TO_RANGE( G, 0, 255 );
			*tp++ = PUSH_TO_RANGE( B, 0, 255 );
		}


		if( (i & 0x01) == 0 ){
			//pCr -= sim->width/2;
			pCr -= sim->width;
		}


	}


	return( im );
}





image_type *
imageNV21_to_RGB_swap( image_type *sim, image_type *im )
{
	u_char	*tp;
	u_char	*pY,	*pCr;

	int	i,	j;
	int	R,	G,	B,	Y,	Cr,	Cb;


	im = image_realloc( im, sim->height, sim->width, 3, IMAGE_TYPE_U8, 1 );


	tp = im->data;
	pY = sim->data;
	pCr = sim->data + sim->width*sim->height;


	for( i = 0 ; i < im->width ; i++ ){
		tp = IMAGE_PIXEL( im, im->height-1, i );
		for( j = 0 ; j < im->height/2 ; j++ ){

			Cr = *pCr++;
			Cb = *pCr++;
	
			Y  = *pY++;

			COLOR_YUV2RGB( y, Cr, Cb, R, G, B )

			tp[0] = PUSH_TO_RANGE( R, 0, 255 );
			tp[1] = PUSH_TO_RANGE( G, 0, 255 );
			tp[2] = PUSH_TO_RANGE( B, 0, 255 );

			tp -= 3*im->width;

			Y  = *pY++;

			COLOR_YUV2RGB( y, Cr, Cb, R, G, B )


			tp[0] = PUSH_TO_RANGE( R, 0, 255 );
			tp[1] = PUSH_TO_RANGE( G, 0, 255 );
			tp[2] = PUSH_TO_RANGE( B, 0, 255 );

			tp -= 3*im->width;

		}


		if( (i & 0x01) == 0 ){
			pCr -= sim->width;
		}

	}


	return( im );
}


image_type *
	imageNV21_to_RGB_swap_inv( image_type *sim, image_type *im )
{
	u_char	*tp;
	u_char	*pY,	*pCr;

	int	i,	j;
	int	R,	G,	B,	Y,	Cr,	Cb;


	im = image_realloc( im, sim->height, sim->width, 3, IMAGE_TYPE_U8, 1 );


	tp = im->data;
	pY = sim->data;
	pCr = sim->data + sim->width*sim->height;


	for( i = 0 ; i < im->width ; i++ ){
//		tp = IMAGE_PIXEL( im, im->height-1, i );
		tp = IMAGE_PIXEL( im, 0, i );
		for( j = 0 ; j < im->height/2 ; j++ ){

			Cr = *pCr++;
			Cb = *pCr++;

			Y  = *pY++;

			COLOR_YUV2RGB( y, Cr, Cb, R, G, B )

			tp[0] = PUSH_TO_RANGE( R, 0, 255 );
			tp[1] = PUSH_TO_RANGE( G, 0, 255 );
			tp[2] = PUSH_TO_RANGE( B, 0, 255 );

			tp += 3*im->width;

			Y  = *pY++;

			COLOR_YUV2RGB( y, Cr, Cb, R, G, B )


				tp[0] = PUSH_TO_RANGE( R, 0, 255 );
			tp[1] = PUSH_TO_RANGE( G, 0, 255 );
			tp[2] = PUSH_TO_RANGE( B, 0, 255 );

			tp += 3*im->width;

		}


		if( (i & 0x01) == 0 ){
			pCr -= sim->width;
		}

	}


	return( im );
}



image_type *
imageNV21_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
{
	int	i;

	 
	if( im == NULL )
		im = image_createYUV420( height, width, NULL );


	u_char *sp = sim->data + y0*sim->width + x0;
	u_char *tp = im->data;
	for( i = 0 ; i < im->height ; i++, tp+= im->width, sp += sim->width )
		memcpy( tp, sp, im->width );



	int w = 0.5*sim->width;
	sp = sim->data + sim->width*sim->height + (int)(0.5* y0*w + 0.5*x0);

	for( i = 0 ; i < 0.5*im->height ; i++, tp+= im->width, sp += sim->width )
		memcpy( tp, sp, im->width );


	return( im );
}



image_type *
imageNV21_sample2( image_type *sim, image_type *im )
{
	int	i,	j;


	if( im == NULL )
		im = image_createYUV420( 0.5*im->height, 0.5*im->width, NULL );


	u_char *sp = sim->data;
	u_char *sp1 = sp + sim->width;
	u_char *tp = im->data;

	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, sp += 2, sp1 += 2 ){
			*tp++ = (sp[0] + sp[1] + sp1[0] + sp1[1])>>2;
		}
	}


	for( i = 0 ; i < 0.5*im->height ; i++ ){
		for( j = 0 ; j < im->width ; j++, sp += 4, sp1 += 4 ){
			*tp++ = (sp[0] + sp[2] + sp1[0] + sp1[2])>>2;
			*tp++ = (sp[1] + sp[3] + sp1[1] + sp1[3])>>2;
		}
	}


	return( im );
}