/**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"

static image_type *	image1_subtract( image_type *sim1, image_type *sim0, image_type *im );


image_type *
image_subtract( image_type *sim1, image_type *sim0, image_type *im )
{
    switch( sim1->depth ){
    case 1:
            im = image1_subtract( sim1, sim0, im );
        break;

    case 4:
            im = image4_subtract( sim1, sim0, im );
        break;
    }

	return( im );
} 


static image_type *
image1_subtract( image_type *sim1, image_type *sim0, image_type *im )
{
u_char	*sp0,	*sp1;
u_char	*p;
int	row,	col,	i,	j;
int	tmp;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	im = image_recreate( im, row, col, 1, 1 );

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim0, i, 0 );
		sp1 = IMAGE_PIXEL( sim1, i, 0 );
		for( j = 0 ; j < im->column ; j++ ){
			tmp = (*sp1) - (*sp0) + 128;
			*p++ = PUSH_TO_RANGE( tmp, 0, 255 );


			sp0++;
			sp1++;
		}
	}

	return( im );
}



image_type *
image4_subtract( image_type *sim1, image_type *sim0, image_type *im )
{
u_int	*sp0,	*sp1;
u_int	*p;
int	row,	col,	i,	j;
int	r,	g,	b,	tmp;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	im = image_recreate( im, row, col, 4, 1 );

	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE4_PIXEL( sim0, i, 0 );
		sp1 = IMAGE4_PIXEL( sim1, i, 0 );

		for( j = 0 ; j < im->column ; j++ ){
			tmp = IMAGE4_RED(*sp1) - IMAGE4_RED(*sp0) + 128;
			r = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = IMAGE4_GREEN(*sp1) - IMAGE4_GREEN(*sp0) + 128;
			g = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = IMAGE4_BLUE(*sp1) - IMAGE4_BLUE(*sp0) + 128;
			b = PUSH_TO_RANGE( tmp, 0, 255 );

			sp0++;
			sp1++;

			*p++ = IMAGE4_RGB( r, g, b );
		}
	}

	return( im );
}


