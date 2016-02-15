/******************
 ***   Tf.cpp   ***
 ******************/
#include	<math.h>
#include	<string.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"
#include	"TfType.h"


#define TF_VERSION 1


static tf_type *	tf_read( FILE *fp );




tf_type *
tf_alloc( int n )
{
tf_type *tf;

	tf = (tf_type*)malloc( sizeof(tf_type) );
	tf->NA = n;
	tf->a = ( float *)malloc( tf->NA*sizeof(float) );

	tf->nA = n;
	strcpy( tf->name, "none" );
	tf->id = 0;

	return( tf );
}

tf_type *
tf_copy( tf_type *stf, tf_type *tf )
{
int	i;

	if( tf == NULL )
		tf = tf_alloc( stf->nA );

	for( i = 0 ; i < stf->nA ; i++ )
		tf->a[i] = stf->a[i];
	tf->nA = stf->nA;

	strcpy( tf->name, stf->name );
	tf->id = stf->id;
	tf->iFrame = stf->iFrame;

	return( tf );
}





void
tf_destroy( tf_type *tf )
{
	free( tf->a );
	free( tf );
}

void
tf_clear( tf_type *t )
{
	int	i;

	t->id = 0;

	for( i = 0; i < t->NA ; i++ )
		t->a[i] = 0;
}

void
	tf_set( tf_type *t, float val )
{
	int	i;

	t->id = 0;
	t->name[0] = 0;
	t->iFrame = 0;

	for( i = 0; i < t->nA ; i++ )
		t->a[i] = val;
}


tf_type *
tf_create( int iFrame, float a[], int nA )
{
int	i;

	tf_type *t = tf_alloc( nA );


	strcpy( t->name, "none" );

	t->iFrame = iFrame;


	t->nA = nA;
	for( i = 0; i < t->nA ; i++ )
		t->a[i] = a[i];


	return( t );
}


tf_type *
tf_append( tf_type *t0, tf_type *t1 )
{
int	i;

	tf_type *t = tf_alloc( t0->nA + t1->nA );


	strcpy( t->name, t0->name );

	t->iFrame = t0->iFrame;


	t->nA = 0;
	for( i = 0; i < t0->nA ; i++ )
		t->a[t->nA++] = t0->a[i];

	for( i = 0; i < t1->nA ; i++ )
		t->a[t->nA++] = t1->a[i];

	return( t );
}


void
tfA_axb( tfA_type *at, float a, float b )
{
	int	i;

	for( i = 0; i < at->nA ; i++ )
		tf_axb( at->a[i], a, b );
}

void
tf_axb( tf_type *t, float a, float b )
{
	int	i;

	for( i = 0; i < t->nA ; i++ )
		t->a[i] = a* t->a[i] + b;;
}


void
tf_avarge( tf_type *t, float *av, float *var )
{
	int	i;

	*av = *var = 0;
	for( i = 0; i < t->nA ; i++ ){
		*av += t->a[i];
		*var += t->a[i]*t->a[i];
	}

	*av /= t->nA;

	*var = *var / t->nA - *av * *av;
}

void 
	tf_accumulate( tf_type *st, tf_type *tf, float a )
{
	int	i;

	for( i = 0 ; i < st->nA ; i++ ){
		st->a[i] += tf->a[i] * a;
	}
}


tfA_type *
tfA_alloc( int no )
{
tfA_type *atf;
int	i;

	atf = (tfA_type*)malloc( sizeof(tfA_type) );

	atf->NA = no;
	atf->a = (tf_type **)malloc( atf->NA * sizeof(tf_type *) );

	for( i = 0 ; i < atf->NA ; i++ )
		atf->a[i] = NULL;

	atf->nA = 0;
	return( atf );
}


void 
tfA_destroy( tfA_type *atf )
{	
int	i;

	for( i = 0 ; i < atf->nA ; i++ ){
		if( atf->a[i] != NULL ){
			tf_destroy( atf->a[i] );
			atf->a[i] = NULL;
		}
	}

	free( atf->a );
	free( atf );
}



tfA_type *
tfA_create( int nT, int nA )
{
	int	i;

	tfA_type *atf = tfA_alloc( nT );

	for( i = 0 ; i < atf->NA ; i++ ){
		tf_type *tf =tf_alloc( nA );
		tf_clear( tf );

		tfA_add( atf, i, tf );
	}

	return( atf );
}


void 
tfA_clear( tfA_type *atf )
{
	int	i;

	for( i = 0 ; i < atf->nA ; i++ ){
		if( atf->a[i] != NULL ){
			tf_clear( atf->a[i] );
		}
	}
}


