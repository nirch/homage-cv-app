 /*****************************
 ***	Image1EdgeSobol.c	***
 ******************************/

#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"


#define RC	25

image_type *
image2_gridient( image_type *sim, image_type *im )
{
short	*sp,	*sp0,	*sp2;
short	*tp;
int	i,	j;
int	cx,	cy;



	im = image_recreate( im, sim->row, sim->column, 4, 1 );
	image1_const( im, 0 );



	sp = (short *)sim->data;
	tp = (short *)im->data;
	
	sp += sim->column+1;
	tp += 2*(sim->column+1);

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 4 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){


			cx = *(sp+1) - *(sp-1);
			cy = *(sp2) - *(sp0);

			cx >>= 1;
			cy >>= 1;

			*tp++ = cx;
			*tp++ = cy;
		}
	}


	return( im );
}


