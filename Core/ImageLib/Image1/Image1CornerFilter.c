 /*********************************
 ***	Image1EqualNeighbor.c	***
 *********************************/

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


static image_type *	image1_corner( image_type *sim, image_type *im );


int
image1_corener_filter( image_type *sim )
{
image_type *im;
u_char	*sp,	*sp0,	*sp1,	*sp2,	*tp,	*tp0,	*tp1;
int	i,	j,	n,	k,	val;
int	align,	no;


	im = image1_corner( sim, NULL );

	no = 0;
	align = sim->width - 3;
	for( i = 1 ; i < sim->row-1 ; i ++ ){
		sp = IMAGE_PIXEL( sim, i, 1 );
		sp0 = sp - sim->width;
		sp1 = sp + sim->width;

		tp = IMAGE_PIXEL( im, i, 1 );
		tp0 = tp - im->width;
		tp1 = tp + im->width;


		for( j = 1 ; j < sim->column-1 ; j ++, sp++, sp0++, sp1++, tp++, tp0++, tp1++ ){

			if( *tp < 3 || *sp == 0 )	continue;

			n = 0;
			if( *(sp0-1) == *sp && *(tp0-1) >= 3 )	n++;
			if( *(sp0) == *sp   && *(tp0) >= 3   )	n++;
			if( *(sp0+1) == *sp && *(tp0+1) >= 3 )	n++;

			if( *(sp -1) == *sp && *(tp-1) >= 3 )	n++;
			if( *(sp+1) == *sp  && *(tp+1) >= 3 )	n++;

			if( *(sp1-1) == *sp && *(tp1-1) >= 3 )	n++;
			if( *(sp1) == *sp   && *(tp1) >= 3   )	n++;
			if( *(sp1+1) == *sp && *(tp1+1) >= 3 )	n++;


			if( n > 2 ){
				*tp = 16;
				no++;
			}
		}
	}


	no = 0;
	for( i = 1 ; i < sim->row-1 ; i ++ ){
		sp = IMAGE_PIXEL( sim, i, 1 );

		tp = IMAGE_PIXEL( im, i, 1 );



		for( j = 1 ; j < sim->column-1 ; j ++, sp++, tp++ ){

			if( *tp != 16  )	continue;
			val = -1;
			sp2 = sp - sim->width -1;

			for( k = 0 ; k < 3 && val <  0 ; k++, sp2 += align ){
				for( n = 0 ; n < 3  ; n++, sp2++ ){
					if( *sp != *sp2 && *sp2 > 0 ){
						val = *sp2;
						break;
					}
				}
			}

			if( val >=0 ){
				*sp = val;
				no++;
			}
		}

	}

	image_destroy( im, 1 );

	return( no );
}


//
//static image_type *
//image1_corner( image_type *sim, image_type *im )
//{
//u_char	*sp,	*sp0,	*sp1,	*tp;
//int	i,	j;
//int	no;
//
//	im = image_recreate( im, sim->height, sim->width, 1, 1 );
//	image1_const( im, 0 );
//
//
//
//
//	
//
//
//	for( i = 1, no = 0 ; i < sim->row-1 ; i ++ ){
//		sp = IMAGE_PIXEL( sim, i, 1 );
//		sp0 = sp - sim->width;
//		sp1 = sp + sim->width;
//
//		tp = IMAGE_PIXEL( im, i, 1 );
//		for( j = 1 ; j < sim->column-1 ; j ++, sp++, sp0++, sp1++, tp++ ){
//
//			if( *sp0 == 0 )	continue;
//
//			if( *sp0 == *(sp0+1) ){
//				if( *sp0 == *sp1 || *sp0 == *(sp1+1) || *(sp1) == *(sp1+1) )
//					continue;
//			}
//			else	{
//				if( *sp0 == *(sp1) || *(sp0+1) == *(sp1)){
//					if( *sp0 == *(sp1+1) || *(sp0+1) == *(sp1+1))
//						continue;
//				}
//			}
//
//			*tp = 1;
//
//		}
//	}
//
//	return( im );
//}
//





static image_type *
image1_corner( image_type *sim, image_type *im )
{
int	i,	j,	n,	k;
u_char	*sp,	*sp0,	*tp;
int	align;

int	aN;
int	a[10];
int	s;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );
//	image1_const( im );




	align = im->width - 3;
	for( i = 1 ; i < im->height-1 ; i++ ){
		sp = IMAGE_PIXEL( sim, i, 1 );
		tp = IMAGE_PIXEL( im, i, 0 );

	
		*tp++ = 0;

		for( j = 1 ; j < im->width-1 ; j++, sp++, tp++ ){

			if( *sp == 0 ){
				*tp = 0;
				continue;
			}

			aN = 0;
			sp0 = sp - im->width - 1;
			for( k = 0 ; k < 3 ; k++, sp0 += align ){
				for( n = 0 ; n < 3 ; n++, sp0++ ){

					for( s = 0 ; s < aN ; s++ )
						if( a[s] == *sp0 )	break;

					if( s == aN )
						a[aN++] = *sp0;
				}
			}

			*tp = aN;
		}


		*tp++ = 0;
	}



	tp = IMAGE_PIXEL( im, 0, 0 );
	for( j = 0 ; j < im->width ; j++ )
		*tp++ = 0;

	tp = IMAGE_PIXEL( im, im->height-1, 0 );
	for( j = 0 ; j < im->width ; j++ )
		*tp++ = 0;

	return( im );
}