void 
tfA_copy_name( tfA_type *atf, tfA_type *stf )
{
	int	i;

	for( i = 0 ; i < atf->nA ; i++ ){
		strcpy( atf->a[i]->name, stf->a[i]->name );
	}
}



tfA_type *
	tfA_copy( tfA_type *as, tfA_type *at )
{
	int	i;

	if( at == NULL )
		at = tfA_alloc( as->nA );

	for( i = 0 ; i < as->nA ; i++ )
		at->a[at->nA++] = tf_copy( as->a[i], NULL );

	return( at );
}






void
tfA_set( tfA_type *tfA, int iFrame, float a[], int nA )
{
int	i;
	if( tfA->a[iFrame] == NULL )
		tfA->a[iFrame] = tf_alloc( nA );

	tf_type	*t = tfA->a[iFrame];

	strcpy( t->name, "none" );

	t->iFrame = iFrame;


	t->nA = nA;
	for( i = 0; i < t->nA ; i++ )
		t->a[i] = a[i];


	if( tfA->nA <= t->iFrame )
		tfA->nA = t->iFrame+1;
}

void
tfA_add( tfA_type *atf, int iFrame, tf_type *t )
{
int	i;

	if( iFrame < 0 )	iFrame = atf->nA;


	if( iFrame >= atf->NA ){

		int NA = atf->NA + iFrame+32;
		atf->a = ( tf_type **)realloc( atf->a, NA*sizeof(tf_type *) );

		for( i = atf->NA ; i <  NA ; i++ )
			atf->a[i] = NULL;

		atf->NA = NA;
	}



	if( iFrame < atf->nA && atf->a[iFrame] != NULL )
		free( atf->a[iFrame] );


	atf->a[iFrame] = t;


	t->iFrame = iFrame;


	if( atf->nA <= t->iFrame )
		atf->nA = t->iFrame+1;
}


void
tfA_append( tfA_type *atf, tfA_type *tatf  )
{
	int	i;


	for( i = 0 ; i < atf->nA ; i++ ){
		tf_type *tf = tfA_get( atf, i );
		if( tf == NULL )	continue;
		tfA_add( tatf, tf->iFrame, tf );
	}

	atf->nA = 0;

	tfA_destroy( atf );
}



int
tfA_get( tfA_type *tfA, int iFrame, float a[], int *nA )
{
	int	i;
	if( tfA->a[iFrame] == NULL )
		return( -1 );

	tf_type	*t = tfA->a[iFrame];


	*nA = t->nA;
	for( i = 0; i < t->nA ; i++ )
		a[i] = t->a[i];

	return( 1 );
}


tf_type *
tfA_get( tfA_type *tfA, int iFrame )
{
	if( tfA == NULL || iFrame < 0 || iFrame >= tfA->nA )
		return ( NULL );

	return ( tfA->a[iFrame] );
}

tf_type *
tfA_get_prev( tfA_type *atf, int iFrame )
{
int	i;

	if( iFrame > atf->nA )
		iFrame = atf->nA;

	for( i = iFrame-1 ; i >= 0 ; i-- ){
		if( atf->a[i] != NULL )
			return( atf->a[i] );
	}

	return( NULL );
}


tf_type *
	tfA_get_next( tfA_type *atf, int iFrame )
{
	int	i;

	if( iFrame >= atf->nA )
		return( NULL );

	for( i = iFrame+1 ; i < atf->nA ; i++ ){
		if( atf->a[i] != NULL )
			return( atf->a[i] );
	}

	return( NULL );
}


