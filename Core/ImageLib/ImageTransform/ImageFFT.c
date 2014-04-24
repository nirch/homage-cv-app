/*-------------------------------------------------------------------------
   Perform a 2D FFT inplace given a complex 2D array
   The direction dir, 1 for forward, -1 for reverse
   The size of the array (nx,ny)
   Return false if there are memory problems or
      the dimensions are not powers of 2
*/
#include	"ImageType/ImageType.h"

#include <math.h>

#include	"ImageType/ImageType.h"

#include	"ImageTransform.h"

static void FFT(int m,double *real,double *imagine);
static void ReverseFFT(int m,double *real,double *imagine);

#define		IMAGE8_PIXEL( im, row, col )	\
		( ((float *)(im)->data) + (row*2)*(im)->column + (col*2) )

static int get2Power( long size);


image_type*
image_FFT(image_type *sim )
{
int i,j;
double *real,*imag;
float *p;
image_type	*im;
u_char	*sp;

	/* Transform the rows */
   real = (double *)malloc(sim->row * sizeof(double));
   imag = (double *)malloc(sim->row * sizeof(double));
   if (real == NULL || imag == NULL)
      return(FALSE);


   im = image_create( sim->row, sim->column, 8, 1, NULL );


   for (j=0;j<sim->column;j++) {

		for (i=0;i<sim->row;i++) {
			sp = IMAGE_PIXEL(sim,i,j);
			real[i] = *sp;
			imag[i] = 0;
		}

		FFT(get2Power(im->row),real,imag);
 
		for (i=0;i<im->row;i++) {
			p = IMAGE8_PIXEL(im,i,j);
			*p = real[i];
			*(p+1) = imag[i];
		} //for
   }
   free(real);
   free(imag);

   /* Transform the columns */
   real = (double *)malloc(im->column * sizeof(double));
   imag = (double *)malloc(im->column * sizeof(double));

   if (real == NULL || imag == NULL)
      return(FALSE);


   for (i=0;i<im->row;i++) {
		
	   for (j=0;j<im->column;j++) {
			p = IMAGE8_PIXEL(im,i,j);
			real[j] = *p;
			imag[j] = *(p+1);
		}

		FFT(get2Power(im->column),real,imag);

		for (j=0;j<im->column;j++) {
			p = IMAGE8_PIXEL(im,i,j);
			*p = real[j];
			*(p+1) = imag[j];
		}
   }
   free(real);
   free(imag);

   return im;
}


image_type *
image_FFT_invese( image_type *im, int Fdestroy )
{
int i,j;
double *real,*imag;
float *p;
image_type	*tim,	*tmp_im;
u_char	*tp;

   /* Transform the rows */
   real = (double *)malloc(im->row * sizeof(double));
   imag = (double *)malloc(im->row * sizeof(double));
   if (real == NULL || imag == NULL)
      return(FALSE);


   if( Fdestroy == 0 )
		tmp_im = image_create( im->row, im->column, 8, 1, NULL );
   else	tmp_im = im;


   for (j=0;j<im->column;j++) {
      for (i=0;i<im->row;i++) {
	  p = IMAGE8_PIXEL(im,i,j);
         real[i] = *p;
         imag[i] = *(p+1);
      }


      ReverseFFT(get2Power(im->row),real,imag);

		for (i=0;i<im->row;i++) {
//			 p = IMAGE8_PIXEL(im,i,j);
			p = IMAGE8_PIXEL(tmp_im, i, j);
			*p = real[i];
			*(p+1) = imag[i];
      } //for
   }
   free(real);
   free(imag);

   /* Transform the columns */
   real = (double *)malloc(im->column * sizeof(double));
   imag = (double *)malloc(im->column * sizeof(double));

   if (real == NULL || imag == NULL)
      return(FALSE);
   for (i=0;i<im->row;i++) {

       for (j=0;j<im->column;j++) {
			p = IMAGE8_PIXEL(tmp_im,i,j);
			real[j] = *p;
			imag[j] = *(p+1);
		}

		ReverseFFT(get2Power(im->column),real,imag);

		for (j=0;j<im->column;j++) {
			p = IMAGE8_PIXEL(tmp_im,i,j);
			*p = real[j];
			*(p+1) = imag[j];
		}
   }
   free(real);
   free(imag);




	tim = image_create( im->row, im->column, 1, 1, NULL );
	tp = tim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ) {
			float* p = IMAGE8_PIXEL(tmp_im, i, j);

			*tp++ = PUSH_TO_RANGE( *p, 0, 255 );
		}


	image_destroy( tmp_im, 1 );

	return( tim );
}

static int get2Power( long size)
{

		int count = 0;
		long exp = 1;
		while ( exp < size ) {
			exp = exp * 2; count++;
		} //while

		return count;

} //get2Power


/*-------------------------------------------------------------------------
	FFT computes in-place complex-to-complex FFT
   real and imagine are the real and imaginary arrays of 2^m points.

     Formula:
                  N-1
                  ---
              1   \          - j k 2 pi n / N
      X(n) = ---   >   x(k) e                    = transform
              N   /                                n=0..N-1
                  ---
                  k=0
*/
static void FFT(int m,double *real,double *imagine)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = real[i];
         ty = imagine[i];
         real[i] = real[j];
         imagine[i] = imagine[j];
         real[j] = tx;
         imagine[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * real[i1] - u2 * imagine[i1];
            t2 = u1 * imagine[i1] + u2 * real[i1];
            real[i1] = real[i] - t1;
            imagine[i1] = imagine[i] - t2;
            real[i] += t1;
            imagine[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = -(sqrt((1.0 - c1) / 2.0));
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
      for (i=0;i<nn;i++) {
         real[i] /= (double)nn;
         imagine[i] /= (double)nn;
      }
   }





/*-------------------------------------------------------------------------
	ReverseFFT computes in-place complex-to-complex reverse FFT
   real and imagine are the real and imaginary arrays of 2^m points.

      Formula: 
                  N-1
                  ---
                  \          j k 2 pi n / N
      X(n) =       >   x(k) e                    = reverse transform
                  /                                n=0..N-1
                  ---
                  k=0
*/
static void ReverseFFT(int m,double *real,double *imagine)
{
   long nn,i,i1,j,k,i2,l,l1,l2;
   double c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   nn = 1;
   for (i=0;i<m;i++)
      nn *= 2;

   /* Do the bit reversal */
   i2 = nn >> 1;
   j = 0;
   for (i=0;i<nn-1;i++) {
      if (i < j) {
         tx = real[i];
         ty = imagine[i];
         real[i] = real[j];
         imagine[i] = imagine[j];
         real[j] = tx;
         imagine[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT-1 */
   c1 = -1.0;
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0;
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<nn;i+=l2) {
            i1 = i + l1;
            t1 = u1 * real[i1] - u2 * imagine[i1];
            t2 = u1 * imagine[i1] + u2 * real[i1];
            real[i1] = real[i] - t1;
            imagine[i1] = imagine[i] - t2;
            real[i] += t1;
            imagine[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      c1 = sqrt((1.0 + c1) / 2.0);
   }

}



