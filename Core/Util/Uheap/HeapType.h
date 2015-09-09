/***********************
 ***	HeapType.h   ***
 ***********************/

#ifndef		_HEAP_TYPE_
#define		_HEAP_TYPE_



typedef struct heap_type {
	char	*data0;
	char	*data,	*p;
	int	size;

} heap_type;




#define		HEAP_ALLOC( heap, type, target ) \
			{\
				target = ( type * )heap->p; \
				heap->p += sizeof( type );\
			}


#define		HEAP_ALLOC_N( heap, type, n, target) \
			{ \
				target = ( type * )heap->p; \
				heap->p += sizeof( type ) * n; \
			}



heap_type *	heap_alloc( int size );

void			heap_destroy( heap_type *heap );

void			heap_clear( heap_type *heap );

void			heap_extand( heap_type *heap, int size );


#endif