tf_type *
tfA_average( tfA_type *tfA )
{
	int	i,	k,	n;

	tf_type	*atf = NULL;





	for( i = 0 ; i < tfA->nA ; i++ ){
		tf_type *tf = tfA->a[i];
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


int
	tfA_var( tfA_type *at, int id, tf_type **av, tf_type **var )
{
	int	i,	k;
	tf_type *tf;

	for( i = 0  ; i < at->nA ; i++ ){
		tf = at->a[i];
		if( tf == NULL )	continue;
		if( id >= 0 && tf->id != id )	continue;

		break;
	}

	if( i >= at->nA )	return( -1 );

	


	*av = tf_alloc( tf->nA );
	tf_set( *av, 0 );
	(*av)->iFrame = id;
	(*av)->id = id;
	strcpy( (*av)->name, tf->name );


	*var = tf_alloc( tf->nA );
	tf_set( *var, 0 );
	(*var)->iFrame = id;
	(*var)->id = id;
	strcpy( (*var)->name, tf->name );


	tf_type *na = tf_alloc( tf->nA );
	tf_set( na, 0 );

	int	n;
	for( i = 0, n = 0  ; i < at->nA ; i++ ){
		tf_type *tf = at->a[i];
		if( id >= 0 && tf->id != id )	continue;

		for( k = 0 ; k < tf->nA ; k++ ){
			if( tf->a[k] == -1000 )
				continue;

			(*av)->a[k] += tf->a[k];
			(*var)->a[k] += tf->a[k]*tf->a[k];

			float a = na->a[k];
			na->a[k] += 1;
		}

		n++;
	}

	if( n == 0 )
		return( -1 );

	for( k = 0 ; k < na->nA ; k++ ){

		(*av)->a[k] /= na->a[k];
		(*var)->a[k] = (*var)->a[k] / na->a[k] - (*av)->a[k]*(*av)->a[k];

		(*var)->a[k] = ((*var)->a[k] < 0 )? 0 : sqrt( (*var)->a[k] );
	}

	if( id < 0 ){
		strcpy( (*var)->name, "all" );
		strcpy( (*av)->name, "all" );

	}

	return( n );
}



tfA_type *
tfA_transform( tfA_type *stfA, matrix_type *m )
{
	int	i;
	float	a[64];


	tfA_type *tfA = tfA_alloc( stfA->nA );


	for( i = 0 ; i < stfA->nA ; i++ ){
		tf_type *tf = stfA->a[i];
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



tfA_type *
	tfA_bluar( tfA_type *atf, int n )
{
	int	i,	k,	no;

	tfA_type *atfb = tfA_alloc( atf->nA );

	for( i = 0 ; i  < atf->nA ; i++ ){
		if( atf->a[i] == NULL )	continue;

		tf_type * tf = tf_alloc( atf->a[i]->nA );

		tf_clear( tf );

		for( k = i - n, no = 0 ;  k <= i + n ; k++ ){
			if( k < 0 || k >= atf->nA )	continue;
			if( atf->a[k] == NULL )	continue;

			tf_accumulate( tf, atf->a[k], 1 );
			no++;
		}

		tf_axb( tf, 1.0/no, 0 );

		tfA_add( atfb, i, tf );
	}

	return( atfb );
}


tf_type *
tfA_band( tfA_type *atf, int n )
{
	int	i;

	tf_type *tf = tf_alloc( atf->nA );

	tf->nA = 0;
	for( i = 0 ; i  < atf->nA ; i++ ){
		if( atf->a[i] == NULL )	continue;
		
		tf->a[tf->nA++] = atf->a[i]->a[n];
	}

	return( tf );
}




int	tfA_findByName( tfA_type *aw, char *name )
{
	int	i;



	for( i = 1 ; i < aw->nA ; i++ ){

		if( gp_stricmp( name, aw->a[i]->name ) == 0 )
			return( i );
	}

	return( -1 );
}



int
	tfA_absdif( tfA_type *atf, tfA_type *atf1 )
{
	int	i;

	float aMax = 0;
	for( i = 0 ; i < atf->nA ; i++ ){
		tf_type *tf = tfA_get( atf, i );
		if( tf == NULL )	continue;


		tf_type *tf1 = tfA_get( atf1, i );
		if( tf1 == NULL )	continue;

		float a = tf_absdiff( tf, tf1 );

		if( a > aMax )	aMax = a;
	}

	return( aMax );
}


float
	tf_absdiff( tf_type *stf, tf_type *tf )
{
	int	i;

	float	aMax = 0;

	for( i = 0 ; i < stf->nA ; i++ ){
		float a = tf->a[i] - stf->a[i];
		if( a < 0 )	a = -a;

		if( a > aMax )
			aMax = a;

	}

	return( aMax );
}



tfB_type *
tfB_alloc( int no )
{
	tfB_type *btf;
	int	i;

	btf = (tfB_type*)malloc( sizeof(tfB_type) );

	btf->NA = no;
	btf->a = (tfA_type **)malloc( btf->NA * sizeof(tfA_type *) );

	for( i = 0 ; i < btf->NA ; i++ )
		btf->a[i] = NULL;

	btf->nA = 0;
	return( btf );
}


void 
tfB_destroy( tfB_type *btf )
{	
	int	i;

	for( i = 0 ; i < btf->nA ; i++ ){
		if( btf->a[i] != NULL ){
			tfA_destroy( btf->a[i] );
		}
	}

	free( btf->a );
	free( btf );
}