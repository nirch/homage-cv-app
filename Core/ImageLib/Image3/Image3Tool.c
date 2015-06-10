/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"






image_type *
image3_to_image4( image_type *sim, image_type *im )
{
u_int	*tp;
u_char	*sp;
int	i,	j;


	if( im != NULL && ( im->row != sim->row || im->column != sim->column ) ){
		image_destroy( im, 1 );
		im = NULL;
	}
	
	if( im == NULL )
		im = image_create( sim->row, sim->column, 4, 1, NULL );


	tp = (u_int *)im->data;
	sp = sim->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp += 3 ){
			*tp++ = IMAGE4_RGB( *sp, *(sp+1), *(sp+2) );
		}

	return( im );
}



void
image3_const( image_type *im, int R, int G, int B )
{
u_char	*sp;
int	i,	j;

	sp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){
			*sp++ = R;
			*sp++ = G;
			*sp++ = B;

		}
}

//void
//image3_copy_in( image_type *sim, image_type *im, int x0, int y0 )
//{
//u_char	*tp,	*sp;
//int	i,	j,	algin;
//
//
//	algin = im->column - sim->column;
//	sp = sim->data;
//	tp = IMAGE_PIXEL( im, y0, x0 );
//
//	for( i = 0 ; i < sim->row ; i++, tp += 3*algin ){
//		for( j = 0 ; j < 3*sim->column ; j++ )
//			*tp++ = *sp++;
//	}
//}

image_type *
image3_subtruct( image_type *im1, image_type *im0, image_type *im )
{
u_char	*sp1,	*sp0,	*tp;
int	tmp;
int	i,	j;

	if( im == NULL )
		im = image_create( im1->row, im1->column, 3, 1, NULL );

	sp1 = im1->data;
	sp0 = im0->data;
	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			tmp = 128L + *sp1 - *sp0;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
			sp1++; sp0++;

			tmp = 128L + *sp1 - *sp0;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
			sp1++; sp0++;

			tmp = 128L + *sp1 - *sp0;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );
			sp1++; sp0++;
		}

	return( im );
}


//image_type *
//image3_crop( image_type *sim, int x0, int y0, int width, int height, image_type *im )
//{
//u_char	*sp,	*tp;
//int	i,	j;
//
//
//	im = image_recreate( im, height, width, 3, 1 );
//
//	tp = (u_char *)im->data;
//	for( i = 0 ; i < im->row ; i++ ){
//		sp = IMAGE_PIXEL( sim, y0+i, x0 );
//		for( j = 0 ; j < im->column ; j++ ){
//			*tp++ = *sp++;
//			*tp++ = *sp++;
//			*tp++ = *sp++;
//		}
//	}
//
//	return( im );
//}




image_type *
image3_dup( image_type *sim, int drow, int dcol, image_type *im )
{
u_char	*sp,	*tp,	*sp0;
int	i,	j,	i1,	j1;

	im = image_recreate( im, sim->row*drow, sim->column*dcol, 3, 1 );

	
	tp = im->data;
	sp0 = sim->data;
	for( i = 0 ; i < sim->row ; i++, sp0 += 3*sim->column ){


		for( i1 = 0 ; i1 < drow ; i1++ ){
			sp = sp0;
			for( j = 0 ; j < sim->column ; j++, sp += 3 )
				for( j1 = 0 ; j1 < dcol ; j1++ ){
					*tp++ = *sp;
					*tp++ = *(sp+1);
					*tp++ = *(sp+2);
				}
		}
	}

	return( im );
}




#ifdef _AA_
image_type *
image3_sample( image_type *sim, int d, image_type *im )
{
u_char	*sp,	*sp0,	*tp;
int	i,	j;
int	id;
int	k,	n;
int	align,	sum;
int	r,	g,	b;


	im = image_recreate( im, sim->row/d, sim->column/d, 3, 1 );

	align = 3*(sim->width - d);
	tp = im->data;
	for( i = 0, id = 0 ; i < im->row ; i++, id += d ){

		sp0 = IMAGE_PIXEL( sim, id, 0 );
		for( j = 0 ; j < im->column ; j++, sp0 += 3*d ){

			r = g = b = 0;
			for( k = 0, sp = sp0, sum = 0 ; k < d ; k++, sp += align ){
				for( n = 0 ; n < d ; n++ ){
					r += *sp++;
					g += *sp++;
					b += *sp++;
				}
			}

			*tp++ = r /( d*d );
			*tp++ = g /( d*d );
			*tp++ = b /( d*d );
		}
	}

	return( im );
}
#endif

