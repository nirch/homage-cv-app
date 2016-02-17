 /*****************************
 ***	Image1EdgeSobol.c	***
 ******************************/

#include	"ImageType/ImageType.h"
#include	"ImageUSTool.h"


#ifdef _AA_
image_type *
image1_gridient_sobol( image_type *sim, image_type *tim )
{
u_char	*sp,	*sp0,	*sp2;
s_char	*tp;
int	i,	j;
int	cx,	cy;



	if( tim == NULL ){
		tim = image_create( sim->row, sim->column, 2, 1, NULL );
		image2_const( tim, 0 );
	}


	sp = (u_char *)sim->data;
	tp = (s_char *)tim->data;



	
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
#endif




image_type *
imageUS_gridient_sobol_value( image_type *sim, image_type *tim )
{
	int	i,	j;
	int	cx,	cy,	val;




	tim = image_realloc( tim, sim->width, sim->height, 1, IMAGE_TYPE_U16, 1 );
	imageUS_const( tim, 0 );


	u_short *sp = sim->data_us;
	u_short *tp = tim->data_us;




	sp += sim->column+1;
	tp += sim->column+1;

	for( i = 1 ; i < sim->height-1 ; i++, sp += 2, tp += 2 ){

		u_short *sp0 = sp - sim->width;
		u_short *sp2 = sp + sim->width;
		for( j = 1 ; j < sim->width-1 ; j++, sp++, sp0++, sp2++ ){


			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));

			cx >>= 3;
			cy >>= 3;


			val = ABS(cx) + ABS(cy );

			*tp++ = PUSH_TO_RANGE( val, 0, 255 );
		}
	}

	return( tim );
}

#ifdef _AA_
image_type *
	image1M_gridient_sobol_value( image_type *sim, image_type *mim, image_type *im )
{
	u_char	*sp,	*sp0,	*sp2,	*mp;
	u_char	*tp;
	int	i,	j;
	int	cx,	cy;



	im = image_recreate( im, sim->row, sim->column, 1, 1 );
	image1_const( im, 0 );

	tp = im->data;



	for( i = 1 ; i < sim->row-1 ; i++ ){

		sp = IMAGE_PIXEL( sim, i, 1 );
		mp = IMAGE_PIXEL( mim, i, 1 );
		tp = IMAGE_PIXEL( im, i, 1 );

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++, mp++, tp++ ){


			if( *mp != 0 )	continue;

			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) - *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2) - *(sp0)) + (*(sp2+1) - *(sp0+1)));

			cx >>= 3;
			cy >>= 3;

			*tp = 2*(ABS(cx) + ABS(cy));
		}
	}

	return( im );
}



image_type *
image1_gridient_sobol3( image_type *sim, image_type *tim )
{
	u_char	*sp,	*sp0,	*sp2;
	s_char	*tp;
	int	i,	j;
	int	cx,	cy;



	if( tim == NULL ){
		tim = image_create( sim->row, sim->column, 3, 1, NULL );
		image3_const( tim, 0, 0, 0 );
	}


	sp = (u_char *)sim->data;
	tp = (s_char *)tim->data;




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



image_type *
image1_gridient_to_value( image_type *sim, image_type *tim )
{
s_char	*sp;
u_char	*tp;
int	i,	j,	val;

	if( tim == NULL )
		tim = image_create( sim->row, sim->column, 1, 1, NULL );


	sp = (s_char *)sim->data;
	tp = (u_char *)tim->data;
	


	for( i = 0 ; i < sim->row ; i++ ){

		for( j = 0 ; j < sim->column ; j++, sp += 2 ){
			

			val = ABS(*sp) + ABS(*(sp+1) );

			*tp++ = PUSH_TO_RANGE( val, 0, 256 );
		}
	}


	return( tim );
}
#endif

