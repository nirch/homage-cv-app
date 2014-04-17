  /************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"

//As mentioned in last conf call, here's the low pass filter Frantz mentioned. The equations are (in 2 directions left to right, and right to left),
//y(n) = b_0 * x(n) + a_1 * y(n-1) + a_2 * y(n-2)
//y(n) = b_0 * x(n) + a_1 * y(n+1) + a_2 *y(n+2)
//where x() are inputs, y() are outputs,
//and b_0 = (1-r)^2
//a_1 = 2*r
//a_2 = -r^2
//where r is a floating point number between [0.0, 1.0]. The bigger the r, the stronger the low pass filter (more blurred).





image_type *
image1_filter_IIR( image_type *sim, float r, image_type *im )
{
	int	i,	j;
	u_char	*sp,	*tp;
	int	y2,	y1,	y;
	float	A,	B,	C;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	A = ( 1-r)*(1-r);
	B = 2*r;
	C = -r*r;

	for( i = 0 ; i < sim->height ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, i, 0 );


		y2 = *tp++ = *sp++;
		y1 = *tp++ = *sp++;

		for( j = 2 ; j < sim->width -2 ; j++, sp++ ){
			y = *sp * A + y1 * B + y2 * C;

			*tp++ = y;
			y2 = y1;
			y1 = y;
		}
		*tp++ = *sp++;
		*tp++ = *sp++;



		sp--;
		tp--;

		y2 = *tp-- = *sp--;
		y1 = *tp-- = *sp--;

		for( j = 2 ; j < sim->width -2 ; j++, sp-- ){
			y = *sp * A + y1 * B + y2 * C;

			*tp-- = y;
			y2 = y1;
			y1 = y;
		}
		*tp-- = *sp--;
		*tp-- = *sp--;

	}

	return( im );
}


image_type *
image3_filter_IIR( image_type *sim, float r, image_type *im )
{
	int	i,	j;
	u_char	*sp,	*tp;
	int	y2[3],	y1[3],	y[3];
	float	A,	B,	C;

	im = image_recreate( im, sim->height, sim->width, 3, 1 );

	A = ( 1-r)*(1-r);
	B = 2*r;
	C = -r*r;

	for( i = 0 ; i < sim->height ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 0 );
		tp = IMAGE_PIXEL( im, i, 0 );


		y2[0] = *tp++ = *sp++;
		y2[1] = *tp++ = *sp++;
		y2[2] = *tp++ = *sp++;

		y1[0] = *tp++ = *sp++;
		y1[1] = *tp++ = *sp++;
		y1[2] = *tp++ = *sp++;



		for( j = 2 ; j < sim->width -2 ; j++ ){
			y[0] = *sp++ * A + y1[0] * B + y2[0] * C;
			*tp++ = y[0];
			y2[0] = y1[0];
			y1[0] = y[0];

			y[1] = *sp++ * A + y1[1] * B + y2[1] * C;
			*tp++ = y[1];
			y2[1] = y1[1];
			y1[1] = y[1];

			y[2] = *sp++ * A + y1[2] * B + y2[2] * C;
			*tp++ = y[2];
			y2[2] = y1[2];
			y1[2] = y[2];

		}
		*tp++ = *sp++;
		*tp++ = *sp++;
		*tp++ = *sp++;
		*tp++ = *sp++;
		*tp++ = *sp++;
		*tp++ = *sp++;



		sp--;
		tp--;

		y2[2] = *tp-- = *sp--;
		y2[1] = *tp-- = *sp--;
		y2[0] = *tp-- = *sp--;


		y1[2] = *tp-- = *sp--;
		y1[1] = *tp-- = *sp--;
		y1[0] = *tp-- = *sp--;

		for( j = 2 ; j < sim->width -2 ; j++ ){
			y[2] = *sp-- * A + y1[2] * B + y2[2] * C;
			*tp-- = y[2];
			y2[2] = y1[2];
			y1[2] = y[2];

			y[1] = *sp-- * A + y1[1] * B + y2[1] * C;
			*tp-- = y[1];
			y2[1] = y1[1];
			y1[1] = y[1];

			y[0] = *sp-- * A + y1[0] * B + y2[0] * C;
			*tp-- = y[0];
			y2[0] = y1[0];
			y1[0] = y[0];

		}
		*tp-- = *sp--;
		*tp-- = *sp--;

		*tp-- = *sp--;
		*tp-- = *sp--;
		*tp-- = *sp--;
		*tp-- = *sp--;


	}

	return( im );
}




image_type *
image1_filter_IIR_RL( image_type *sim, float r, image_type *im )
{
int	i,	j;
u_char	*sp,	*tp;
int	y2,	y1,	y;
float	A,	B,	C;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	A = ( 1-r)*(1-r);
	B = 2*r;
	C = -r*r;

	sp = sim->data;
	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		y2 = *tp++ = *sp++;
		y1 = *tp++ = *sp++;
	
		for( j = 2 ; j < sim->width -2 ; j++, sp++ ){
			y = *sp * A + y1 * B + y2 * C;

			*tp++ = y;
			y2 = y1;
			y1 = y;
		}
		*tp++ = *sp++;
		*tp++ = *sp++;
	}

	return( im );
}



image_type *
image1_filter_IIR_LR( image_type *sim, float r, image_type *im )
{
	int	i,	j;
	u_char	*sp,	*tp;
	int	y2,	y1,	y;
	float	A,	B,	C;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	A = ( 1-r)*(1-r);
	B = 2*r;
	C = -r*r;

	sp = sim->data+sim->width*sim->height-1;
	tp = im->data+sim->width*sim->height-1;
	for( i = 0 ; i < sim->height ; i++ ){
		y2 = *tp-- = *sp--;
		y1 = *tp-- = *sp--;

		for( j = 2 ; j < sim->width -2 ; j++, sp-- ){
			y = *sp * A + y1 * B + y2 * C;

			*tp-- = y;
			y2 = y1;
			y1 = y;
		}
		*tp-- = *sp--;
		*tp-- = *sp--;
	}

	return( im );
}



#ifdef AA
void fgl2(u_short px[], u_short py[], int w, int h)
{
	short i, j;
	u_short A, B, C, x;
	u_int k, y, y_1=0, y_2=0;
	const u_short shift = 16;

	// second order IIR filter for FGL implementation:
	// y(n)=A*x(n)+B*y(n-1)+C*y(n-2)

	//mpyu unsigned multiply 
	px+=2; py+=2;
	for(i=2; i<(h-2); i++)
	{
		for(j=2; j<(w-2); j++)
		{
			// this takes 5 cycleson a C6x DSP core
			x = *(px++);
			k =_mpyu(A, x);
			y =_mpyu(C, y_2);
			y =y+k;
			y_2=y_1;

			y_1=_mpyu(B, y_1);

			y_1=y_1+y;
			y_1=y_1 >> shift;
			*(py++)=y_1; //the value are modified in situ
		}
		px+=4; py+=4; y_1=0, y_2=0; // start a new line
	}
}
#endif