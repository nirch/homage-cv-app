#include	<stdio.h>
#include	"ImageType/ImageType.h"



void
imageA_box(image_type *im, box2i *box, int *x0, int *y0, int *x1, int *y1 )
{
	if( box == NULL ){
		*y0 = 0;
		*y1 = im->row;
		*x0 = 0;
		*x1 = im->column;

		return;
	}

	*y0 = ( box->y0 >= 0 )? box->y0 : 0;
	*y1 = ( box->y1 < im->height ) ? box->y1 : im->height;

	*x0 = ( box->x0 >= 0 )? box->x0 : 0;
	*x1 = ( box->x1 < im->width ) ? box->x1 : im->width;

}



void
imageA_icycle( image_type *aIm[], int N )
{
	int	k;

	image_type	*tim = aIm[N-1];
	for( k = N-1 ; k > 0 ;  k-- ){
		aIm[k] = aIm[k-1];
	}

	aIm[0] = tim;
}



void
imageA_cycle( imageA_type *aim )
{
	int	k;

	image_type	*tim = aim->a[aim->nA-1];
	for( k = aim->nA-1 ; k > 0 ;  k-- ){
		aim->a[k] = aim->a[k-1];
	}

	aim->a[0] = tim;
}


void
imageA_ocycle( imageA_type *aim )
{
	int	k;

	image_type	*tim = aim->a[0];
	for( k = 0 ; k < aim->nA-1 ; k++ )
		aim->a[k] = aim->a[k+1];

	aim->a[aim->nA-1] = tim;
}