//image_type *
//image3_sample( image_type *sim, int width, int height )
//{
//u_char	*sp,	*tp;
//image_type	*im;
//int	i,	j,	i1,	j1;
//int	r,	g,	b,	n,	k,	algin;
//float	dup2;
//int	dw,	dh;
//
//	im = image_create( height, width, 3, 1, NULL );
//
//	dw = sim->column/width;
//	dh = sim->row /height;
//
//	dup2 = 1.0F / (dw*dh);
//
//	algin = 3* (sim->column - dw);
//	
//	tp = (u_char *)im->data;
//	for( i = 0, i1 = 0 ; i < im->row ; i++, i1 += dh ){
//	    for( j = 0, j1 = 0 ; j < im->column ; j++, j1 += dw ){
//
//			sp = IMAGE_PIXEL( sim, i1, j1 );
//			r = g = b = 0;
//			for( k = 0 ; k < dh ; k++, sp += algin )
//				for( n = 0 ; n < dw ; n++ ){
//
//					r += *sp++;
//					g += *sp++;
//					b += *sp++;
//				}
//
//			r *= dup2;
//			g *= dup2;
//			b *= dup2;
//
//			*tp++ = r;
//			*tp++ = g;
//			*tp++ = b;
//	    }
//	}
//
//
//	return( im );
//}


float
image3_information_estimation( image_type *sim )
{
image_type	*im;
float	inf;

	im = image3_to_y( sim, NULL );

	inf = image1_information_estimation( im );

	image_destroy( im, 1);

	return( inf );
}


#define	FR	19595		// 0.29900 * ( 1<<16)
#define FG	38469		// 0.58700 * ( 1<<16);
#define FB	7471		// 0.11400 * ( 1<<16);

image_type *
image3_to_y( image_type *sim, image_type *im )
{
u_char	*sp;
u_char	*p;
int	i,	j;

int	R,	G,	B;
#ifdef RGB2Y_FLOAT
int	Y;
#endif




	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = (u_char *)sim->data;
	p = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

#ifdef RGB2Y_FLOAT
			Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
			*p++ = PUSH_TO_RANGE( Y, 0, 255 );
#else
			*p++ = (R*FR + G *FG + B *FB)>>16;
#endif



		}
	}

	return( im );
}


image_type *
image3_to_cr( image_type *sim, image_type *yim, image_type *im )
{
u_char	*sp;
u_char	*yp,	*tp;
int	i,	j;

int	R,	Y;
#ifdef RGB2Y_FLOAT
int	cr;
#endif




	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = (u_char *)sim->data;
	yp = (u_char *)yim->data;
	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp+=3 ){
			R = *sp;
			Y = *yp++;
			

#ifdef RGB2Y_FLOAT

			//Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
			cr = (  R - Y )*0.7132 + 128;
			*tp++ = PUSH_TO_RANGE( cr, 0, 255 );
#else
			*tp++ = (((  R - Y )*46740)>>16) + 128;
#endif



		}
	}

	return( im );
}

//image_type *
//image3_linear_combination( image_type *sim, float a, float fR, float fG, float fB, image_type *im )
//{
//u_char	*sp;
//u_char	*tp;
//int	i,	j;
//
//	int	R,	G,	B;
//
//	int	Y;
//
//
//
//
//	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );
//
//	sp = (u_char *)sim->data;
//	tp = (u_char *)im->data;
//	for( i = 0 ; i < im->row ; i++ ){
//		for( j = 0 ; j < im->column ; j++ ){
//			R = *sp++;
//			G = *sp++;
//			B = *sp++;
//
//
//			Y  =  a + fR * R + fG * G + fB * B + 0.5;
//			*tp++ = PUSH_TO_RANGE( Y, 0, 255 );
//		}
//	}
//
//	return( im );
//}


image_type *
image3_linear_combinationF( image_type *sim, float a, float fR, float fG, float fB, image_type *im )
{
u_char	*sp;
float	*tp;
int	i,	j;

int	R,	G,	B;





	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_F, 1 );

	sp = (u_char *)sim->data;
	tp = im->data_f;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;


			*tp++  =  a + fR * R + fG * G + fB * B;
		}
	}

	return( im );
}


image_type *
image3_mult( image_type *sim, float fr, float fg, float fb, image_type *im )
{
u_char	*sp;
u_char	*tp;
int	i,	j;
int	tmp;


	if( im == NULL )
		im = image_create( sim->row, sim->column, 3, 1, NULL );

	sp = (u_char *)sim->data;
	tp = (u_char *)im->data;



	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			tmp = fr * *sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = fg * *sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );

			tmp = fb * *sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 255 );

		}
	}

	return( im );
}


#ifdef _AA_
image_type *
image3_sample2( image_type *sim, image_type *im )
{
u_char	*sp0,	*sp1, *sp2,	*sp3,	*tp;
int	i,	j;

	if( im == NULL )
		im = image_create( sim->row/2, sim->column/2, 3, 1, NULL );


	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 2*i, 0 );
		sp1 = sp0+3;
		sp2 = sp0 + 3*sim->width;
		sp3 = sp2 + 3;

		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = ( *(sp0++) + *(sp1++) + *(sp2++) + *(sp3++) ) >>2;
			*tp++ = ( *(sp0++) + *(sp1++) + *(sp2++) + *(sp3++) ) >>2;
			*tp++ = ( *(sp0++) + *(sp1++) + *(sp2++) + *(sp3++) ) >>2;

			sp0 += 3;
			sp1 += 3;
			sp2 += 3;
			sp3 += 3;
		}
	}

	return( im );
}
#endif

