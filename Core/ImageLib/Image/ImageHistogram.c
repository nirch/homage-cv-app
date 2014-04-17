/****************************
 ***    ImageHistogram.c  ***
 ***************************/
 
#include	"ImageType/ImageType.h"


static int	imageUS_histogram( image_type *im, box2i *box, int h[], int fclear );
static int	imageS_histogram( image_type *im, box2i *box, int h[], int fclear );


int
imageA_histogram( imageA_type *aim, box2i_type *box, int **h, int *nH )
{
	int	i,	pixel,	type;

	*nH = 0;


	type = IMAGE_TYPE( aim->a[0] );
	if(  type == IMAGE_TYPE_U8 )
		*nH = 256;

	if(  type == IMAGE_TYPE_U16 )
		*nH = 1<<16;

	if(  type == IMAGE_TYPE_S12 )
		*nH = 1<<12;


	if( *nH == 0 )	return( -1 );


	*h = (int *)calloc( *nH, sizeof(int) );


	pixel = 0;
	for( i = 0; i < aim->nA ; i++ )
		pixel += image_histogram( aim->a[i], NULL, *h, 0 );

	return( pixel );
}



int
image_histogram( image_type *sim, box2i_type *box, int h[], int fClear )
{
int	ret;

	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U8 ){
		ret = image1_histogram( sim, box, h, fClear );
		return( ret );
	}

	if( IMAGE_TYPE(sim) == IMAGE_TYPE_U16 ){
		ret = imageUS_histogram( sim, box, h, fClear );
		return( ret );
	}
	if( IMAGE_TYPE(sim) == IMAGE_TYPE_S12 ){
		ret = imageS_histogram( sim, box, h, fClear );
		return( ret );
	}

	return( 0 );
} 
 
 
static int
imageUS_histogram( image_type *im, box2i *box, int h[], int fclear )
{
	u_short	*sp;
	int	i,	j,	n;
	int	i0,	i1,	j0,	j1;


	imageA_box( im , box, &j0, &i0, &j1, &i1 );

	if( fclear )
		for( i = 0 ; i < 256 ; i++ )	h[i] = 0;


	for( i = i0, n = 0 ; i < i1 ; i++ ){
		sp = (u_short*)IMAGE_PIXEL( im, i, j0 );
		for( j = j0 ; j < j1 ; j++ ){
			h[*sp++] ++;

			n++;
		}
	}

	return( n );
}


static int
imageS_histogram( image_type *im, box2i *box, int h[], int fclear )
{
	u_short	*sp;
	int	i,	j,	n;
	int	i0,	i1,	j0,	j1,	k;


	imageA_box( im , box, &j0, &i0, &j1, &i1 );

	if( fclear )
		for( i = 0 ; i < 256 ; i++ )	h[i] = 0;


	for( i = i0, n = 0 ; i < i1 ; i++ ){
		sp = (u_short*)IMAGE_PIXEL( im, i, j0 );
		for( j = j0 ; j < j1 ; j++ ){
			k = PUSH_TO_RANGE( *sp, 0, 4095 );
			h[k] ++;

			n++;
		}
	}

	return( n );
}