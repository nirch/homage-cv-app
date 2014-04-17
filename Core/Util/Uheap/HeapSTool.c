/***********************
 ***	HeapTool.c   ***
 ***********************/


#include	<stdlib.h>

#include	"Uigp/igp.h"

#include	"HeapSType.h"




heapS_type *	
heapS_alloc( int num, int size )
{
heapS_type	*hs;

	hs = (heapS_type *)malloc( sizeof( heapS_type ) );

	hs->size = size;
	hs->no = num;



	hs->data = (char *)malloc( hs->size * hs->no );

	hs->p = hs->data;

	return( hs );
}


void
heapS_destroy( heapS_type *mp )
{
	free( mp->data );

	free( mp );
}


void
heapS_rewind( heapS_type *mp )
{
	mp->p = mp->data;

}


void
heapS_set_front( heapS_type *hs, void *p )
{
	hs->p = p;
}