/******************
 ***   Tf.cpp   ***
 ******************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"
#include	"FloatType.h"


#include "Umath/St/StType.h"


floatA_type *
floatA_alloc( int no )
{
floatA_type *aS;


	aS = (floatA_type*)malloc( sizeof(floatA_type) );

	aS->NA = no;
	aS->a = (float *)malloc( aS->NA * sizeof(float) );


	aS->nA = 0;
	return( aS );
}



floatA_type *
floatA_realloc( floatA_type *ai, int no )
{
	if( ai == NULL ){
		ai = floatA_alloc( no );
		return( ai );
	}

	if( ai->NA < no ){
		ai->NA = no;
		ai->a = (float *)realloc( ai->a, ai->NA*sizeof(float) );
	}

	return( ai );
}



void 
floatA_destroy( floatA_type *aS )
{	

	free( aS->a );
	free( aS );
}


floatA_type * 
floatA_copy( floatA_type *as, floatA_type *ai )
{
int	i;

	ai = floatA_realloc( ai, as->nA );

	for( i = 0 ; i < as->nA ; i++ )
		ai->a[i] = as->a[i];

	ai->nA = as->nA;

	return( ai );
}


void 
floatA_add( floatA_type *as, int val )
{

	if( as->nA >= as->NA ){
		as->NA += 4;
		as->a = (float *)realloc( as->a, as->NA*sizeof(float) );
	}

	as->a[as->nA++] = val;
}

void 
floatA_add( floatA_type *tai, floatA_type *ai )
{
int	i;

	for( i = 0 ; i < ai->nA ; i++ )
		if( floatA_index( tai, ai->a[i]) < 0 )
			floatA_add( tai, ai->a[i] );
}


void 
floatA_add( floatA_type *as, int val[], int nVal )
{
int	i;
	if( as->nA + nVal >= as->NA ){
		as->NA = as->nA + nVal + 4;
		as->a = (float *)realloc( as->a, as->NA*sizeof(int) );
	}

	for( i = 0 ; i < nVal ; i++ )
		as->a[as->nA++] = val[i];
}

void 
floatA_cycle( floatA_type *as, int n )
{
	int	i;

	if( as->nA < n )
		as->nA++;

	for( i = as->nA-1 ; i >0 ; i-- )
		as->a[i] = as->a[i-1];

}


void 
	floatA_st( floatA_type *as, st_type *st )
{
int	i;

	st_init( st );

	for( i = 0 ; i < as->nA ; i++ )
		st_add( st, as->a[i] );


	st_final( st );
}

float 
floatA_average( floatA_type *as )
{
int	i;

	float sum = 0;

	for( i = 0 ; i < as->nA ; i++ )
		sum += as->a[i];

	sum /= as->nA;

	return( sum );
}

int
floatA_write( floatA_type *ai, char *file )
{
FILE *fp;

	if( ( fp = fopen( file, "wb")) == NULL )
		return( -1 );

	floatA_write( ai, fp );

	fclose( fp );
	return( 1 );
}


void
floatA_write( floatA_type *ai, FILE *fp )
{
	int	i;

	fprintf( fp, " %d", ai->nA );

	for( i = 0 ; i < ai->nA ; i++ )
		fprintf( fp, " %f", ai->a[i] );


	fprintf( fp, "\n" );
}


void
floatA_writeb( floatA_type *ai, FILE *fp )
{
int	i;

	fwrite( &ai->nA, 4, 1, fp );

	for( i = 0 ; i < ai->nA ; i++ )
		fwrite( &ai->a[i], 4, 1, fp );
}

void
floatA_readb( floatA_type **ai, FILE *fp )
{
	int	i,	nA;

	fread( &nA, 4, 1, fp );

	*ai = floatA_alloc( nA ); 

	for( i = 0 ; i < nA ; i++ )
		fread( &(*ai)->a[i], 4, 1, fp );

	(*ai)->nA = nA;


}

int
floatA_read( floatA_type **ai, char *file )
{
	FILE *fp;

	if( ( fp = fopen( file, "rb")) == NULL )
		return( -1 );

	floatA_read( ai, fp );

	fclose( fp );
	return( 1 );
}



int
floatA_read( floatA_type **ai, FILE *fp )
{
	int	i,	nA;

	fscanf( fp, " %d", &nA );


	*ai = floatA_alloc( nA ); 

	for( i = 0 ; i < nA ; i++ )
		fscanf( fp, " %f", &(*ai)->a[i] );

	(*ai)->nA = nA;
	return( 1 );
}




int
floatA_index( floatA_type *ai, int val )
{
	int	i;

	for( i = 0 ; i < ai->nA ; i++ ){
		if( ai->a[i] == val )
			return( i );
	}

	return( -1 );
}


