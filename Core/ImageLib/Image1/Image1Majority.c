/******************************
 ***   Image1Magority.cpp   ***
 ******************************/

//#ifdef _DEBUG
#define _DUMP
//#endif

#include "ImageType/ImageType.h"
#include "Image1Tool.h"


image_type *
image1_majority( image_type *sim, int N, int mH, image_type *im )
{
	int	i,	j,	n,	k;
	u_char	*sp,	*tp;

	int	h[255];
	box2i	b;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	if( sim->palette != NULL )
		im->palette = palette_copy( sim->palette, NULL );



	tp = im->data;
	for( i = 0 ; i < im->height ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );

		if( (b.y0 = i-N) < 0 )	b.y0 = 0;
		if( (b.y1 = i+N) > sim->height )	b.y1 = im->height;

		for( j = 0 ; j < im->width ; j++, tp++ ){

			for( k = 0; k < mH ; k++ )
				h[k] = 0;

			if( (b.x0 = j-N) < 0 )	b.x0 = 0;
			if( (b.x1 = j+N) > sim->width )	b.x1 = im->width;


			for( k = b.y0 ; k < b.y1 ; k++ ){
				sp = IMAGE_PIXEL( sim, k, b.x0 );
				for( n = b.x0 ; n < b.x1 ; n++, sp++ )
					h[*sp]++;
			}

			n = 0;
			for( k = 1 ; k < mH ; k++ )
				if( h[k] > h[n] )	n = k;

 
			*tp = n;

		}
	}


	return( im );
}

image_type *
image1M_majority_O( image_type *sim, image_type *mim, int N, int mH, image_type *im )
{
	int	i,	j,	n,	k;
	u_char	*sp,	*mp,	*mp0,	*tp;

	int	h[256];
	box2i	b;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	if( sim->palette != NULL )
		im->palette = palette_copy( sim->palette, NULL );



	tp = im->data;
	mp0 = mim->data;
	for( i = 0 ; i < im->height ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );

		if( (b.y0 = i-N) < 0 )	b.y0 = 0;
		if( (b.y1 = i+N) > sim->height )	b.y1 = im->height;

		for( j = 0 ; j < im->width ; j++, tp++, mp0++ ){

			if( *mp0 == 0 )	continue;

			for( k = 0; k < mH ; k++ )
				h[k] = 0;

			if( (b.x0 = j-N) < 0 )	b.x0 = 0;
			if( (b.x1 = j+N) > sim->width )	b.x1 = im->width;


			for( k = b.y0 ; k < b.y1 ; k++ ){
				sp = IMAGE_PIXEL( sim, k, b.x0 );
				mp = IMAGE_PIXEL( mim, k, b.x0 );

				for( n = b.x0 ; n < b.x1 ; n++, sp++, mp++ )
					if( *mp != 0 )
						h[*sp]++;
			}

			n = 0;
			for( k = 1 ; k < mH ; k++ )
				if( h[k] > h[n] )	n = k;


			*tp = n;

		}
	}


	return( im );
}


image_type *
image1M_majority( image_type *sim, image_type *mim, int N, int mH, image_type *im )
{
	int	i,	j,	n,	k,	x0;
	u_char	*sp,	*mp,	*mp0,	*tp;

	int	h[256];
	box2i	b;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	if( sim->palette != NULL )
		im->palette = palette_copy( sim->palette, NULL );



	tp = im->data;
	mp0 = mim->data;
	for( i = 0 ; i < im->height ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );

		if( (b.y0 = i-N) < 0 )	b.y0 = 0;
		if( (b.y1 = i+N) > sim->height )	b.y1 = im->height;

		for( k = 0; k < mH ; k++ )
			h[k] = 0;


		for( k = b.y0 ; k < b.y1 ; k++ ){
			sp = IMAGE_PIXEL( sim, k, 0 );
			mp = IMAGE_PIXEL( mim, k, 0 );

			for( n = 0 ; n < N-1 ; n++, sp++, mp++ )
				if( *mp != 0 )
					h[*sp]++;
		}

		for( j = 0 ; j < im->width ; j++, tp++, mp0++ ){



			x0 = j - N-1;
			if( x0 >= 0 ){
				sp = IMAGE_PIXEL( sim, b.y0, x0 );
				mp = IMAGE_PIXEL( mim, b.y0, x0 );
				for( k = b.y0 ; k < b.y1 ; k++, sp+= sim->width , mp += mim->width ){
					if( *mp != 0 )
						h[*sp] --;
				}
			}

			x0 = j + N-1;
			if( x0 < sim->width ){
				sp = IMAGE_PIXEL( sim, b.y0, x0 );
				mp = IMAGE_PIXEL( mim, b.y0, x0 );
				for( k = b.y0 ; k < b.y1 ; k++, sp+= sim->width , mp += mim->width ){
					if( *mp != 0 )
						h[*sp]++;
				}
			}

			if( *mp0 == 0 )	continue;

			n = 0;
			for( k = 1 ; k < mH ; k++ )
				if( h[k] > h[n] )	n = k;


			*tp = n;

		}
	}


	return( im );
}