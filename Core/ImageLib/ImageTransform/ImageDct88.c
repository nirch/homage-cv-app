

#include	<stdio.h>

#include	<math.h>


#include	"Umat/Mat2fType.h"
#include "ImageType/ImageType.h"




	
void 
image_copy_mat2f( image_type *im, int row, int col, mat2f_type  *m )
{
int	i,	j;

float	*f;
u_char	*p;


	f = m->f;

	for( i = 0 ; i < m->ni && row+i < IMAGE_ROW(im) ; i++ ){
		p = IMAGE_PIXEL( im, row+i, col );
		for( j = 0 ; j < m->nj && col +j < IMAGE_COLUMN(im) ; j++ )
			*f++ = *p++;
	}
}

void 
image_paste_mat2f( image_type *im, int row, int col, mat2f_type  *m )
{
int	i,	j;

float	*f;
int		a;
u_char	*p;


	f = m->f;

	for( i = 0 ; i < m->ni && row+i < IMAGE_ROW(im) ; i++ ){
		p = IMAGE_PIXEL( im, row+i, col );
		for( j = 0 ; j < m->nj && col +j < IMAGE_COLUMN(im) ; j++, f++ ){
//			a = (*f < 0)? *f-0.5 : *f +0.5;
			a = *f;
			*p = (u_char)PUSH_TO_RANGE(a, 0, 255);
			p++;
		}
	}
}


	

image_type *
image_dct88( image_type *sim )
{
image_type *tim;
mat2f_type *base[64],	*mat,	*dct;
int	i,	j;
int	row,	col;


	row = ((int)(( IMAGE_ROW(sim) + 7 )/8 )) *8;
	col = ((int)(( IMAGE_COLUMN(sim) + 7 )/8 )) *8;

	tim = image_create( row, col, 1, 1, NULL );


	mat2f_base_dct( base, 8, 8 );
	mat = mat2f_alloc( 8, 8 );
	dct = mat2f_alloc( 8, 8 );

	for( i = 0 ; i < IMAGE_ROW(sim) ; i += 8 )
		for( j = 0 ; j < IMAGE_COLUMN(sim) ; j += 8 ){
		

			image_copy_mat2f( sim, i, j, mat );

			mat2f_add_const( mat, -128 );

			mat2f_base_apply( base, 8*8, mat, dct->f );


			mat2f_mult_const( dct, 1.0/8.0 );

			mat2f_add_const( dct, 128 );

			image_paste_mat2f( tim, i, j,  dct );			
		}

		return( tim );
}



image_type *
image_idct88( image_type *sim )
{
image_type *tim;
mat2f_type *base[64],	*mat,	*dct;
int	i,	j;
int	row,	col;


	row = ((int)(( IMAGE_ROW(sim) + 7 )/8 )) *8;
	col = ((int)(( IMAGE_COLUMN(sim) + 7 )/8 )) *8;

	tim = image_create( row, col, 1, 1, NULL );


	mat2f_base_dct( base, 8, 8 );
	mat = mat2f_alloc( 8, 8 );
	dct = mat2f_alloc( 8, 8 );

	for( i = 0 ; i < IMAGE_ROW(sim) ; i += 8 )
		for( j = 0 ; j < IMAGE_COLUMN(sim) ; j += 8 ){
		

			image_copy_mat2f( sim, i, j, dct );

			mat2f_add_const( dct, -128 );
			mat2f_mult_const( dct, 8.0 );

			mat2f_base_apply_inv( base, 8*8, dct->f, mat );


			mat2f_add_const( mat, 128 );

			image_paste_mat2f( tim, i, j,  mat );			
		}


	return( tim );
}



