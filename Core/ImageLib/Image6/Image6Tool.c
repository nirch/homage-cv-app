/************************
 ***   Image3Tool.c   ***
 ************************/
#include	<string.h>

#include	"ImageType/ImageType.h"


#define         PIXEL_12_TO_8( x )   \
	      		( ( (x) <= 0 )? 0 : (( (x) > 4096 )? 255 : ((x)>>4) ) )



image_type *
image6_to_image3( image_type *sim, image_type *im )
{
//image_type	*im;
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;


	if( im == NULL )
		im = image_create( sim->row, sim->column, 3 , 1, NULL );

	sp = (u_short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

	return( im );
}


image_type *
image6_to_image4( image_type *sim, image_type *im )
{
short	*sp;
u_int	*tp;
int	i,	j;
int	R,	G,	B,	tmp;


	if( im == NULL )
		im = image_create( sim->row, sim->column, 4 , 1, NULL );

	sp = (u_short *)sim->data;
	tp = (u_int *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *sp >> 4;
			sp++;
			R = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *sp >> 4;
			sp++;
			G = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *sp >> 4;
			sp++;
			B = PUSH_TO_RANGE(tmp, 0, 255 );

			*tp++ = IMAGE4_RGB( R, G, B );
		}
	}

	return( im );
}

image_type *
image6_to_image3_BGR( image_type *sim, image_type *im )
{
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;


	if( im == NULL )
		im = image_create( sim->row, sim->column, 3 , 1, NULL );

	sp = (u_short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *(sp+2) >> 4;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *(sp+1) >> 4;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			tmp = *(sp) >> 4;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
			
			sp += 3;
		}
	}

	return( im );
}




image_type *
image6_to_image3_l( image_type *sim )
{
image_type	*im;
short	*sp;
u_char	*tp;
int	i,	j;
int	tmp;

	im = image_create( sim->row, sim->column, 3 , 1, NULL );

	sp = (u_short *)sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = PUSH_TO_RANGE(*sp, 0, 4095 );
			sp++;
			*tp++ = tmp & 0xF;

			tmp = PUSH_TO_RANGE(*sp, 0, 4095 );
			sp++;
			*tp++ = tmp & 0xF;

			tmp = PUSH_TO_RANGE(*sp, 0, 4095 );
			sp++;
			*tp++ = tmp & 0xF;
		}
	}

	return( im );
}


void
image6_to_image3_2( image_type *sim, image_type **mim, image_type **lim )
{
short	*sp;
u_char	*tp,	*tpl;
int	i,	j;
int	tmp;

	*mim = image_create( sim->row, sim->column, 3 , 1, NULL );
	*lim = image_create( sim->row, sim->column, 3 , 1, NULL );

	sp = (u_short *)sim->data;
	tp = (*mim)->data;
	tpl = (*lim)->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			*tpl++ = *sp & 0xF;
			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			*tpl++ = *sp & 0xF;
			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );

			*tpl++ = *sp & 0xF;
			tmp = *sp >> 4;
			sp++;
			*tp++ = PUSH_TO_RANGE(tmp, 0, 255 );
		}
	}

}


void
image6_force_range( image_type *im )
{
short	*sp;
int	i,	j;

	sp = (short *)im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			if( *sp < 0 )	*sp = 0;
			else	if( *sp >= 4095 )	*sp = 4095;
			sp++;

			if( *sp < 0 )	*sp = 0;
			else	if( *sp >= 4095 )	*sp = 4095;
			sp++;

			if( *sp < 0 )	*sp = 0;
			else	if( *sp >= 4095 )	*sp = 4095;
			sp++;
		}
	}
}


void
image6_const( image_type *im, int r, int g, int b )
{
u_short	*tp;
int	i,	j;

	tp = (u_short *)im->data;


	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = r;
			*tp++ = g;
			*tp++ = b;
		}
	}
}

void
image6_dynamic_range( image_type *im, float h0, float h1, float maxD )
{
short	*tp;
int	i,	j;

int	h[4096];
int	sum,	max,	tmp;
float	a0,	a1,	a,	b;



	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;

	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			tmp = *tp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;

			tmp = *tp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;

			tmp = *tp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;
		}
	}



	max = im->row *im->column *3 * h0/100;
	for( i = 0, sum = 0 ; i < 4096 ; i++ ){
		sum += h[i];
		if( sum > max )	break;
	}
		
	a0 = i -1;
	if( maxD > 0 && a0 > maxD*16 )	a0 = maxD*16;



	max = im->row *im->column *3 * h1/100;
	for( i = 4096-1, sum = 0 ; i > 0 ; i-- ){
		sum += h[i];
		if( sum > max )	break;
	}

	a1 = i + 1;
	if( maxD > 0 && a1 < 4096 - maxD*16 )	a1 = 4096 - maxD*16;
	


	a = 4095/(a1-a0);
	b = - a* a0;

	

	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = a * (*tp) + b;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );
						
			tmp = a * (*tp) + b;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

			tmp = a * (*tp) + b;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );
		}
	}
}


