/******************
 ***   Tf.cpp   ***
 ******************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"
#include	"IntType.h"





intA_type *
intA_alloc( int no )
{
intA_type *aS;


	aS = (intA_type*)malloc( sizeof(intA_type) );

	aS->NA = no;
	aS->a = (int *)malloc( aS->NA * sizeof(int) );


	aS->nA = 0;
	return( aS );
}



intA_type *
intA_realloc( intA_type *ai, int no )
{
	if( ai == NULL ){
		ai = intA_alloc( no );
		return( ai );
	}

	if( ai->NA < no ){
		ai->NA = no;
		ai->a = (int *)realloc( ai->a, ai->NA*sizeof(int) );
	}

	return( ai );
}



void 
intA_destroy( intA_type *aS )
{	

	free( aS->a );
	free( aS );
}


intA_type * 
intA_copy( intA_type *as, intA_type *ai )
{
int	i;

	ai = intA_realloc( ai, as->nA );

	for( i = 0 ; i < as->nA ; i++ )
		ai->a[i] = as->a[i];

	ai->nA = as->nA;

	return( ai );
}


void
intA_set( intA_type *a, int val )
{
	int	i;

	for( i = 0 ; i < a->nA ; i++ )
		a->a[i] = val;

}


int
intA_sum( intA_type *a )
{
	int	i,	sum;

	for( i = 0, sum = 0 ; i < a->nA ; i++ )
		sum += a->a[i];

	return( sum );
}


void 
intA_add( intA_type *as, int val )
{

	if( as->nA >= as->NA ){
		as->NA += 4;
		as->a = (int *)realloc( as->a, as->NA*sizeof(int) );
	}

	as->a[as->nA++] = val;
}

void 
intA_add( intA_type *tai, intA_type *ai )
{
int	i;

	for( i = 0 ; i < ai->nA ; i++ )
		if( intA_index( tai, ai->a[i]) < 0 )
			intA_add( tai, ai->a[i] );
}


void 
intA_add( intA_type *as, int val[], int nVal )
{
int	i;
	if( as->nA + nVal >= as->NA ){
		as->NA = as->nA + nVal + 4;
		as->a = (int *)realloc( as->a, as->NA*sizeof(int) );
	}

	for( i = 0 ; i < nVal ; i++ )
		as->a[as->nA++] = val[i];
}


int
	intA_write_index( intA_type *ai, char *file )
{
	FILE *fp;
	int	i;

	if( ( fp = fopen( file, "wb")) == NULL )
		return( -1 );
		
	fprintf( fp, " %d \n", ai->nA );

	for( i = 0 ; i < ai->nA ; i++ )
		fprintf( fp, "%3d  %d\n", i, ai->a[i] );


	fprintf( fp, "\n" );

	fclose( fp );
	return( 1 );
}


void
intA_dump( intA_type *ai, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".txt", file ) < 0 )
		return;

	intA_write_index( ai, file );
}


int
intA_write( intA_type *ai, char *file )
{
FILE *fp;

	if( ( fp = fopen( file, "wb")) == NULL )
		return( -1 );

	intA_write( ai, fp );

	fclose( fp );
	return( 1 );
}


void
intA_write( intA_type *ai, FILE *fp )
{
	int	i;

	fprintf( fp, " %d :", ai->nA );

	for( i = 0 ; i < ai->nA ; i++ )
		fprintf( fp, " %d", ai->a[i] );


	fprintf( fp, "\n" );
}


void
intA_writeb( intA_type *ai, FILE *fp )
{
int	i;

	fwrite( &ai->nA, 4, 1, fp );

	for( i = 0 ; i < ai->nA ; i++ )
		fwrite( &ai->a[i], 4, 1, fp );
}

void
intA_readb( intA_type **ai, FILE *fp )
{
	int	i,	nA;

	fread( &nA, 4, 1, fp );

	*ai = intA_alloc( nA ); 

	for( i = 0 ; i < nA ; i++ )
		fread( &(*ai)->a[i], 4, 1, fp );

	(*ai)->nA = nA;


}

int
intA_read( intA_type **ai, char *file )
{
	FILE *fp;

	if( ( fp = fopen( file, "rb")) == NULL )
		return( -1 );

	intA_read( ai, fp );

	fclose( fp );
	return( 1 );
}



int
intA_read( intA_type **ai, FILE *fp )
{
	int	i,	nA;

	fscanf( fp, " %d", &nA );


	*ai = intA_alloc( nA ); 

	for( i = 0 ; i < nA ; i++ )
		fscanf( fp, " %d", &(*ai)->a[i] );

	(*ai)->nA = nA;
	return( 1 );
}




int
intA_index( intA_type *ai, int val )
{
	int	i;

	for( i = 0 ; i < ai->nA ; i++ ){
		if( ai->a[i] == val )
			return( i );
	}

	return( -1 );
}


void
intA_flip( intA_type *b )
{
int	i0,	i1;

	for( i0 = 0, i1 = b->nA-1 ; i0 < i1 ; i0++, i1-- ){
		int tmp = b->a[i0];
		b->a[i0] = b->a[i1];

		b->a[i1] = tmp;
	}
}