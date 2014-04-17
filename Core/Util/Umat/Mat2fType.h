#ifndef	_MAT2F_TYPE_
#define	_MAT2F_TYPE_

typedef struct mat2f_type {

	int	ni,	nj;

	float 	*f ;	

} mat2f_type ;




#define		MAT2F_ELM( mat, i, j )	\
			( (mat)->f + (i)*(mat)->nj + j  )



	// Mat2fTool.c
mat2f_type *	mat2f_alloc( int ni, int nj );

void		mat2f_free( mat2f_type *mat );


float		mat2f_inner( mat2f_type *m1, mat2f_type *m2 );


mat2f_type *	mat2f_add( mat2f_type *m1, mat2f_type *m2 );

void		mat2f_accumulate( mat2f_type *m, mat2f_type *m1, float t );


void		mat2f_const( mat2f_type *m, float a );

void		mat2f_add_const( mat2f_type *m, float a );

void		mat2f_mult_const( mat2f_type *m, float a );


	// Mat2fBase.c
void	mat2f_base_ortonormalize( mat2f_type *m[], int no );

void	mat2f_base_apply( mat2f_type *base[] ,int no, mat2f_type *mat, float f[] );

void	mat2f_base_apply_inv( mat2f_type *base[] ,int no, float f[], mat2f_type *mat );

float	mat2f_base_apply_inv1( mat2f_type *base[] ,int no, float f[], int i0, int j0 );
 
void	mat2f_base_free( mat2f_type *base[] ,int no );


	// Mat2fBaseDct.c
void	mat2f_base_dct( mat2f_type *base[], int N, int M );



	// Mat2fBaseJet2.c
void	mat2f_base_jet2( mat2f_type *base[], int N, int M );
#endif
