#include	<stdio.h>


#include	"Uigp/igp.h"

#include	"Box1i.h"



void
box1i_print(  char *title, box1i_type ab[], int nP, FILE *fp )
{
	int	i;

	fprintf( fp, "%s  %d\n", title, nP );

	for( i = 0 ; i < nP ; i++ ){
		fprintf( fp, "%d   %d  %d\n", i, ab[i].i0, ab[i].i1 );
	}
}