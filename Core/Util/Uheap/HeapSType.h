/************************
 ***	HeapSType.h   ***
 ************************/

#ifndef		_HEAPS_TYPE_
#define		_HEAPS_TYPE_



typedef struct heapS_type {
	int	size;
	int	no;

	char	*data;
	char	*p;

} heapS_type;


#define		HEAPS_ALLOC( heap, type ) (((type *)heap->p)++)

#define		HEAPS_ALLOC_O( heap, type, target ) \
			{\
				target = ( type * )heap->p; \
				heap->p += sizeof( type );\
			}


#define		HEAPS_ALLOC_N( heap, type, n, target) \
			{ \
				target = ( type * )heap->p; \
				heap->p += sizeof( type ) * n; \
			}



#define	HEAPS_NEXT( hs, type )		(((type *)hs->p)++)
#define	HEAPS_FRONT( hs, type )				((type *)hs->p)

#define	HEAPS_NEXTO( hs, type, target ) \
{\
	target = ( type * )hs->p; \
	hs->p += sizeof( type );\
}



heapS_type	*	heapS_alloc( int no, int size );

void			heapS_destroy( heapS_type *hs );

void			heapS_rewind( heapS_type *hs );

void			heapS_set_front( heapS_type *hs, void *p );




#endif

