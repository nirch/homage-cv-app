/************************
 ***   ImageEqual.c   ***
 ************************/
#include	"ImageType/ImageType.h"


static int	image1_equal( image_type *im0, image_type *im1 );

static int	image8_equal( image_type *im0, image_type *im1 );


int
image_equal( image_type *im0, image_type *im1 )
{
int	i,	j;
u_char	*p0,	*p1;
int	n;

	if( im0->depth == 8 )
		return image8_equal( im0, im1 );

	if( im0->depth == 1 )
		return image1_equal( im0, im1 );


	p0 = im0->data;
	p1 = im1->data;

	n = 0;
	for( i = 0 ; i < im0->row ; i++ )
		for( j = 0 ; j < im0->column*im0->depth ; j++ ){
			if( *p0 != *p1 )
				n++;
			p0++;
			p1++;

		}

		return(n);
}



static int
image1_equal( image_type *im0, image_type *im1 )
{
int	i,	j;
u_char	*p0,	*p1;
int	n;

	if( im0->depth == 8 )
		return image8_equal( im0, im1 );

	if( im0->depth == 1 )
		return image1_equal( im0, im1 );


	p0 = (u_char *)im0->data;
	p1 = (u_char *)im1->data;

	n = 0;
	for( i = 0 ; i < im0->row ; i++ )
		for( j = 0 ; j < im0->column ; j++ ){
			if( *p0 != *p1 )
				n++;
			p0++;
			p1++;

		}

		return(n);
}


static int
image8_equal( image_type *im0, image_type *im1 )
{
int	i,	j;
float	*p0,	*p1;
int	n;
	p0 = (float *)im0->data;
	p1 = (float *)im1->data;

	n = 0;
	for( i = 0 ; i < im0->row ; i++ )
		for( j = 0 ; j < im0->column ; j++ ){
			if( *p0 != *p1 )
				n++;
			p0++;
			p1++;
			if( *p0 != *p1 )
				n++;
			p0++;
			p1++;
		}

		return(n);
}
