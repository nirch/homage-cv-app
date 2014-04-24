 /**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"

static image_type *	image1_sample3( image_type *sim, image_type *im );

static image_type *image1_sample4( image_type *sim, image_type *im );



image_type *
image1_sample2( image_type *sim, image_type *im )
{
u_char	*sp0,	*sp1, *sp2,	*sp3,	*tp;
int	i,	j;


	im = image_recreate( im, sim->row/2, sim->column/2, 1, 1 );


	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 2*i, 0 );
		sp1 = sp0+1;
		sp2 = sp0 + sim->width;
		sp3 = sp2 + 1;

		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = ( *(sp0++) + *(sp1++) + *(sp2++) + *(sp3++) ) >>2;
	
			sp0 += 1;
			sp1 += 1;
			sp2 += 1;
			sp3 += 1;
		}
	}

	return( im );
}


image_type *
image1_sample2L( image_type *sim, int level, image_type *im )
{
	image_type	*tim;
	int	i;

	if( level == 2 ){
		im = image1_sample4( sim, im );
		return( im );
	}

	im = image1_sample2( sim, im  );

	for( i = 1 ; i < level ; i++ ){
		tim = image1_sample2( im, NULL );
		image_destroy( im, 1 );
		im = tim;
	}

	return( im );
}


image_type *
image1_dup2( image_type *sim, image_type *im )
{
u_char	*sp,	*tp0,	*tp1;
int	i,	j;
	
	im = image_recreate( im, sim->row*2, sim->column*2, 1, 1 );


	sp = sim->data;
	tp0 = im->data;
	for( i = 0 ; i < sim->row ; i++){
		tp0 = IMAGE_PIXEL( im, i*2, 0 );
		tp1 = tp0 + im->width;

		for( j = 0 ; j < sim->column ; j++, sp++ ){
			*tp0++ = *sp;
			*tp0++ = *sp;

			*tp1++ = *sp;
			*tp1++ = *sp;
		}
	}

	return( im );
}



image_type *
image1_sample( image_type *sim, int d, image_type *im )
{
u_char	*sp,	*sp0,	*tp;
int	i,	j;
int	id;
int	k,	n;
int	align,	sum;

	if( d == 2 )
		return image1_sample2( sim, im );


	if( d == 3 )
		return image1_sample3( sim, im );

	im = image_recreate( im, sim->row/d, sim->column/d, 1, 1 );

	align = sim->width - d;
	tp = im->data;
	for( i = 0, id = 0 ; i < im->row ; i++, id += d ){

		sp0 = IMAGE_PIXEL( sim, id, 0 );
		for( j = 0 ; j < im->column ; j++, sp0 += d ){
	
			for( k = 0, sp = sp0, sum = 0 ; k < d ; k++, sp += align ){
				for( n = 0 ; n < d ; n++ )
					sum += *sp++;
			}

			*tp++ = sum /( d*d );
		}
	}

	return( im );
}



static image_type *
image1_sample3( image_type *sim, image_type *im )
{
	u_char	*sp0,	*sp1, *sp2,	*tp;
	int	i,	j;
	float	tmp;




	tmp = 1.0/(3.0*3.0 );

	im = image_realloc( im, sim->width/3, sim->height/3, 1, IMAGE_TYPE_U8, 1 );


	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 3*i, 0 );
		sp1 = sp0 + sim->width;
		sp2 = sp1 + sim->width;

		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = (sp0[0] + sp0[1] + sp0[2] + sp1[0] + sp1[1] + sp1[2] + sp2[0] + sp2[1] + sp2[2]) * tmp;
	
			sp0 += 3;
			sp1 += 3;
			sp2 += 3;
		}
	}

	return( im );
}


static image_type *
image1_sample4( image_type *sim, image_type *im )
{
	u_char	*sp0,	*sp1, *sp2,	*sp3,	*tp;
	int	i,	j;


	im = image_realloc( im, sim->width/4, sim->height/4, 1, IMAGE_TYPE_U8, 1 );


	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){
		sp0 = IMAGE_PIXEL( sim, 4*i, 0 );
		sp1 = sp0 + sim->width;
		sp2 = sp1 + sim->width;
		sp3 = sp2 + sim->width;


		for( j = 0 ; j < im->column ; j++ ){

			*tp++ = (	sp0[0] + sp0[1] + sp0[2] + sp0[3] +
						sp1[0] + sp1[1] + sp1[2] + sp1[3] +
						sp2[0] + sp2[1] + sp2[2] + sp2[3] +
						sp3[0] + sp3[1] + sp3[2] + sp3[3])  >>4;

			sp0 += 4;
			sp1 += 4;
			sp2 += 4;
			sp3 += 4;

		}
	}

	return( im );
}

//image_type *
//image1_sample_column( image_type *sim, int si0, int d, image_type *im, int i0 )
//{
//u_char	*sp,	*sp0,	*tp;
//int	j;
//int	k,	n;
//int	align,	sum;
//
//	align = sim->width - d;
//	tp = IMAGE_PIXEL( im, i0, 0 );
//
//
//	sp0 = IMAGE_PIXEL( sim, si0, 0 );
//
//	for( j = 0 ; j + d < im->column ; j++, sp0 += d ){
//
//		for( k = 0, sp = sp0, sum = 0 ; k < d ; k++, sp += align ){
//			for( n = 0 ; n < d ; n++ )
//				sum += *sp++;
//		}
//
//		*tp++ = sum /( d*d );
//	}
//
//
//	return( im );
//}


image_type *
image1_dup( image_type *sim, int d, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;
	int	id,	jd;
	int	k,	n;


	if( d == 2 )
		return image1_dup2( sim, im );



	im = image_recreate( NULL, sim->row*d, sim->column*d, 1, 1 );


	sp = sim->data;
	for( i = 0, id = 0 ; i < sim->row ; i++, id += d ){

		for( j = 0, jd = 0 ; j < sim->column ; j++, jd += d, sp++ ){

			for( k = id ; k < id+d ; k++ ){
				tp = IMAGE_PIXEL( im, k, jd );

				for( n = jd ; n < jd+d ; n++ )
					*tp++ = *sp;
			}
		}
	}

	return( im );
}




image_type *
image1_sample_mathlab( image_type *sim, int d, image_type *im )
{
image_type	*cim;
int	rC,	i,	j,	i0,	j0;
float	g[100];
float	sum;
u_char	*sp,	*tp;


	rC = d;
	for (i = 0, sum = 0 ; i < d ; i++ ) {
		g[i] = 2*i+1;
		g[2*d-1-i] = 2*i+1;
		sum += g[i] + g[2*d-1-i];
	}
	for (i = 0; i < d*2; i++)
		g[i] /= sum;

	g[d*2] = 0;

	
	cim = image1_convolution_separably( sim, g, g, rC, NULL );



	im = image_recreate( im, sim->row/d, sim->column/d, 1, 1 );


	tp = im->data;
	for( i = 0 ; i < im->row ; i++ ){

		i0 = i*d+d/2-1;

		j0 = 0*d+d/2-1;

		sp = IMAGE_PIXEL( cim, i0, j0 );
		for( j = 0 ; j < im->column ; j++, sp += d, tp++ ){
			*tp = *sp;
		}
	}


	image_destroy( cim, 1 );

	return( im );
}