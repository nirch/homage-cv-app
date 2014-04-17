/************************
 ***   ImageAlpha.c   ***
 ************************/
#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"




image_type *
image_alpha_copy( image_type *sim, int margin )
{
image_type	*tim;
u_int	*sp;
u_char	*tp;

int	i,	j;


	tim = image_create( sim->row+2*margin, sim->column+2*margin, 1, 1, NULL );

	tp = tim->data;
	for( i = 0 ; i < tim->row ; i++ )
		for( j = 0 ; j < tim->column ; j++ )
			*tp++ = 0xFF;
	

	sp = (u_int *)sim->data;

	tp = IMAGE_PIXEL( tim, margin, margin );

	
	for( i = 0 ; i < sim->row ; i++, tp += 2*margin ){
		for( j = 0 ; j < sim->column ; j++, tp++, sp++ )
			*tp = ((*sp)>>24) & 0xFF;
	}

	return( tim );
}





void
image_alpha_clear( image_type *image )
{
int x, y;
u_int *sp;

	sp = (u_int *)image->data;

    for ( y=0; y < image->row; y++)
		for ( x=0; x < image->column; x++, sp++ )
			*sp &= 0x00FFFFFF;
}



