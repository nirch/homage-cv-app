#ifndef _STRING_TYPE_
#define _STRING_TYPE_





typedef struct stringA_type {

	int	NA;

	int	nA;
	char	**a;

}	stringA_type;




stringA_type *	stringA_alloc( int no );

void	stringA_destroy( stringA_type *aS );

stringA_type *	stringA_realloc( stringA_type *ai, int no );

stringA_type *		stringA_copy( stringA_type *as, stringA_type *ai );

stringA_type *	stringA_create( char *a[], int nA );



void	stringA_add( stringA_type *as, char *s, int fCopy );


void	stringA_add( stringA_type *tai, stringA_type *ai );


void	stringA_add( stringA_type *as, int val[], int nVal );


int		stringA_index( stringA_type *ai, char *val );


void	stringA_flip( stringA_type *b );


void	stringA_write( stringA_type *ai, FILE *fp );

void	stringA_writeb( stringA_type *ai, FILE *fp );

void	stringA_readb( stringA_type **ai, FILE *fp );


int		stringA_read( stringA_type **ai, char *file );

int		stringA_read( stringA_type **ai, FILE *fp );



int	stringA_write( stringA_type *ai, char *file );



#endif

