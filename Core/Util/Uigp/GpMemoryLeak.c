#include	<stdio.h>
#include	<string.h>



#include	"GpMemoryLeak.h"


#define		NIM		100000
static int	nIm = 0;
static void	*aIm[NIM];



void	
gpMemory_leak_alloc( void *im )
{
	if( nIm < NIM )
		aIm[nIm++] = im;
}


void	
gpMemory_leak_destroy( void *im )
{
	int	i;

	for( i = 0 ; i < nIm ; i++ ){
		if( aIm[i] == im ){
			aIm[i] = NULL;
			return;
		}
	}
}

void	
gpMemory_leak_print( FILE *fp )
{
int	i;

	fprintf( fp, "gpMemory Leak:   ");
	for( i = 0 ; i < nIm ; i++ ){
		if( aIm[i] != NULL ){
			fprintf( fp, "%d   ", i );
		}
	}
	fprintf( fp, "\n");
}
