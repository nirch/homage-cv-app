#include	<stdio.h>
#include	<string.h>
#include	"ImageType/ImageType.h"


#include "ImageLeak.h"


#define		NIM		10000
static int	nIm = 0;
static image_type	*aIm[NIM];



void	
image_leak_alloc( image_type *im )
{
	if( nIm < NIM )
		aIm[nIm++] = im;
}


void	
image_leak_destroy( image_type *im )
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
image_leak_print( FILE *fp )
{
int	i;

	fprintf( fp, "Image Leak:   ");
	for( i = 0 ; i < nIm ; i++ ){
		if( aIm[i] != NULL ){
			fprintf( fp, "%d   ", i );
		}
	}

	fprintf( fp, "\n");
}


int	
image_leak_no()
{
	int	i,	no;

	for( i = 0, no = 0 ; i < nIm ; i++ ){
		if( aIm[i] != NULL )
			no++;
	}

	return( no );
}