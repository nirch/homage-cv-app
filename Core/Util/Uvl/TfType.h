#ifndef _TF_TYPE_
#define _TF_TYPE_


#include <stdio.h>
//#ifdef __cplusplus
//extern "C" {
//#endif


//#define TF_MAX	128


typedef struct	tf_type {
	char name[64];
	int	iFrame;
	int	id;

	int NA;
	int	nA;
	float	*a;
	//float	a[TF_MAX];

} tf_type;


typedef struct tfA_type {
	union {
//		int	NT;
		int NA;
	};

	union {
//		int	nT;
		int	nA;
	};



	tf_type	**a;


}	tfA_type;


tf_type *	tf_alloc( int n );

tf_type *	tf_copy( tf_type *stf, tf_type *tf );



void	tf_destroy( tf_type *tf );

void	tf_clear( tf_type *t );

void	tf_set( tf_type *t, float val );

tf_type *	tf_create( int iFrame, float a[], int nA );

tf_type *	tf_append( tf_type *t0, tf_type *t1 );


void	tfA_axb( tfA_type *at, float a, float b );

void	tf_axb( tf_type *t, float a, float b );

void	tf_avarge( tf_type *t, float *av, float *var );


void	tf_accumulate( tf_type *st, tf_type *tf, float a );


int	tfA_nearest( tf_type *tf, float a0, float T );



tfA_type *	tfA_alloc( int no );

void	tfA_destroy( tfA_type *aS );


void	tfA_clear( tfA_type *aS );

tfA_type *tfA_create( int nT, int nA );

void	tfA_copy_name( tfA_type *atf, tfA_type *stf );



tfA_type *	tfA_copy( tfA_type *as, tfA_type *at );


int	tf_write( tf_type *s, int prefix, FILE *fp );
int	tfA_write( tfA_type *aS, int prefix, char *file );


tfA_type *	tfA_read( char *file );


int	tfA_read( tfA_type **atf, char *file );

int	tfA_read_from_data( tfA_type **atf, char *data );


tf_type *	tfA_average( tfA_type *tfA );


tfA_type *	tfA_transform( tfA_type *stfA, struct matrix_type *m );


void	tfA_set( tfA_type *tfA, int iFrame, float a[], int nA );

void	tfA_add( tfA_type *tfA, int iFrame, tf_type *t );

int	tfA_get( tfA_type *tfA, int iFrame, float a[], int *nA );


tf_type *	tfA_get( tfA_type *tfA, int iFrame );

tf_type *	tfA_get_prev( tfA_type *tfA, int iFrame );

tf_type *	tfA_get_next( tfA_type *atf, int iFrame );



tfA_type *	tfA_bluar( tfA_type *atf, int n );

tf_type *	tfA_band( tfA_type *atf, int n );

int		tfA_findByName( tfA_type *aw, char *name );

	// TfFrequency.cpp
void	tf_frequency( tf_type *tf, double *freq, double *amp );

void	tf_frequency_test( double f, double a, int N );

int		tf_frequencyA( tf_type *tf, double *freq, double *amp );


tf_type *	tf_axb( tf_type *stf, double a, double b, tf_type *tf );

int	tf_app_axb( tf_type *tf, double *a, double *b );



	// TfMean.cpp
int	tfA_mean( tfA_type *tfA, float T, float *av, float *var, int aI[] );



//#ifdef __cplusplus
//}
//#endif


#endif