void
image6_dynamic_range_YCrCb( image_type *im, float h0, float h1, float maxD )
{
image_type	*imY,	*imCr,	*imCb;

	image6_to_YCrCb( im, &imY, &imCr, &imCb );

	image2_dynamic_range( imY, NULL, h0, h1, maxD );

	im = image6_YCrCb_to_RGB( imY, imCr, imCb, im );
}


int
image6_black( image_type *im, float h0 )
{
short	*tp;
int	i,	j;

int	h[4096];
int	sum,	max,	tmp;



	for( i = 0 ; i < 4096 ; i++ )	h[i] = 0;

	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			tmp = *tp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;

			tmp = *tp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;

			tmp = *tp++;
			tmp = PUSH_TO_RANGE(tmp, 0, 4095 );
			h[tmp]++;
		}
	}



	max = im->row *im->column *3 * h0/100;
	for( i = 0, sum = 0 ; i < 4096 ; i++ ){
		sum += h[i];
		if( sum > max )	break;
	}

	return( i / 16 );
}



float
image6_averageY( image_type *im, float min, float max )
{
short	*tp;
int	i,	j,	no;

int	sum,	Y;
int	R,	G,	B,	tmp;



	no = 0;
	sum = 0;

	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			tmp = *tp++;
			R = PUSH_TO_RANGE(tmp, 0, 4095 );
	
			tmp = *tp++;
			G = PUSH_TO_RANGE(tmp, 0, 4095 );

			tmp = *tp++;
			B = PUSH_TO_RANGE(tmp, 0, 4095 );

			Y   =  0.29900 * R   + 0.58700 * G  + 0.11400 * B;
			Y /= 16;

			if( Y < min || Y > max )	continue;

			sum += Y;
			no++;
		}
	}

	sum /= no;

	return( sum );
}



image_type *
image6_subtruct( image_type *im1, image_type *im0, image_type *im )
{
short	*sp1,	*sp0,	*tp;
int	i,	j;

	if( im == NULL )
		im = image_create( im1->row, im1->column, 6, 1, NULL );

	sp1 = (short *)im1->data;
	sp0 = (short *)im0->data;
	tp = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = (1<<11) + *sp1 - *sp0;
			sp1++; sp0++;

			*tp++ = (1<<11) + *sp1 - *sp0;
			sp1++; sp0++;

			*tp++ = (1<<11) + *sp1 - *sp0;
			sp1++; sp0++;
		}

	return( im );
}



//#define	FR	19595		// 0.29900 * ( 1<<16)
//#define FG	38469		// 0.58700 * ( 1<<16);
//#define FB	7471		// 0.11400 * ( 1<<16);

#define	FIX16( a )	( (int)(a*(1<<16)))

image_type *
image6_to_y( image_type *sim )
{
short	*sp;
short	*p;
image_type	*im;
int	row,	col,	i,	j;
int	R,	G,	B;
#ifdef MEP_FLOAT
int	Y;
#endif

	row = IMAGE_ROW(sim);
	col = IMAGE_COLUMN(sim);

	im = image_create( row, col, 2, 1, NULL );

	sp = (short *)sim->data;
	p = (short *)im->data;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			R = *sp++;
			G = *sp++;
			B = *sp++;

#ifdef MEP_FLOAT
			Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B + 0.5;
			*p++ = PUSH_TO_RANGE( Y, 0, 255 );
#else
			*p++ = (FIX16(0.29900) * R + FIX16(0.58700) * G + FIX16(0.11400) * B) >> 16;
//			*p++ = (R*FR + G *FG + B *FB)>>16;
#endif



		}
	}

	return( im );
}



image_type *
image6_mult( image_type *sim, float fr, float fg, float fb, image_type *im )
{
short	*sp;
short	*tp;
int	i,	j;
int	tmp;


	if( im == NULL )
		im = image_create( sim->row, sim->column, 6, 1, NULL );

	sp = (short *)sim->data;
	tp = (short *)im->data;



	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			tmp = fr * *sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

			tmp = fg * *sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

			tmp = fb * *sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

		}
	}

	return( im );
}