image_type *
image3_rotate90( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;
	int	align;

	align = 3*sim->width + 3;


	im = image_recreate( im, sim->column, sim->row, 3, 1 );

	tp = im->data;
	for ( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( sim, sim->height-1, i );

		for ( j = 0 ; j < im->column ; j++){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;

			sp -= align;
		}
	}

	return( im );

}


image_type *
image3_rotate180( image_type *sim, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;


	im = image_realloc( im, sim->width, sim->height, 3, IMAGE_TYPE_U8, 1 );

	tp = im->data;
	for ( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( sim, sim->height-1-i, sim->width-1 );

		for ( j = 0 ; j < im->column ; j++, sp -= 3 ){
			*tp++ = sp[0];
			*tp++ = sp[1];
			*tp++ = sp[2];
		}
	}

	return( im );
}


void
image3_rotate180_I( image_type *sim )
{
	u_char	*sp,	*tp;
	int	i,	j;



	tp = sim->data;
	sp = IMAGE_PIXEL( sim, sim->height-1, sim->width-1 );
	for ( i = 0 ; i < sim->height / 2 ; i++ ){
		
		for ( j = 0 ; j < sim->column ; j++, sp -= 3 ){
			u_char tmp = sp[0];
			sp[0] = *tp;
			*tp++ = tmp;

			tmp = sp[1];
			sp[1] = *tp;
			*tp++ = tmp;

			tmp = sp[2];
			sp[2] = *tp;
			*tp++ = tmp;
		}
	}
}


image_type *
image3_rotate90_N( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;
	int	align;

	align = 3*sim->width - 3;


	im = image_recreate( im, sim->column, sim->row, 3, 1 );

	tp = im->data;
	for ( i = 0 ; i < im->row ; i++ ){
		sp = IMAGE_PIXEL( sim, 0, i );

		for ( j = 0 ; j < im->column ; j++){
			*tp++ = *sp++;
			*tp++ = *sp++;
			*tp++ = *sp++;

			sp += align;
		}
	}

	return( im );

}





image_type *
image3_maskT( image_type *sim, image_type *mim, int T0, int T1, image_type *im )
{
	u_char	*sp,	*tp,	*mp;
	int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 3, 1 );

	sp = sim->data;
	mp = mim->data;

	tp = im->data;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, mp++ ){
			if( *mp < T0 || *mp > T1 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;

				sp++;
				sp++;
				sp++;
			}
			else {
				*tp++ = *sp++;
				*tp++ = *sp++;
				*tp++ = *sp++;
			}

		}
	}

	return( im );
}




void
image3_swap_rb( image_type *sim )
{
	int	i,	j;
	u_char	*tp,	tmp;

	tp = sim->data;
	for( i = 0; i < sim->height ; i++ )
		for( j = 0; j < sim->width ; j++, tp+=3 ){
			tmp = *tp;
			*tp = *(tp+2);
			*(tp+2) = tmp;
		}
}



image_type *
	image3_abs_diff_image1( image_type *im0, image_type *im1, image_type *im )
{
	u_char	*sp0,	*sp1,	*tp;
	int	i,	j;
	int	tmp,	d;

	im = image_realloc( im, im0->width, im0->height, 1, IMAGE_TYPE_U8, 1 );

	sp0 = im0->data;
	sp1 = im1->data;

	tp = im->data;


	for( i = 0; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){



			if( (d = *sp1++ - *sp0++) < 0 )	d = -d;

			if( (tmp = *sp1++ - *sp0++) < 0 )	tmp = -tmp;
			if( tmp > d )	d = tmp;

			if( (tmp = *sp1++ - *sp0++) < 0 )	tmp = -tmp;
			if( tmp > d )	d = tmp;



			if( d < 0 )	d = -d;
			*tp++ = d;
		}
	}



	return( im );
}


image_type *
image3_binary( image_type *sim, int Tr, int Tg, int Tb, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp += 3, tp++ ){

			if( sp[0] > Tr && sp[1] > Tg && sp[2] > Tb )
				*tp = 255;
			else *tp = 0;
		}
	}

	return( im );
}


image_type *
image3_binaryM( image_type *sim, int r, int g, int b, image_type *im )
{
	u_char *sp,	*tp;
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0; j < sim->width ; j++, sp += 3, tp ++ ){
			if(sp[0] == r && sp[1] == g && sp[2] == b )
				*tp = 0;
			else *tp = 255;
		}
	}

	return( im );
}