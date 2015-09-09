/*****************************
 ***   Image1FilterBox.c   ***
 ****************************/
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


image_type *
image1_filter_box( image_type *sim, int n, image_type *im )
{
image_type *tim;
int	i,	j,	n2;
u_char	*tp;
u_int	*ip,	*ip0,	*ip1,	*ip2;
float	t,	t1;
int	i0,	j0,	i1;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	tim = image1_integral( sim, NULL );


	n2 = 2*n + 1;
	t = 1.0 / ( n2*n2 );


	tp = im->data;
	for( i = 0 ; i < sim->height ; i++ ){

		i1 = i-n;
		i0 = i+1+n;
		if( i1 < 0 )
			i1 = 0;
		if( i0 > tim->height-1 )
			i0 = tim->height-1;
		n2 = i0 - i1;

		j0 = n+1;
		ip = (u_int *)IMAGE_PIXEL( tim, i0,	j0 );
		ip0 = (u_int *)IMAGE_PIXEL( tim, i1, 0 );
		ip1 = (u_int *)IMAGE_PIXEL( tim, i1, j0 );
		ip2 = (u_int *)IMAGE_PIXEL( tim, i0, 0 );


		for( j = 0 ; j < n ; j++,  ip++, ip1++ ){
			t1 = 1.0 / ( n2*(n+1 + j) );
			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t1 + 0.5;
		}


		t =  1.0/(n2*(2*n+1));
		for(  ; j < sim->width - n ; j++, ip++, ip0++,ip1++, ip2++ ){
			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t + 0.5;
		}


		ip1--;
		ip--;

		for(  ; j < sim->width ; j++, ip0++, ip2++ ){
			t1 = 1.0 / ( n2*(n+1 + sim->width -1- j) );
			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t1 + 0.5;
		}
	}

	image_destroy( tim, 1 );

	return( im );
}


image_type *
image1_filter_boxO( image_type *sim, int n, image_type *im )
{
image_type *tim;
int	i,	j,	n2;
u_char	*sp,	*tp;
u_int	*ip,	*ip0,	*ip1,	*ip2;
float	t;
int	i0,	j0;
	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	tim = image1_integral( sim, NULL );


	n2 = 2*n + 1;
	t = 1.0 / ( n2*n2 );

	sp = sim->data;
	tp = im->data;
	for( i = 0 ; i < n ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){
			*tp++ = *sp++;
		}
	}

	

	for(  ; i < sim->height - n ; i++ ){
		for( j = 0 ; j < n+1 ; j++ ){
			*tp++ = *sp++;
		}

		i0 = i+1 + n;
		j0 = n+1 + n;
		ip = (u_int *)IMAGE_PIXEL( tim, i0,	j0 );
		ip0 = (u_int *)IMAGE_PIXEL( tim, i0 - n2, 0 );
		ip1 = (u_int *)IMAGE_PIXEL( tim, i0 - n2, n2 );
		ip2 = (u_int *)IMAGE_PIXEL( tim, i0, 0 );

		for(  ; j < sim->width - n ; j++, sp++, ip++, ip0++,ip1++, ip2++ ){
			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t + 0.5;
		}


		for(  ; j < sim->width ; j++ ){
			*tp++ = *sp++;
		}
	}


	for(  ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++ ){
			*tp++ = *sp++;
		}
	}


	image_destroy( tim, 1 );

	return( im );
}



//image_type *
//image1_filter_box1( image_type *sim, int n, image_type *im )
//{
//	image_type *tim;
//	int	i,	j,	n2;
//	u_char	*sp,	*tp;
//	u_int	*ip,	*ip0,	*ip1,	*ip2;
//	float	t,	t1;
//	int	i0,	j0;
//	im = image_recreate( im, sim->height, sim->width, 1, 1 );
//
//	tim = image1_integral( sim, NULL );
//
//
//	n2 = 2*n + 1;
//	t = 1.0 / ( n2*n2 );
//
//	sp = sim->data;
//	tp = im->data;
//	for( i = 0 ; i < n ; i++ ){
//		for( j = 0 ; j < sim->width ; j++ ){
//			*tp++ = *sp++;
//		}
//	}
//
//
//
//	for(  ; i < sim->height - n ; i++ ){
//		i0 = i+1 + n;
//		j0 = n+1;
//		ip = (u_int *)IMAGE_PIXEL( tim, i0,	j0 );
//		ip0 = (u_int *)IMAGE_PIXEL( tim, i0 - n2, 0 );
//		ip1 = (u_int *)IMAGE_PIXEL( tim, i0 - n2, j0 );
//		ip2 = (u_int *)IMAGE_PIXEL( tim, i0, 0 );
//
//
//		for( j = 0 ; j < n ; j++,  ip++, ip1++ ){
//			t1 = 1.0 / ( n2*(n+1 + j) );
//			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t1 + 0.5;
//		}
//
//
//		for(  ; j < sim->width - n ; j++, ip++, ip0++,ip1++, ip2++ ){
//			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t + 0.5;
//		}
//
//
//		ip1--;
//		ip--;
//
//		for(  ; j < sim->width ; j++, ip0++, ip2++ ){
//			t1 = 1.0 / ( n2*(n+1 + sim->width -1- j) );
//			*tp++ = ( *ip + *ip0 - *ip1 - *ip2 )*t1 + 0.5;
//		}
//	}
//
//
//
//	for(  ; i < sim->height ; i++ ){
//		for( j = 0 ; j < sim->width ; j++ ){
//			*tp++ = *sp++;
//		}
//	}
//
//
//	image_destroy( tim, 1 );
//
//	return( im );
//}







image_type *                                      
image1_integral( image_type *sim, image_type *im )            
{                                                               
int	i,	j;
u_int	*tp,	*tp0;
u_char	*sp;
int s;   


	im = image_recreate( im, sim->height+1, sim->width+1, 4, 1 );

	memset( im->data, 0, im->width*sizeof(int));             

	sp = sim->data;
	tp0 = im->data_ui;
	tp = (u_int *)IMAGE_PIXEL( im, 1, 0 );
	for( i = 0 ; i  < sim->height ; i++ ){
		*tp++ = *tp0++;

		for( j = 0, s = 0 ; j < sim->width ; j++, sp++, tp++, tp0++ ){
			s += *sp;
			*tp = *tp0 + s;
		}
	}

	return( im );
}




