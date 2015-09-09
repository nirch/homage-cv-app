/*******************************************
 *** ImageDct.c  - Worte by neta and ginam *
 *******************************************/


#include <stdio.h>

#include	"Uigp/igp.h"

#include "ImageType/ImageType.h"




#include <math.h>



double C(int m, int n, image_type *sim,double cM0,double cM,double cN0,double cN,
		 double **cosM, double **cosN)
{
	int x,y,N,M;
	double prod = 0.0, cos2, Ixy, c1, c2;
	u_char	*p;
	double	tmp;

	M = sim->row;
	N = sim->column;

	p = sim->data;

	for (x=0; x<M; x++)
	{
		tmp = 0;
		for (y=0; y<N; y++, p++)
		{
			Ixy = (float)( (*p) - 127.5 );
			cos2 = cosN[y][n];
			tmp += Ixy * cos2;
		}

		prod += tmp * cosM[x][m];
	}

	if (m==0)
		c1 = cM0;
	else
		c1 = cM;
	if (n==0)
		c2 = cN0;
	else
		c2 = cN;
	prod *= c1 * c2;
	return prod;
}



double I(int x, int y, image_type *sim,double cM0,double cM,double cN0,double cN,
		 double **cosM, double **cosN)
{
	int m,n,N,M;
	double prod = 0.0, cos2, Cmn, c1, c2;
	float	*p;
	double	tmp;

	M = sim->row;
	N = sim->column;

	p = (float *)sim->data;
	for (m=0; m<M; m++)
	{
		tmp = 0;

		for (n=0; n<N; n++, p++)
		{
			Cmn = *p;
			cos2 = cosN[y][n];

			if (n==0)
				c2 = cN0;
			else
				c2 = cN;

			tmp += Cmn * cos2 * c2;
		}


		if (m==0)
				c1 = cM0;
		else
				c1 = cM;

		prod += tmp *c1 *cosM[x][m];


	}
	return (prod+127.5);
}




image_type * image_dct(image_type *sim)
{
float	*p;
image_type	*im;
int	M, N, i, j;
double P = M_PI, cN0, cN, cM0, cM, ** cosM, ** cosN;
	
	M = sim->row;
	N = sim->column;
	cosM = (double **)malloc( sizeof(double*)*M );
	for (i=0; i<M; i++)
	{
		cosM[i] = (double *)malloc( sizeof(double)*M );
	}
	cosN = (double **)malloc( sizeof(double*)*N );
	for (i=0; i<N; i++)
	{
		cosN[i] = (double *)malloc( sizeof(double)*N );
	}
	cM0 = sqrt (1.0 / (double)M);
	cM = sqrt (2.0 / (double)M);
	cN0 = sqrt (1.0 / (double)N);
	cN = sqrt (2.0 / (double)N);
	for (i=0; i<M; i++)
		for (j=0; j<M; j++)
			cosM[i][j] = cos( ((2.0*((double)i)+1.0)*((double)j)*P)/(2.0*(double)M) );
	for (i=0; i<N; i++)
		for (j=0; j<N; j++)
			cosN[i][j] = cos( ((2.0*((double)i)+1.0)*((double)j)*P)/(2.0*(double)N) );
	im = image_create( M, N, 4, 1, NULL );
	p = (float *)im->data;
	for( i = 0 ; i < M; i++ )
	{
		for( j = 0 ; j < N; j++ )
		{
			*p = C(i,j,sim,cM0,cM,cN0,cN,cosM,cosN);
			p++;
		}
	}
	for (i=0; i<M; i++)
	{
		free( cosM[i]);
	}
	free (cosM);
	for (i=0; i<N; i++)
	{
		free( cosN[i]);
	}
	free( cosN);
	return im;
}

image_type * image_dct_inverse(image_type *sim)
{
u_char	*p;
image_type	*im;
int	M, N, x, y, m, n, i;
double P = M_PI, cN0, cN, cM0, cM, ** cosM, ** cosN;
	
	M = IMAGE_ROW(sim);
	N = IMAGE_COLUMN(sim);
	cosM = (double **)malloc( sizeof(double*)*M );
	for (x=0; x<M; x++)
	{
		cosM[x] = (double *)malloc( sizeof(double)*M );
	}
	cosN = (double **)malloc( sizeof(double*)*N );
	for (y=0; y<N; y++)
	{
		cosN[y] = (double *)malloc( sizeof(double)*N );
	}
	cN0 = sqrt (1.0 / (double)N);
	cN = sqrt (2.0 / (double)N);
	cM0 = sqrt (1.0 / (double)M);
	cM = sqrt (2.0 / (double)M);
	for (x=0; x<M; x++)
		for (m=0; m<M; m++)
			cosM[x][m] = cos( ((2.0*(double)x+1.0)*(double)m*P)/(2.0*(double)M) );
	for (y=0; y<N; y++)
		for (n=0; n<N; n++)
			cosN[y][n] = cos( ((2.0*(double)y+1.0)*(double)n*P)/(2.0*(double)N) );
	im = image_create( M, N, 1, 1, NULL );
	p = (u_char *)im->data;
	for( x = 0 ; x < M ; x++ )
	{
		for( y = 0 ; y < N ; y++ )
		{
			double ccc = I(x,y,sim,cM0,cM,cN0,cN,cosM,cosN);
			if (ccc<0)
				ccc=0;
			if (ccc>255)
				ccc=255;
			*p = (u_char)(ccc+0.5);
			p += im->depth;
		}
	}
	for (i=0; i<M; i++)
	{
		free( cosM[i]);
	}
	free( cosM);
	for (i=0; i<N; i++)
	{
		free(cosN[i]);
	}
	free( cosN);
	return im;
}


image_type * image_dct_draw(image_type *sim)
{
u_char	*p;
image_type	*im;
int	M, N, i, j;
float min, max, cur, MAX_COLOR = 255.0;
	
	M = sim->row;
	N = sim->column;
	im = image_create( M, N, 1, 1, NULL );
	max = min = (*((float *)(IMAGE_PIXEL( sim, 0, 0))));
	for( i = 0 ; i < M; i++ )
	{
		for( j = 0 ; j < N; j++ )
		{
			cur = (*((float *)(IMAGE_PIXEL( sim, i, j))));
			if (cur<min)
				min=cur;
			if (cur>max)
				max=cur;
		}
	}
	p = (u_char *)im->data;
	for( i = 0 ; i < M; i++ )
	{
		for( j = 0 ; j < N; j++ )
		{
			cur = (*((float *)(IMAGE_PIXEL( sim, i, j))));
			cur = ( MAX_COLOR * (cur-min))/(max-min);
			*p = (u_char) cur;
			p += im->depth;
		}
	}
	return im;
}





void
image_dct_highpass_filter( image_type *im, int N )
{
int	i,	j;
float	*p;


	if( IMAGE_COLUMN(im) > N ){
		for( i = 0 ; i < N && i < IMAGE_ROW(im) ; i++ ){
			p = (float *)IMAGE_PIXEL( im, i, N );
			for( j = N ; j < IMAGE_COLUMN(im) ; j++ )
				*p++ = 0;
		}
	}

	for( i = N; i < IMAGE_ROW(im) ; i++ ){
		p = (float *)IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < IMAGE_COLUMN(im) ; j++ )
			*p++ = 0;
	}
}

