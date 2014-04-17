/*
  Image2YUV.cpp by KEN
  image_type to YUV convertion utilities definitions
*/
#include "Image2YUV.h"

#define SCALEBITS 8
#define ONE_HALF (1 << (SCALEBITS - 1))
#define FIX(x) ((int) ((x) * (1L<<SCALEBITS) + 0.5))


int 
image_GetYUVFrame( image_type *sim, u_char **data, int *bytes )
{
image_type	*tim;
int width;
int height;


	tim = image3_from( sim, NULL );

	width = sim->width;
	height = sim->height;



	// allocate memory for ouput yuv frame:
	// 4 RGB pixels are encoded with 4 Y, 2 U and 2 V (6 bytes)
	*bytes = tim->height * tim->width * 6 / 4;


	*data = (unsigned char*) malloc(sizeof(char) * (*bytes));
	if (*data == 0) {
		return -1;
	}

	int iUOffset = tim->height * tim->width;
	int iVOffset = iUOffset + (tim->width / 2) * (tim->height / 2);	// "/2" is important if width or height is odd

	image_RGB24_to_YUV420P( *data, *data + iUOffset, *data + iVOffset,
		tim->data, tim->width, tim->height );


	if( tim != sim )
		image_destroy( tim, 1 );

	return 0;
}


void image_RGB24_to_YUV420P(unsigned char* lum, unsigned char* cb, unsigned char* cr,
					  unsigned char* src, int width, int height)
{
struct _RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
}* _src = (_RGB*) src;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
		*(lum++) = (FIX(0.29900) * _src->r + FIX(0.58700) * _src->g + FIX(0.11400) * _src->b 
        	+ ONE_HALF) >> SCALEBITS;
		_src++;
		}
	}

	_src = (_RGB*) src;

	for (int i = 0; i < height / 2; i++) {
		for (int j = 0; j < width / 2; j ++) {
			int r = _src[0].r + _src[1].r + _src[width].r + _src[width + 1].r;
			int g = _src[0].g + _src[1].g + _src[width].g + _src[width + 1].g;
			int b = _src[0].b + _src[1].b + _src[width].b + _src[width + 1].b;
			_src += 2;

			*(cb++) = ((- FIX(0.16874) * r - FIX(0.33126) * g + FIX(0.50000) * b 
					+ 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;
			*(cr++) = ((FIX(0.50000) * r - FIX(0.41869) * g - FIX(0.08131) * b 
					+ 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;
		}
		_src += width;	// skip line
	}
}




image_type *
image3_to_YUV420( image_type *sim, image_type *im )
{
int	r,	g,	b;
u_char	*sp,	*sp1;
u_char	*tp,	*cr,	*cb;
int	i,	j;

	if( im != NULL ){
		if( im->format != IMAGE_FORMAT_YUV420 || im->width != sim->width || im->height != sim->height ){
			image_destroy( im, 1 );
			im = NULL;
		}
	}

	if( im == NULL )
		im = image_createYUV420( sim->height, sim->width, NULL );



	tp = im->data;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );

		for( j = 0 ; j < im->width ; j++ ){
			r = *sp++;
			g = *sp++;
			b = *sp++;
			*tp++ = (FIX(0.29900) * r + FIX(0.58700) * g + FIX(0.11400) * b + ONE_HALF) >> SCALEBITS;
		}
	}


	cb = im->data + im->height * im->width;
	cr = cb + im->height * im->width / 4;

	for( i = 0 ; i < im->height/2 ; i++ ){
		sp = IMAGE_PIXEL( sim, 2*i, 0 );
		sp1 = sp + 3*sim->width;

		for( j = 0 ; j < im->width/2 ; j++ ){
			r = *sp + *(sp+3) + *sp1 + *(sp1+3);
			g = *(sp+1) + *(sp+4) + *(sp1+1) + *(sp1+4);
			b = *(sp+2) + *(sp+5) + *(sp1+2) + *(sp1+5);
			sp += 6;
			sp1 += 6;



			*(cb++) = ((- FIX(0.16874) * r - FIX(0.33126) * g + FIX(0.50000) * b 
				+ 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;
			*(cr++) = ((FIX(0.50000) * r - FIX(0.41869) * g - FIX(0.08131) * b 
				+ 4 * ONE_HALF - 1) >> (SCALEBITS + 2)) + 128;
		}
	}


	return( im );
}


//
//      Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
//      Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + 128
//      Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + 128


#define MEP_FLOAT

image_type *
imageYUV420_to_RGB( image_type *sim, image_type *im )
{
u_char	*tp;
u_char	*pY,	*pCr,	*pCb;

int	i,	j;
int	R,	G,	B,	Y,	Cr,	Cb;


	im = image_recreate( im, sim->height, sim->width, 3, 1 );




	tp = im->data;
	pY = sim->data;
	pCb = sim->data + sim->width*sim->height;
	pCr =  pCb + sim->width*sim->height/4;

	for( i = 0 ; i < im->row ; i++ ){
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
		if( (i & 0x01) == 0 ){
			pCr -= sim->width/2;
			pCb -= sim->width/2;
		}


	}


	return( im );
}