/***********************
 ***	HeapTool.c   ***
 ***********************/


#include	<stdlib.h>

#include	"Uigp/igp.h"

#include	"HeapType.h"




heap_type *	
heap_alloc( int size )
{
heap_type	*mp;
char	**p;

	mp = (heap_type *)malloc( sizeof( heap_type ) );

	mp->size = size;

	mp->data = mp->data0 = (char *)malloc( mp->size + 8 );


	p = (char **)mp->data;
	p[0] = NULL;
	mp->p = mp->data + 4;

	return( mp );
}


void
heap_destroy( heap_type *mp )
{
char	**p;

	while( mp->data0 != NULL ){
		p = (char **)mp->data0;
		mp->data0 = p[0];
		free( p );
	}

	free( mp );
}


void
heap_clear( heap_type *mp )
{
	mp->data = mp->data0;

	mp->p = mp->data + 4;

}


void
heap_extand( heap_type *mp, int size )
{
char	*data,	**p;

	if( mp->p - mp->data + 4  > size )
		return;


	data = (char *)malloc( mp->size + 4 );
	p = (char **)mp->data;
	p[0] = data;

	mp->data = data;

	p = (char **)mp->data;
	p[0] = NULL;
	mp->p = mp->data + 4;
}