image_type *
image6_smooth_mult( image_type *sim, float fr, float fg, float fb, image_type *im )
{
short	*sp,	*cp;
short	*tp;
int	i,	j;
int	tmp;

float	g[10];
image_type	*cim;

	guasian( 4, 2.5, g );
	cim = image6_convolution_separably( sim, g, g, 4, NULL );

	if( im == NULL )
		im = image_create( sim->row, sim->column, 6, 1, NULL );

	sp = (short *)sim->data;
	cp = (short *)cim->data;
	tp = (short *)im->data;



	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			tmp = (fr-1) * *cp + *sp;
			cp++; sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

			tmp = (fg-1) * *cp + *sp;
			cp++; sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

			tmp = (fb-1) * *cp + *sp;
			cp++; sp++;
			*tp++ = PUSH_TO_RANGE( tmp, 0, 4095 );

		}
	}


	image_destroy( cim, 1 );

	return( im );
}

void
image6_accumulate( image_type *tim, image_type *sim, float a )
{
short	*tp;
short	*sp;
int	i,	j;


	sp = (short *)sim->data;
	tp = (short *)tim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp += 3, tp += 3 ){
			*tp     += a * *sp;
			*(tp+1) += a * *(sp+1);
			*(tp+2) += a * *(sp+2);
		}
	}
}

void
image6_sum( image_type *tim, image_type *sim )
{
short	*tp;
short	*sp;
int	i,	j;


	sp = (short *)sim->data;
	tp = (short *)tim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp += 3, tp += 3 ){
			*tp += *sp;
			*(tp+1) += *(sp+1);
			*(tp+2) += *(sp+2);
		}
	}
}


void
image6_compare( image_type *tim, image_type *sim, short D[3] )
{
short	*tp;
short	*sp;
int	i,	j;
int	tmp;


	D[0] = D[1] = D[2] = 0;
	sp = (short *)sim->data;
	tp = (short *)tim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			if( (tmp = *tp - *sp ) < 0 )	tmp = -tmp;
			if( tmp > D[0] )	D[0] = tmp;
			tp++;
			sp++;

			if( (tmp = *tp - *sp ) < 0 )	tmp = -tmp;
			if( tmp > D[1] )	D[1] = tmp;
			tp++;
			sp++;

			if( (tmp = *tp - *sp ) < 0 )	tmp = -tmp;
			if( tmp > D[2] )	D[2] = tmp;
			tp++;
			sp++;
		}
	}
}



void
image6_unpack( image_type *im, image_type **imR, image_type **imG, image_type **imB )
{
short	*sp;
short	*pR,	*pG,	*pB;

int	width,	height,	i,	j;


	height = IMAGE_ROW(im);
	width = IMAGE_COLUMN(im);


	*imR = image_create( height, width, 2, 1, NULL );
	*imG = image_create( height, width, 2, 1, NULL );
	*imB = image_create( height, width, 2, 1, NULL );



	sp = (short *)im->data;

	pR = (short *)(*imR)->data;
	pG = (short *)(*imG)->data;
	pB = (short *)(*imB)->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*pR++ = *sp++;
			*pG++ = *sp++;
			*pB++ = *sp++;
		}
	}
}





image_type *
image6_pack( image_type *imR, image_type *imG, image_type *imB, image_type *im )
{
short	*sp;
short	*pR,	*pG,	*pB;

int	i,	j;



//	if( im == NULL )
	im = image_recreate( im, IMAGE_ROW(imR), IMAGE_COLUMN(imR), 6, 1 );



	sp = (short *)im->data;

	pR = (short *)(imR)->data;
	pG = (short *)(imG)->data;
	pB = (short *)(imB)->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){
			*sp++ = *pR++;
			*sp++ = *pG++;
			*sp++ = *pB++;
		}
	}

	return( im );
}




image_type *
image3_add_21( image_type *sim, image_type *im )
{
	u_short	*tp;
	u_char	*sp;
	int	i,	j;

	if( im == NULL )
		im = image_create( sim->row, sim->column, 2 , 1, NULL );

	sp = sim->data;
	tp = im->data_s;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ += *sp++;
			*tp++ += *sp++;
			*tp++ += *sp++;
		}
	}

	return( im );
}


image_type *
image6_average_21( image_type *sim, int no, image_type *im )
{
	u_short	*sp;
	u_char	*tp;
	int	i,	j;
	float	ninv;

	if( im == NULL )
		im = image_create( sim->row, sim->column, 3 , 1, NULL );

	ninv = 1.0 / no;

	sp = sim->data_s;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = (*sp++) * ninv + 0.5;
			*tp++ = (*sp++) * ninv + 0.5;
			*tp++ = (*sp++) * ninv + 0.5;
		}
	}

	return( im );
}