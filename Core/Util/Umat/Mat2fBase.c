/************************
 ***	Mat2fBase.c   ***
 ************************/
#include	<stdio.h>

#include	<math.h>
#include	"Uigp/igp.h"



#include	"Umat/Mat2fType.h"




void 
mat2f_base_ortonormalize( mat2f_type *m[], int no )
{
int i,	j;
float	t;

	for( i = 0 ; i < no ; i++ ){

		for( j = 0 ; j < i ; j++ ){

			t = mat2f_inner( m[j],m[i] );

			mat2f_accumulate( m[i], m[j], -t );
		}


		t = mat2f_inner( m[i],m[i] );

		mat2f_mult_const( m[i], 1.0 / sqrt(t) );
	}
}



void 
mat2f_base_apply( mat2f_type *base[] ,int no, mat2f_type *mat, float f[] )
{
int i;


	for (i = 0; i < no ; i++)
		*f++ = mat2f_inner( base[i], mat );		
}


void 
mat2f_base_apply_inv( mat2f_type *base[] ,int no, float f[], mat2f_type *mat )
{
int i;

	mat2f_mult_const( mat, 0.0 );

	for (i = 0; i < no ; i++ )
		mat2f_accumulate( mat, base[i], f[i] );
}


float 
mat2f_base_apply_inv1( mat2f_type *base[] ,int no, float f[], int i0, int j0 )
{
int i;
float	val,	*p;

	val = 0;
	for (i = 0; i < no ; i++ ){
		p = MAT2F_ELM( base[i], i0, j0 );
		val += f[i] * *p;
	}

	return( val );
}

void 
mat2f_base_free( mat2f_type *base[], int no )
{
int i;

	for (i = 0; i < no ; i++)
		mat2f_free( base[i] );	
}
