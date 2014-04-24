/******************************
 ***    Image1Convolution.c  ***
 *****************************/
#include	"Umath/Matrix.h" 
#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"

 

 static float	image1_innerMF( image_type *im, int i0, int j0, matrix_type *m );

 
image_type *
image_convolutionM( image_type *sim, matrix_type *m, image_type *im )
{
u_char  *p,	*sp;
int    i,      j;
int	d,	val;
 

	if( sim->depth != 1 )	return( NULL );


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );
 
	d = m->ni / 2;
 
	sp = (u_char *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			if( i-d < 0 || i + d >= sim->height || j-d < 0 || j + d >= sim->width ){
					*p = *sp ;
					p++;
					sp++;
					continue;
			}
 
			val = image1_innerMF( sim, i-d, j-d, m );
			*p = PUSH_TO_RANGE( val, 0, 255 );

			p++;
			sp++;
		}

	return( im );
}




static float
image1_innerMF( image_type *im, int i0, int j0, matrix_type *m )
{
u_char  *p;
int     i,      j;
float	*mf,	sum = 0;

	

	mf = m->f;
	for( i = 0 ; i < m->ni ; i++ ){
		p = IMAGE_PIXEL( im, i0+i, j0 );
		for( j = 0 ; j < m->nj ; j++ ){

			sum += (*mf) * (*p);

			p++;
			mf++;
		}
	}

	return( sum );
}

