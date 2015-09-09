/********************** Fourier Transform *************
 *	Worte by Avri Golan - 12-2000					***
 ******************************************************/

#include	"ImageType/ImageType.h"

#include <math.h>

#define		IMAGE8_PIXEL( im, row, col )	\
		( ((float *)(im)->data) + (row*2)*(im)->column + (col*2) )


static void dft2d(image_type * im,int n,int m,float * Real,float * Imagine);

static int idft2d(image_type * im,int n,int m);




image_type * 
image_fourier_amplitod( image_type *im )
{

	float p1,p2;
	unsigned char * p;
	image_type *dim;
	int row=IMAGE_ROW(im)
		,col=IMAGE_COLUMN(im)
		,i,j,real;

	dim = image_create( row,col, 1, 1, NULL );
	for( i = 0 ; i < row ; i++ )
		for( j = 0 ; j < col ; j++ ){
			dft2d(im,i,j,&p1,&p2);

			p=IMAGE_PIXEL( dim, i, j );

			real=(int)sqrt(p1*p1+p2*p2);
			*p =PUSH_TO_RANGE( real, 0, 255 );
		}
	return dim;
}


image_type *
image_fourier(image_type * sim)
{
	float *p;
	image_type *dim;
	int row=IMAGE_ROW(sim)
		,col=IMAGE_COLUMN(sim)
		,i,j;
	dim = image_create( row,col, 8, 1, NULL );
	for( i = 0 ; i < row ; i++ )
		for( j = 0 ; j < col ; j++ ){
			p=IMAGE8_PIXEL( dim, i, j );
			dft2d(sim,i,j,p,p+1);
		}
	return dim;
}
 
 

image_type *
image_fourier_inverse(image_type *im){
	u_char *p;
	image_type *dim;
	int row=IMAGE_ROW(im)
		,col=IMAGE_COLUMN(im)
		,val,i,j;
	dim = image_create( row,col, 1, 1, NULL );
	for( i = 0 ; i < row ; i++ )
		for( j = 0 ; j < col ; j++ ){
			val=idft2d(im,i,j);
			p=IMAGE_PIXEL( dim, i, j );
			*p = val;
		}
	return dim;
}

static void 
dft2d(image_type * im,int n,int m,float * Real,float * Imagine)
{
	int N=IMAGE_ROW(im),
		M=IMAGE_COLUMN(im);
	int x,y;
	u_char *p;
	float real=0
  ,imagine=0,temp,temp1;

	for (x=0;x<N;x++)
		for (y=0;y<M;y++){
			temp=(float)(2*M_PI*((n*x/(float)N)+(m*y/(float)M)));
			p=IMAGE_PIXEL( im, x, y );
			temp1=(float)(*p);
			real+=(float)(temp1*cos(temp));
			imagine-=(float)(temp1*sin(temp));
		}
	real=real/(N*M);
	imagine=imagine/(N*M);
	*Real=real;
	*Imagine=imagine;
}


static int
idft2d(image_type * im,int n,int m)
{
 int N=IMAGE_ROW(im),
  M=IMAGE_COLUMN(im);
 int x,y;
 float *p;
 float real,imagine,answer=0,
  temp,si,co;
 for (x=0;x<N;x++)
  for (y=0;y<M;y++){
	  	temp=(float)(2*M_PI*((n*x/(float)N)+(m*y/(float)M)));
   p=IMAGE8_PIXEL( im, x, y );
   real=*p;
   imagine=*(p+1);
   si=(float)sin(temp);
   co=(float)cos(temp);
   answer+=(float)(real*co-imagine*si);
  }
answer=PUSH_TO_RANGE( answer, 0, 255 );
 return (int)answer;
}

/*******************End of Fourier Transform***************************/
