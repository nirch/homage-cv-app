 /*****************************
 ***	Image2EdgeSobol.c	***
 ******************************/

#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"



image_type *
image2_gridient_sobol( image_type *sim, image_type *tim )
{
short	*sp,	*sp0,	*sp2;
short	*tp;
int	i,	j;
int	cx,	cy;



	if( tim == NULL ){
		tim = image_alloc( sim->row, sim->column, 2, IMAGE_TYPE_S12, 1 );
		image2_const( tim, 0 );
	}


	sp = sim->data_s;
	tp = tim->data_s;



	
	sp += sim->column+1;
	tp += 2*(sim->column+1);

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 4 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){
			

			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));


			cx >>= 3;
			cy >>= 3;

			*tp++ = cx;
			*tp++ = cy;
		}
	}

	return( tim );
}



image_type *
image2_gridient_sobol_value( image_type *sim, image_type *tim )
{
	short	*sp,	*sp0,	*sp2;
	short	*tp;
	int	i,	j;
	int	cx,	cy,	val;


	if( tim == NULL ){
		tim = image_alloc( sim->column, sim->row, 1, IMAGE_TYPE_S12, 1 );
		image2_const( tim, 0 );
	}


	sp = sim->data_s;
	tp = tim->data_s;




	sp += sim->column+1;
	tp += sim->column+1;

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 2 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){


			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));

			cx >>= 3;
			cy >>= 3;


			val = ABS(cx) + ABS(cy );

//			*tp++ = PUSH_TO_RANGE( val, 0, 256 );
			*tp++ = val;
		}
	}

	return( tim );
}


image_type *
image2_gridient_sobol3( image_type *sim, image_type *tim )
{
	short	*sp,	*sp0,	*sp2;
	short	*tp;
	int	i,	j;
	int	cx,	cy;


	tim = image_realloc( tim, sim->row, sim->column, 3, IMAGE_TYPE_S12, 1 );



	sp = sim->data_s;
	tp = tim->data_s;




	sp += sim->column+1;
	tp += 3*(sim->column+1);

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 6 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){


			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));


			cx >>= 3;
			cy >>= 3;

			*tp++ = cx;
			*tp++ = cy;
			*tp++ = hypotA( cx, cy );
		}
	}

	return( tim );
}



