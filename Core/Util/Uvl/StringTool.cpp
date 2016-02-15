/******************
 ***   Tf.cpp   ***
 ******************/
#include	<math.h>
#include <string.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix.h"
#include	"StringType.h"





stringA_type *
stringA_alloc( int no )
{
stringA_type *aS;


	aS = (stringA_type*)malloc( sizeof(stringA_type) );

	aS->NA = no;
	aS->a = (char **)malloc( aS->NA * sizeof(char *) );


	aS->nA = 0;
	return( aS );
}



stringA_type *
stringA_realloc( stringA_type *ai, int no )
{
	if( ai == NULL ){
		ai = stringA_alloc( no );
		return( ai );
	}

	if( ai->NA < no ){
		ai->NA = no;
		ai->a = (char **)realloc( ai->a, ai->NA*sizeof(char *) );
	}

	return( ai );
}



void 
stringA_destroy( stringA_type *aS )
{	

	free( aS->a );
	free( aS );
}


stringA_type * 
	stringA_create( char *a[], int nA )
{
	int	i;

	stringA_type *as = stringA_alloc( nA );

	for( i = 0 ; i < nA ; i++ )
		as->a[as->nA++] = strdup( a[i] );

	return( as );
}

stringA_type * 
	stringA_copy( stringA_type *as, stringA_type *ai )
{
	int	i;
	if( as == NULL )
		return( NULL );

	ai = stringA_realloc( ai, as->nA );

	for( i = 0 ; i < as->nA ; i++ )
		ai->a[i] = as->a[i];

	ai->nA = as->nA;

	return( ai );
}


void 
stringA_add( stringA_type *as, char *s, int fCopy )
{

	if( as->nA >= as->NA ){
		as->NA += 4;
		as->a = (char **)realloc( as->a, as->NA*sizeof(char *) );
	}

	if( fCopy == 1 )
		s = strdup( s );

	as->a[as->nA++] = s;
}

void 
stringA_add( stringA_type *tai, stringA_type *ai )
{
int	i;

	for( i = 0 ; i < ai->nA ; i++ )
		if( stringA_index( tai, ai->a[i]) < 0 )
			stringA_add( tai, ai->a[i], 1 );
}


void 
stringA_add( stringA_type *as, char *val[], int nVal )
{
int	i;
	if( as->nA + nVal >= as->NA ){
		as->NA = as->nA + nVal + 4;
		as->a = (char **)realloc( as->a, as->NA*sizeof(char *) );
	}

	for( i = 0 ; i < nVal ; i++ )
		as->a[as->nA++] = val[i];
}


int
stringA_write( stringA_type *ai, char *file )
{
FILE *fp;

	if( ( fp = fopen( file, "wb")) == NULL )
		return( -1 );

	stringA_write( ai, fp );

	fclose( fp );
	return( 1 );
}


void
stringA_write( stringA_type *ai, FILE *fp )
{
	int	i;

	fprintf( fp, " %d :", ai->nA );

	for( i = 0 ; i < ai->nA ; i++ )
		fprintf( fp, " %s\n", ai->a[i] );


	fprintf( fp, "\n" );
}





int
stringA_read( stringA_type **ai, char *file )
{
	FILE *fp;

	if( ( fp = fopen( file, "rb")) == NULL )
		return( -1 );

	stringA_read( ai, fp );

	fclose( fp );
	return( 1 );
}



int
stringA_read( stringA_type **ai, FILE *fp )
{
	int	i,	nA;

	fscanf( fp, " %d", &nA );


	*ai = stringA_alloc( nA ); 

	char	buf[1024];
	for( i = 0 ; i < nA ; i++ ){
		fgets( buf, 1024, fp );
		(*ai)->a[i] = strdup( buf );
	}

	(*ai)->nA = nA;
	return( 1 );
}




int
stringA_index( stringA_type *ai, char *val )
{
	int	i;

	for( i = 0 ; i < ai->nA ; i++ ){
		if( strcmp( ai->a[i], val) == 0  )
			return( i );
	}

	return( -1 );
}


void
stringA_flip( stringA_type *b )
{
int	i0,	i1;

	for( i0 = 0, i1 = b->nA-1 ; i0 < i1 ; i0++, i1-- ){
		char *tmp = b->a[i0];
		b->a[i0] = b->a[i1];

		b->a[i1] = tmp;
	}
}