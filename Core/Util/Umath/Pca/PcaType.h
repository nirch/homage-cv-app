/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _PCA_TYPE_
#define _PCA_TYPE_


#ifdef __cplusplus
extern "C" {
#endif

#include	"Umath/Matrix.h"


typedef struct pca_type {
	int	n;
	int	i;

	matrix_type	*m;

	matrix_type	*a;


	matrix_type *em;
	matrix_type *ev;

} pca_type;



pca_type *pca_alloc( int n );

void	pca_destory( pca_type *pca );

void	pca_add( pca_type *pca, double f[] );


void	pca_final( pca_type *pca );



#ifdef __cplusplus
}
#endif

#endif