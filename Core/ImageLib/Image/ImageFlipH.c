/**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"


static void	image1_flipH( image_type *im );

static void	image3_flipH( image_type *im );

static void	image4_flipH( image_type *im );



void
image_flipH( image_type *sim )
{

    switch( sim->depth ){
    case 1:
			image1_flipH( sim );
        break;

	case 3:
			image3_flipH( sim );
		break;

    case 4:
			image4_flipH( sim );
        break;
    }
} 



static void
image1_flipH( image_type *im )
{
	u_char	*p0,	*p1,	tmp;
	int	i,	j,	hcol;


	hcol = im->column / 2;
	for( i = 0 ; i < im->row  ; i++ ){

		p0 = IMAGE_PIXEL( im, i, 0 );
		p1 = IMAGE_PIXEL( im, i, im->column-1 );
		for( j = 0 ; j < hcol ; j++, p0++, p1-- ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}

static void
image3_flipH( image_type *im )
{
	u_char	*p0,	*p1,	tmp;
	int	i,	j,	hcol;


	hcol = im->column / 2;
	for( i = 0 ; i < im->row  ; i++ ){

		p0 = IMAGE_PIXEL( im, i, 0 );
		p1 = IMAGE_PIXEL( im, i, im->column-1 );
		for( j = 0 ; j < hcol ; j++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0++;
			p1++;

			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0++;
			p1++;

			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
			p0++;
			p1++;

			p1 -= 6;
		}
	}
}


static void
image3_flipV( image_type *im )
{
	u_char	*p0,	*p1,	tmp;
	int	i,	j,	hrow;

	hrow = im->row / 2;
	for( i = 0 ; i < hrow  ; i++ ){

		p0 = IMAGE_PIXEL( im, i, 0 );
		p1 = IMAGE_PIXEL( im, im->row-1-i, 0 );
		for( j = 0 ; j < 3*im->column ; j++, p0++, p1++ ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}




static void
image4_flipH( image_type *im )
{
u_int	*p0,	*p1,	tmp;
int	i,	j,	hcol;


	hcol = im->column / 2;
	for( i = 0 ; i < im->row  ; i++ ){

		p0 = IMAGE4_PIXEL( im, i, 0 );
		p1 = IMAGE4_PIXEL( im, i, im->column-1 );
		for( j = 0 ; j < hcol ; j++, p0++, p1-- ){
			tmp = *p0;
			*p0 = *p1;
			*p1 = tmp;
		}
	}
}
