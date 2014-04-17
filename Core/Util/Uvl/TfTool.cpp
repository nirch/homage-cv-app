/******************
 ***   Tf.cpp   ***
 ******************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"
#include	"TfType.h"


static int	tf_write( tf_type *s, FILE *fp );

static tf_type *	tf_read( FILE *fp );




tf_type *
tf_alloc( int nT )
{
tf_type *tf;

	tf = (tf_type*)malloc( sizeof(tf_type) );

	return( tf );
}


void
tf_destroy( tf_type *tf )
{
	free( tf );
}



tfA_type *
tfA_alloc( int no )
{
tfA_type *aS;
int	i;

	aS = (tfA_type*)malloc( sizeof(tfA_type) );

	aS->NT = no;
	aS->t = (tf_type **)malloc( aS->NT * sizeof(tf_type *) );

	for( i = 0 ; i < aS->NT ; i++ )
		aS->t[i] = NULL;

	aS->nT = 0;
	return( aS );
}


void 
tfA_destroy( tfA_type *aS )
{	
	tfA_clear( aS );

	free( aS->t );
	free( aS );
}


void 
tfA_clear( tfA_type *aS )
{
	int	i;

	for( i = 0 ; i < aS->nT ; i++ ){
		if( aS->t[i] != NULL ){
			tf_destroy( aS->t[i] );
			aS->t[i] = NULL;
		}
	}
}




int
tfA_write( tfA_type *aS, char *file )
{
FILE	*fp;
int	i;

	gp_filename_force_extension( file, ".tf" );

	if(  (fp = fopen( file, "wb" )) == NULL )
		return( -1 );

	fprintf( fp, "%d\n", aS->nT );


	for( i = 0 ; i < aS->nT ; i++ ){
		if(  aS->t[i] == NULL )	continue;

		tf_write( aS->t[i], fp );
	}


	fclose( fp );

	return ( 1 );
}


static int
tf_write( tf_type *s, FILE *fp )
{
int	i;


	fprintf( fp, "%d\t%d", s->iFrame, s->nA );


	for( i = 0 ; i < s->nA ; i++ )
		fprintf( fp, "\t%f", s->a[i] );

	fprintf( fp, "\n" );

	return ( 1 );
}




tfA_type *
tfA_read( char *file )
{
tfA_type *aS;
tf_type *s;
FILE	*fp;
int	nS;

	gp_filename_force_extension( file, ".tf" );

	if(  (fp = fopen( file, "rb" ) ) == NULL )
		return( NULL );


	fscanf( fp, "%d", &nS );

	aS = tfA_alloc( nS );



	while( (s = tf_read( fp )) > 0 ){
		aS->t[ s->iFrame ] = s;

		aS->nT = s->iFrame + 1;
	}


	fclose( fp );

	return( aS );
}


static tf_type *
tf_read( FILE *fp )
{
tf_type *s;
int	i;
int	iFrame,	nP;


	if( fscanf( fp, "%d   %d", &iFrame, &nP ) == EOF )
		return( NULL );

	s = tf_alloc( 0 );
	s->iFrame = iFrame;
	s->nA = nP;


	for( i = 0 ; i < s->nA ; i++ )
		fscanf( fp, "%f", &s->a[i] );

	return ( s );
}



void
tfA_set( tfA_type *tfA, int iFrame, float a[], int nA )
{
int	i;
	if( tfA->t[iFrame] == NULL )
		tfA->t[iFrame] = tf_alloc( nA );

	tf_type	*t = tfA->t[iFrame];

	t->iFrame = iFrame;


	t->nA = nA;
	for( i = 0; i < t->nA ; i++ )
		t->a[i] = a[i];


	if( tfA->nT <= t->iFrame )
		tfA->nT = t->iFrame+1;
}

int
tfA_get( tfA_type *tfA, int iFrame, float a[], int *nA )
{
	int	i;
	if( tfA->t[iFrame] == NULL )
		return( -1 );

	tf_type	*t = tfA->t[iFrame];


	*nA = t->nA;
	for( i = 0; i < t->nA ; i++ )
		a[i] = t->a[i];

	return( 1 );
}


tf_type *
tfA_average( tfA_type *tfA )
{
	int	i,	k,	n;

	tf_type	*atf = NULL;



	for( i = 0 ; i < tfA->nT ; i++ ){
		tf_type *tf = tfA->t[i];
		if( tf == NULL )	continue;


		if( atf == NULL ){
			atf = tf_alloc( 0 );
			atf->nA = tf->nA;
			for( k = 0 ; k < atf->nA ; k++ )
				atf->a[k] = 0;

			n = 0;
		}



		for( k = 0 ; k < atf->nA ; k++ )
			atf->a[k] += tf->a[k];


		n++;


	}


	for( k = 0 ; k < atf->nA ; k++ )
		atf->a[k] /= n;

	return( atf );
}



tfA_type *
tfA_transform( tfA_type *stfA, matrix_type *m )
{
	int	i;
	float	a[64];


	tfA_type *tfA = tfA_alloc( stfA->nT );


	for( i = 0 ; i < stfA->nT ; i++ ){
		tf_type *tf = stfA->t[i];
		if( tf == NULL )	continue;


		matrixD_multVF( m, tf->a, a );

		tfA_set( tfA, tf->iFrame, a, tf->nA );
	}


	return( tfA );
}


int
tfA_nearest( tf_type *tf, float a0, float T )
{
	int	i,	iMin;
	float	d,	dMin;


	iMin = -1;
	dMin = 0;
	for( i = 0 ; i < tf->nA ; i++ ){

		d = tf->a[i] - a0;
		if( d < 0 )	d = -d;

		if( iMin < 0 || d < dMin ){
			iMin = i;
			dMin = d;
		}
	}

	if( dMin > T )	return( -1 );

	return( iMin );
}