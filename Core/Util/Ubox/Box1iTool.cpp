#include	<stdio.h>


#include	"Uigp/igp.h"
#include	"Uigp/GpMemoryLeak.h"

#include	"Box1i.h"


box1iA_type *
box1iA_alloc( int n )
{
	box1iA_type	*avl;


	avl = (box1iA_type *)malloc( sizeof( box1iA_type) );

	avl->NA = n;
	avl->a = ( box1i_type *)malloc( avl->NA*sizeof(box1i_type) );

	avl->nA = 0;

	GPMEMORY_LEAK_ALLOC( avl );

	return( avl );
}


box1iA_type *
box1iA_realloc( box1iA_type *avl, int n )
{
	if( avl == NULL ){
		avl = box1iA_alloc(n );
		return( avl );
	}


	if( avl->NA < n ){
		avl->a = ( box1i_type *)realloc( avl->a, n*sizeof(box1i_type) );
		avl->NA = n;
	}

	return( avl );
}


box1iA_type *
box1iA_copy( box1iA_type *bavl, box1iA_type *avl )
{

	avl = box1iA_realloc( avl, bavl->nA );

	int i;
	for( i = 0 ; i < bavl->nA ; i++ )
		avl->a[i] = bavl->a[i];

	avl->nA = bavl->nA;

	return( avl );
}


void
box1iA_destroy( box1iA_type *avl )
{
	GPMEMORY_LEAK_DESTROY( avl );

	free( avl->a );

	free( avl );
}



box1fA_type *
	box1fA_alloc( int n )
{
	box1fA_type	*avl;


	avl = (box1fA_type *)malloc( sizeof( box1fA_type) );

	avl->NA = n;
	avl->a = ( box1f_type *)malloc( avl->NA*sizeof(box1f_type) );

	avl->nA = 0;

	GPMEMORY_LEAK_ALLOC( avl );

	return( avl );
}


box1fA_type *
	box1fA_realloc( box1fA_type *avl, int n )
{
	if( avl == NULL ){
		avl = box1fA_alloc(n );
		return( avl );
	}


	if( avl->NA < n ){
		avl->a = ( box1f_type *)realloc( avl->a, n*sizeof(box1f_type) );
		avl->NA = n;
	}

	return( avl );
}


box1fA_type *
	box1iA_copy( box1fA_type *bavl, box1fA_type *avl )
{

	avl = box1fA_realloc( avl, bavl->nA );

	int i;
	for( i = 0 ; i < bavl->nA ; i++ )
		avl->a[i] = bavl->a[i];

	avl->nA = bavl->nA;

	return( avl );
}


void
	box1fA_destroy( box1fA_type *avl )
{
	GPMEMORY_LEAK_DESTROY( avl );

	free( avl->a );

	free( avl );
}


void
box1i_print(  char *title, box1i_type ab[], int nP, FILE *fp )
{
	int	i;

	fprintf( fp, "%s  %d\n", title, nP );

	for( i = 0 ; i < nP ; i++ ){
		fprintf( fp, "%d   %d  %d\n", i, ab[i].i0, ab[i].i1 );
	}
}


void
	box1iA_write(  char *title, box1iA_type *ab, FILE *fp )
{
	int	i;

	fprintf( fp, "%s  %d\n", title, ab->nA );

	for( i = 0 ; i < ab->nA ; i++ ){
		fprintf( fp, "%d   %d  %d\n", i, ab->a[i].i0, ab->a[i].i1 );
	}
}

int
box1i_overlap( box1i_type *b, box1i_type *b1, box1i_type *b2 )
{
	if( b->i0 < b1->i0 ){

		if( b->i1 < b1->i0 )
			return( b->i1 - b1->i0 );

		if( b->i1 > b1->i1 ){
			b2->i0 = b1->i0;
			b2->i1 = b1->i1;
			return( b2->i1 - b2->i0 );
		}

		b2->i0 = b1->i0;
		b2->i1 = b->i1;
		return( b2->i1 - b2->i0 );
	}


	if( b1->i1 < b->i0 )
		return( b1->i1 - b->i0 );

	if( b1->i1 > b->i1 ){
		b2->i0 = b->i0;
		b2->i1 = b->i1;
		return( b2->i1 - b2->i0 );
	}

	b2->i0 = b->i0;
	b2->i1 = b1->i1;
	return( b2->i1 - b2->i0 );
}