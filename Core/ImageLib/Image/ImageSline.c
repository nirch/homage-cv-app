/******************************
 ***    ImageConvolution.c  ***
 *****************************/
#include	<math.h>
#include	"ImageType/ImageType.h"

 
 
 
image_type *
image_sline(  vec2d *n, float d0, float d1, float a,
				  int c0, int c1, int c2, int row, int col )
{
image_type	*im;
u_char  *p;
int     i,      j;
int		val;
float	t;
 



	t = (float)sqrt(n->x *n->x + n->y*n->y);
	n->x /= t;
	n->y /= t;

	im = image_create( row, col, 1, 1, NULL );
 

 
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			t = n->x*i + n->y*j;

			if( t < d0 - a ){
				*p++ = c0;
				continue;
			}

			if( t < d0 +a ){
				t -= d0;
				val = (c0* (a-t) + c1*(t+a))/(2*a);
				*p++ = val;
				continue;
			}


			if( t < d1 - a ){
				*p++ = c1;
				continue;
			}

			if( t < d1 +a ){
				t -= d1;
				val = (c1* (a-t) + c2*(t+a))/(2*a);
				*p++ = val;
				continue;
			}

 
			*p++ = c2;
		}

	return( im );
}




image_type *
image_S_cosinos(  float a, float n, int row, int col )
{
image_type	*im;
u_char  *p;
int     i,      j;
float	t,	b;
 


	im = image_create( row, col, 1, 1, NULL );
 

 
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			t =(float)(2*M_PI*((n*j/(float)im->column)));

 
			b = 100 *cos( a + t ) + 128;
			*p++ = (u_char)b;
		}

	return( im );
}