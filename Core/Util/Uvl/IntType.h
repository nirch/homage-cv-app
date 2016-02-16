#ifndef _INT_TYPE_
#define _INT_TYPE_





typedef struct intA_type {

	int	NA;

	int	nA;
	int	*a;

}	intA_type;




intA_type *	intA_alloc( int no );

void	intA_destroy( intA_type *aS );

intA_type *	intA_realloc( intA_type *ai, int no );

intA_type *		intA_copy( intA_type *as, intA_type *ai );

void	intA_set( intA_type *a, int val );

int	intA_sum( intA_type *a );



void	intA_add( intA_type *a, int val );

void	intA_add( intA_type *tai, intA_type *ai );


void	intA_add( intA_type *as, int val[], int nVal );

int		intA_index( intA_type *ai, int val );

void	intA_flip( intA_type *b );


void	intA_write( intA_type *ai, FILE *fp );

void	intA_writeb( intA_type *ai, FILE *fp );

void	intA_readb( intA_type **ai, FILE *fp );


int		intA_read( intA_type **ai, char *file );

int		intA_read( intA_type **ai, FILE *fp );



int	intA_write( intA_type *ai, char *file );

int	intA_write_index( intA_type *ai, char *file );

void	intA_dump( intA_type *ai, char *prefix, int index, char *suffix );



#ifdef _DUMP
#define INTA_DUMP( a, name, index, ext )  intA_dump( a, name, index, ext )
#else
#define INTA_DUMP( a, name, index, ext )
#endif

#endif

