/**********************
 ***   ImageAdd.c   ***
 **********************/

#include	"ImageType/ImageType.h"

static image_type *	image1_add( image_type *sim0, image_type *sim1, float a0, float a1, float b );
static image_type *	image4_add( image_type *sim0, image_type *sim1, float a0, float a1, float b );


image_type *
image_add( image_type *sim0, image_type *sim1, float a0, float a1, float b )
{
image_type	*im;
 
        switch( sim1->depth ){
        case 1:
                im = image1_add( sim1, sim0, a0, a1, b );
            break;
 
        case 4:
                im = image4_add( sim1, sim0, a0, a1, b );
            break;
        }

	return( im );
} 


static image_type *
image1_add( image_type *sim0, image_type *sim1, float a0, float a1, float b )
{
u_char	*sp0,	*sp1;
u_char	*p;
image_type	*im;
int	row,	col,	i,	j;
int	tmp;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	im = image_create( row, col, 1, 1, NULL );

	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim0, i, 0 );
		sp1 = IMAGE_PIXEL( sim1, i, 0 );
		for( j = 0 ; j < im->column ; j++ ){
			tmp = a1*(*sp1) + a0*(*sp0) + b;
			*p++ = PUSH_TO_RANGE( tmp, 0, 255 );


			sp0++;
			sp1++;
		}
	}

	return( im );
}



static image_type *
image4_add( image_type *sim0, image_type *sim1, float a0, float a1, float bb )
{
u_int	*sp0,	*sp1;
u_int	*p;
image_type	*im;
int	row,	col,	i,	j;
int	r,	g,	b,	tmp;

	row = MIN( IMAGE_ROW(sim0), IMAGE_ROW(sim1) ); 
	col = MIN( IMAGE_COLUMN(sim0), IMAGE_COLUMN(sim1) ); 

	im = image_create( row, col, 4, 1, NULL );

	p = (u_int *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE4_PIXEL( sim0, i, 0 );
		sp1 = IMAGE4_PIXEL( sim1, i, 0 );

		for( j = 0 ; j < im->column ; j++ ){
			tmp = a1*IMAGE4_RED(*sp1) + a0*IMAGE4_RED(*sp0) + bb;
			r = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = a1*IMAGE4_GREEN(*sp1) +a0*IMAGE4_GREEN(*sp0) + bb;
			g = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = a1*IMAGE4_BLUE(*sp1) +a0*IMAGE4_BLUE(*sp0) + bb;
			b = PUSH_TO_RANGE( tmp, 0, 255 );

			sp0++;
			sp1++;

			*p++ = IMAGE4_RGB( r, g, b );
		}
	}

	return( im );
}


