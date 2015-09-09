
#include	<stdio.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif



#include "Uigp/igp.h"
#include "Utime/GpTime.h"
#include "Umath/Matrix.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"



int
imageAF_eigen( imageA_type *am, imageA_type **amE, matrix_type **ev, image_type **aim )
{
image_type	*im;
int	i;

	matrix_type	*m,	*em;


	*aim = imageAF_avarage( am, NULL );
	IMAGEF_DUMP( *aim, 0, "avarage", 1 , NULL );


	imageA_type *ams = imageAF_subtruct( am, *aim );
	



	imageAF_covariance( ams, &m );
	matrix_multiply_scalar( m, am->a[0]->width*am->a[0]->height/ (am->nA) , m );


	em = matrix_alloc( m->ni, m->nj, MT_DOUBLE );
	*ev = matrix_alloc( 1, m->nj, MT_DOUBLE );

	if( matrixSD_eigen( m, em, (*ev)->d ) < 0 )
		return( -1 );


	matrix_print( "ev", *ev, stdout );


	*amE = imageA_alloc( am->nA );


	double	f[100];
	for( i = 0 ; i < ams->nA ; i++ ){
		matrixD_get_column( em, i, f );
		
		im =  imageAF_linear_combination( ams, f );
		imageF_normal( im );
	

		(*amE)->a[(*amE)->nA++] = im;
	}


	imageA_destroy( ams );


	return( 1 );
}






int
imageAF_covariance( imageA_type *am, matrix_type **m )
{
int	i,	j;
double	*tp;

	
	*m = matrix_alloc( am->nA, am->nA, MT_DOUBLE );


	tp = (*m)->d;
	for( i = 0 ; i < (*m)->ni ; i++ ){
		for( j = 0 ; j < (*m)->nj ; j++, tp++ )
			*tp = imageF_inner( am->a[i], am->a[j] );
	}


	return( 1 );
}



image_type *
imageAF_avarage( imageA_type *am, image_type *im )
{
	int	i,	k,	N;
	double	sum;


	im = image_realloc( im, am->a[0]->width, am->a[0]->height, 1, IMAGE_TYPE_F, 1 );

	N = am->a[0]->width * am->a[0]->height;

	float *tp = im->data_f;

	for( i = 0 ; i < N ; i++, tp++ ){
		sum = 0;	
		for( k = 0 ; k < am->nA ; k++ )
			sum += am->a[k]->data[i];



		*tp = sum / am->nA;
	}

	return( im );
}


imageA_type *
imageAF_subtruct(imageA_type *am, image_type *im )
{
int	i;

	imageA_type	*tam;

	tam = imageA_alloc( am->nA );

	for( i = 0 ; i < am->nA ; i++ ){
		tam->a[tam->nA++] = imageF_subtrct_U8( am->a[i], im, NULL );
	}


	return( tam );
}


image_type *
imageAF_linear_combination(imageA_type *am, double f[] )
{
image_type *im;
int	i;

	im = image_alloc( am->a[0]->width, am->a[0]->height, 1, IMAGE_TYPE_F, 1);
	imageF_const( im, 0 );


	for( i = 0 ; i < am->nA ; i++ )
		imageF_sum( am->a[i], f[i]/am->nA, im );




	return( im );
}



void
imageAF_projection( imageA_type *amE, image_type *im, float f[] )
{
	int	i;

	for( i = 0 ; i < amE->nA ; i++ ){
		f[i] = imageF_inner( amE->a[i], im );
//		fprintf( stdout, "%.4f   ", tmp );
	}

//	fprintf( stdout, "\n" );

}