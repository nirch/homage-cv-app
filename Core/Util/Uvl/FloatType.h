#ifndef _FLOAT_TYPE_
#define _FLOAT_TYPE_





typedef struct floatA_type {

	int	NA;

	int	nA;
	float	*a;

}	floatA_type;




floatA_type *	floatA_alloc( int no );

void	floatA_destroy( floatA_type *aS );

floatA_type *	floatA_realloc( floatA_type *ai, int no );

floatA_type *		floatA_copy( floatA_type *as, floatA_type *ai );


void	floatA_add( floatA_type *a, int val );

void	floatA_add( floatA_type *tai, floatA_type *ai );


void	floatA_add( floatA_type *as, int val[], int nVal );

void	floatA_cycle( floatA_type *as, int n );


void	floatA_st( floatA_type *as, struct st_type *st );

float	floatA_average( floatA_type *as );


int		floatA_index( floatA_type *ai, int val );

void	floatA_write( floatA_type *ai, FILE *fp );

void	floatA_writeb( floatA_type *ai, FILE *fp );

void	floatA_readb( floatA_type **ai, FILE *fp );


int		floatA_read( floatA_type **ai, char *file );

int		floatA_read( floatA_type **ai, FILE *fp );



int	floatA_write( floatA_type *ai, char *file );



#endif

