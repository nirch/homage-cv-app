
#include "ImageDump/ImageDump.h"
#include "bImage.h"


#include "ImageLabel/SbType.h"


image_type *
bImage_set( image_type *sim, image_type *mim, box2i_type *b, int N, image_type *bim )
{
	int	i,	j,	k,	n,	i0,	j0;
	u_char	*sp,	*mp;
	bImage_type *bp,	*bp0;


	bim = bImage_realloc( b, N,  bim );

	bImage_clear( bim );




	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= sim->height-1 )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );
			mp = IMAGE_PIXEL( mim, i0, b->x0 );



			for( j = 1, j0 = b->x0, bp = bp0 ; j < bim->width-1 ; j++, j0 += N, bp++ ){
				int n1 = N;
				if( j0 + n1 > sim->width )	n1 = sim->width - j0;

				for( n = 0 ; n < n1 ; n++, sp += 3, mp++ ){
					if( *mp != 0 )
						continue;

					bp->r += sp[0];
					bp->g += sp[1];
					bp->b += sp[2];
					bp->n++;
				}
			}
		}
	}


	bp = ( bImage_type *)bim->data;
	for( i = 0 ; i < bim->height ; i++ ){

		for( j = 0 ; j < bim->width ; j++, bp++ ){
			if( bp->n < 0.5*n*n ){
				bp->n = 0;
				continue;
			}

			bp->r /= bp->n;
			bp->g /= bp->n;
			bp->b /= bp->n;
		}
	}

	return( bim );
}


image_type *
bImage_set( image_type *sim, box2i_type *b, int N, image_type *bim )
{
	int	i,	j,	k,	n,	i0,	j0;
	u_char	*sp;
	bImage_type *bp,	*bp0;


	bim = bImage_realloc( b, N,  bim );

	bImage_clear( bim );



	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= sim->height-1 )
				break;

			sp = IMAGE_PIXEL( sim, i0, b->x0 );

			for( j = 1, j0 = b->x0, bp = bp0 ; j < bim->width-1 ; j++, j0 += N, bp++ ){
				int n1 = N;
				if( j0 + n1 > sim->width )	n1 = sim->width - j0;

				for( n = 0 ; n < n1 ; n++, sp += 3 ){

					bp->r += sp[0];
					bp->g += sp[1];
					bp->b += sp[2];
					bp->n++;
				}
			}
		}
	}


	bp = ( bImage_type *)bim->data;
	for( i = 0 ; i < bim->height ; i++ ){

		for( j = 0 ; j < bim->width ; j++, bp++ ){
			if( bp->n == 0 )	continue;

			bp->r /= bp->n;
			bp->g /= bp->n;
			bp->b /= bp->n;

			bp->n = N*N;
		}
	}




	return( bim );
}


void
bImage_mask8( image_type *bim, image_type *mim )
{
	int	i,	j;
	u_char	*mp;
	bImage_type *bp;

	mp = IMAGE_PIXEL( mim, 0, 0 );
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 1 );

	for( i = 1 ; i < bim->height-1 ; i++, bp += 2 ){
		for( j = 1 ; j < bim->width-1 ; j++, bp++, mp++ ){
			if( *mp == 255 )
				bp->n = 0;
		}
	}
}

void
bImage_mask( image_type *bim, image_type *mim, box2i_type *b, int N )
{
	int	i,	j,	k,	n,	i0,	j0;
	u_char	*mp;
	bImage_type *bp,	*bp0;


	for( i = 1, i0 = b->y0 ; i < bim->height-1 ; i++ ){
		bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 1 );

		for( k = 0 ; k < N ; k++, i0 += 1 ){

			if( i0 >= mim->height-1 )
				break;

			mp = IMAGE_PIXEL( mim, i0, b->x0 );

			for( j = 1, j0 = b->x0, bp = bp0 ; j < bim->width-1 ; j++, j0 += N, bp++ ){
				int n1 = N;
				if( j0 + n1 > mim->width )	n1 = mim->width - j0;

				for( n = 0 ; n < n1 ; n++, mp++ ){
					if( *mp != 0 )
						bp->n--;
				}
			}
		}
	}

	bp = ( bImage_type *)bim->data;
	for( i = 0 ; i < bim->height ; i++ ){

		for( j = 0 ; j < bim->width ; j++, bp++ ){
			if( bp->n != N*N )
				bp->n = 0;
		}
	}
}






image_type *
	bImage_realloc(  box2i_type *b, int n, image_type *bim )
{

	int	width,	height,	w,	h;
	width = b->x1 - b->x0;
	w = width/n;
	if( width - w*n > 0.25*n )	w++;

	height = b->y1 - b->y0;
	h = height/n;
	if( height - h*n > 0.25*n )	h++;


	if( bim != NULL && ( bim->width != w + 2 || bim->height != h + 2 )){
		image_destroy( bim, 1 );
		bim = NULL;
	}

	if( bim == NULL )
		bim = image_create( h+2, w+2, sizeof(bImage_type), 1, NULL );




	return( bim );
}



void
	bImage_clear( image_type *bim )
{
	int	i,	j;

	bImage_type *bp = (bImage_type *)bim->data;

	for( i = 0 ; i < bim->height ; i++ ){
		for( j = 0 ; j < bim->width ; j++, bp++ ){
			bp->n= 0;
			bp->r = bp->g = bp->b = 0;
		}
	}

}

void
	bImage_clearN( image_type *bim )
{
	int	i,	j;

	bImage_type *bp = (bImage_type *)bim->data;

	for( i = 0 ; i < bim->height ; i++ ){
		for( j = 0 ; j < bim->width ; j++, bp++ ){
			if( bp->n == 0 )
				bp->n = 1;
		}
	}

}

image_type *
	bImage_copy( image_type *bim, image_type *tim  )
{
	int	i,	j;

	if( tim == NULL )
		tim = image_create( bim->height, bim->width, sizeof(bImage_type), 1, NULL );


	bImage_type *bp = (bImage_type *)bim->data;
	bImage_type *tp = (bImage_type *)tim->data;

	for( i = 0 ; i < bim->height ; i++ ){
		for( j = 0 ; j < bim->width ; j++, bp++, tp++ ){
			*tp = *bp;
		}
	}

	return( tim );
}



image_type *
	bImage_to_image1( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 1, IMAGE_TYPE_U8, 1 );



	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	u_char *tp = im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){
		for( j = 0 ; j < im->width ; j++, bp++ ){

			*tp++ = IMAGE_RGB2Y( bp->r, bp->g, bp->b );

		}
	}

	return( im );
}

image_type *
	bImage_to_image3( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 3, IMAGE_TYPE_U8, 1 );



	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	u_char *tp = im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){
		for( j = 0 ; j < im->width ; j++, bp++ ){

			*tp++ = bp->r;
			*tp++ = bp->g;
			*tp++ = bp->b;
		}
	}

	return( im );
}


image_type *
	bImage_to_image( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 3, IMAGE_TYPE_U8, 1 );



	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	u_char *tp = im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){
		for( j = 0 ; j < im->width ; j++, bp++ ){

			if( bp->n == 0 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}
			*tp++ = bp->r;
			*tp++ = bp->g;
			*tp++ = bp->b;
		}
	}

	return( im );
}




void
bImage_dump( image_type *bim, int N, char *prefix, int index, char *suffix )
{

	image_type *im = bImage_to_image(  bim, NULL );

	image_dump_dup( im, N, 1,prefix, index, suffix );

	image_destroy( im, 1 );

}




image_type *
bImage_grad_x_c( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 1, IMAGE_TYPE_U8, 1 );

	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	s_char *tp = (s_char *)im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){


		int	dc;
		for( j = 0 ; j < im->width ; j++, bp++ ){

			bImage_type *bp0 = ( j == 0 )? bp : bp-1;
			bImage_type *bp1 = ( j < im->width-1)? bp+1 : bp;

			int r = bp1->r - bp0->r;
			int g = bp1->g - bp0->g;
			int b = bp1->b - bp0->b;

			dc = r;
			if( ABS(dc) < ABS(g) )
				dc = g;

			//*tp++ = dc;
			*tp++ = PUSH_TO_RANGE(dc, -127, 128 );
		}
	}

	return( im );
}


image_type *
bImage_grad_y_c( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 1, IMAGE_TYPE_U8, 1 );

	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	s_char *tp = (s_char *)im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){


		int	dc;
		bImage_type *yp0 = ( i == 0 )? bp : bp-bim->width;
		bImage_type *yp1 = ( i < im->height-1)? bp+bim->width : bp;
		for( j = 0 ; j < im->width ; j++, bp++, yp0++, yp1++ ){


			int r = yp1->r - yp0->r;
			int g = yp1->g - yp0->g;
			int b = yp1->b - yp0->b;

			dc = r;
			if( ABS(dc) < ABS(g) )
				dc = g;

			*tp++ = PUSH_TO_RANGE(dc, -127, 128 );
		}
	}

	return( im );
}




image_type *
bImage_grad_c( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 1, IMAGE_TYPE_U8, 1 );

	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	s_char *tp = (s_char *)im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){


		int	dc;
		bImage_type *yp0 = ( i == 0 )? bp : bp-bim->width;
		bImage_type *yp1 = ( i < im->height-1)? bp+bim->width : bp;
		for( j = 0 ; j < im->width ; j++, bp++, yp0++, yp1++ ){


			bImage_type *bp0 = ( j == 0 )? bp : bp-1;
			bImage_type *bp1 = ( j < im->width-1)? bp+1 : bp;

			// dx
			int r = bp1->r - bp0->r;
			int g = bp1->g - bp0->g;
			int b = bp1->b - bp0->b;

			dc = r;
			if( ABS(dc) < ABS(g) )
				dc = g;


			// dy
			r = yp1->r - yp0->r;
			g = yp1->g - yp0->g;
			b = yp1->b - yp0->b;



			if( ABS(dc) < ABS(r) )
				dc = r;
			if( ABS(dc) < ABS(g) )
				dc = g;

			//*tp++ = dc;
			*tp++ = PUSH_TO_RANGE(dc, -127, 128 );
		}
	}

	return( im );
}


image_type *
bImage_grad_x( image_type *bim, image_type *im )
{
	int	i,	j;


	im = image_realloc( im, bim->width-2, bim->height-2, 1, IMAGE_TYPE_U8, 1 );


	int	y[3];
	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, 1, 1 );
	s_char *tp = (s_char *)im->data;
	for( i = 0 ; i < im->height ; i++, bp += 2 ){

		bImage_type *bp0 = ( i == 0 )? bp : bp-1;
		bImage_type *bp1 = ( i < im->width-1)? bp+1 : bp;

		y[0] = IMAGE_RGB2Y( bp0->r, bp0->g, bp0->b );
		bp++;
		y[1] = IMAGE_RGB2Y( bp1->r, bp1->g, bp1->b );
		*tp++ = y[1] - y[0];
		bp++;

		for( j = 1 ; j < im->width-1 ; j++, bp++ ){

			y[2] = IMAGE_RGB2Y( bp->r, bp->g, bp->b );

			*tp++ = y[2] - y[0];

			y[0] = y[1];
			y[1] = y[2];
		}

		*tp++ = y[1] - y[0];
	}

	return( im );
}


void
bImage_set_sbA( image_type *bim, sbA_type *as )
{
	int	i;

	for( i = 0 ; i < as->nA ; i++ )
		bImage_set_sb( bim, i, &as->a[i] );

}


void
bImage_set_sb( image_type *bim, int i0, sb_type *s )
{
	int	j;
	int	r,	g,	b,	y,	n;

	if( s->j0 < 0 || s->j1 < 0 ){
		s->r = s->g = s->b = s->y = 0;
		return;
	}



	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, i0+1, s->j0+1 );

	r = g = b = y = n = 0;
	

	for( j = s->j0 ; j <= s->j1 ; j++, bp++ ){

		r += bp->r;
		g += bp->g;
		b += bp->b;


		y += IMAGE_RGB2Y( bp->r, bp->g, bp->b );

		n++;
	}

	if( n == 0 ){
		fprintf( stdout, "%d %d\n", s->j0, s->j1 );
		s->n = 0;
		return;
	}

	s->r = r / n;
	s->g = g / n;
	s->b = b / n;
	s->y = y / n;
	s->n = n;
}


static void	bImage_set_sb_sideL( image_type *bim, int i0, int d, sb_type *s );


void
bImage_set_sbA_sideL( image_type *bim, sbA_type *as, int d )
{
	int	i;

	for( i = 0 ; i < as->nA ; i++ )
		bImage_set_sb_sideL( bim, i, d, &as->a[i] );

}


static void
bImage_set_sb_sideL( image_type *bim, int i0, int d, sb_type *s )
{
	int	j;
	int		n;

	s->ys = 0;
	s->r = s->g = s->b = 0;
	if( s->j0 < 0 || s->j1 < 0 ){
		return;
	}


	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, i0+1, s->j0+1 );

	int	R,	G,	B;
	R = G = B = 0;

	for( j = s->j0, n = 0 ; j < s->j0 + d && j <= s->j1 ; j++, bp++ ){


		s->ys += IMAGE_RGB2Y( bp->r, bp->g, bp->b );

		R += bp->r;
		G += bp->g;
		B += bp->b;

		n++;
	}

	s->ys = s->ys / n;

	s->r = R / n;
	s->g = G / n;
	s->b = B / n;


	int j0 = s->j0 + d;
	if( j0 >= s->j1-d )	j0 = s->j1 - d;
	if( j0 < s->j0 )	j0 = s->j0;
	int	 y = 0;
	bp = (bImage_type *)IMAGE_PIXEL( bim, i0+1, j0+1 );

	for( j = j0, n = 0 ; j < j0 + d && j <= s->j1 ; j++, bp++ ){


		y += IMAGE_RGB2Y( bp->r, bp->g, bp->b );

		n++;
	}

	s->y = y / n;
}



static void	bImage_set_sb_sideR( image_type *bim, int i0, int d, sb_type *s );

void
bImage_set_sbA_sideR( image_type *bim, sbA_type *as, int d )
{
	int	i;

	for( i = 0 ; i < as->nA ; i++ )
		bImage_set_sb_sideR( bim, i, d, &as->a[i] );

}


static void
bImage_set_sb_sideR( image_type *bim, int i0, int d, sb_type *s )
{
	int	j;
	int		n;

	s->ys = s->r = s->g = s->b = 0;
	if( s->j0 < 0 || s->j1 < 0 ){
		return;
	}



	int j0 = s->j1 - d;
	if( j0 < s->j0 )	j0 = s->j0;

	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, i0+1, j0+1 );


	int	R,	G,	B;
	R = G = B = 0;
	for( j = j0, n = 0 ; j <= s->j1 ; j++, bp++ ){


		s->ys += IMAGE_RGB2Y( bp->r, bp->g, bp->b );

		R += bp->r;
		G += bp->g;
		B += bp->b;

		n++;
	}

	s->ys = s->ys / n;

	s->r = R / n;
	s->g = G / n;
	s->b = B / n;





	j0 = s->j1 - 2*d;
	if( j0 < s->j0 )	j0 = s->j0;


	bp = (bImage_type *)IMAGE_PIXEL( bim, i0+1, j0+1 );

	int	y = 0;

	for( j = j0, n = 0 ; j < j0+d && j <= s->j1 ; j++, bp++ ){


		y += IMAGE_RGB2Y( bp->r, bp->g, bp->b );

		n++;
	}

	s->y = y / n;



}





int
bImage_fill_gap( image_type *bim, int a[], int side )
{
	int	i,	i0,	n;

	i0 = -1;
	for( i = 1, n = 0 ; i < bim->height-1 ; i++ ){
		if( a[i] < 0 ){
			n++;
			continue;
		}

		if( n > 0 && i0 >= 0 )
			bImage_fill_gap( bim, a, i0, i, side );

		i0 = i;
		n = 0;
	}

	if( n > 0 && i0 >=0 )
		bImage_fill_gap( bim, a, i0, -1, side );

	return( 1 );
}


#ifdef _AA_
int
bImage_fill_gapR( image_type *bim, int a[] )
{
	int	i;

	int	h = bim->height;


	for( i = h-1; i > h-24 ; i-- ){
		if( a[i] < 0 )	continue;

		if( a[i] > a[i-1] && a[i-1] > a[i-2] && a[i] > bim->width - 10 )
			break;

		a[i] = -1;
	}

	return( 1 );
}


int
	bImage_fill_gapL( image_type *bim, int a[] )
{
	int	i;

	int	h = bim->height;


	for( i = h-1; i > h-24 ; i-- ){
		if( a[i] < 0 )	continue;

		if( a[i] < a[i-1] && a[i-1] < a[i-2] && a[i] < 10 )
			break;

		a[i] = -1;
	}

	return( 1 );
}
#endif

int
bImage_fill_gap_up( image_type *bim, int a0[], int a1[] )
{
	int	i;

	for( i = 0 ; i <  bim->height ; i++ ){
		if( a0[i] >= 0 )	break;
	}

	int i0 = i;


	for( i = 0 ; i <  bim->height ; i++ ){
		if( a1[i] >= 0 )	break;
	}


	int i1 = i;

	if( a1[i1] - a0[i0] < 10 || MIN(i0, i1 ) < 6 || MAX(i0,i1) > 24 )	
		return( 1 );



	for( i = 0 ; i < 4 ; i++ ){
		a0[i0-i] = a0[i0];
		a1[i1-i] = a1[i1];
	}

	return( 1 );
}



int
bImage_fill_gap( image_type *bim, int a[], int i0, int i1, int side )
{
	int	i,	j,	k;


	if( i1 > i0 && i1 < bim->height-1 ){


		bImage_type *bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i0, a[i0] );
		bImage_type *bp1 = ( bImage_type *)IMAGE_PIXEL( bim, i1, a[i1] );



		for( k = i0+1 ; k < i1 ; k++ ){
			float f = ( k - i0)/(float)(i1 - i0);
			int r = ( bp1->r - bp0->r)*f + bp0->r;
			int g = ( bp1->g - bp0->g)*f + bp0->g;
			int b = ( bp1->b - bp0->b)*f + bp0->b;


			int k0 = ( a[i1] - a[i0])*f + a[i0];
			if( side == F_LEFT ){
				if( k0 < a[k] || a[k] < 0 )	a[k] = k0;
			}
			else if( k0 > a[k] || a[k] < 0 )	a[k] = k0;


			int	j0,	j1;
			if( (j0 = a[k]-2) < 1 )	j0 = 1;
			if( (j1 = a[k]+2) > bim->width-2 )	j1 = bim->width-2;

			bImage_type *bp = ( bImage_type *)IMAGE_PIXEL( bim, k, j0);
			for( j = j0; j <= j1 ; j++, bp++ ){

				bp->r = r;
				bp->g = g;
				bp->b = b;
			}
		}

		return( 1 );
	}



	

	bImage_type *bp0,	*bp;
	bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i0, a[i0] );

	for( i = i0+1 ; i < bim->height-1 ; i++ ){

		int k0 =  a[i0];
		if( side == F_LEFT ){
			if( k0 < a[i] )	a[i] = k0;
		}
		else if( k0 > a[i] )	a[i] = k0;

		if( a[i] < 0 )	a[i] = k0;


		int	j0,	j1;
		if( (j0 = a[i]-10) < 1 )	j0 = 1;
		if( (j1 = a[i]+10) > bim->width-2 )	j1 = bim->width-2;

		if( side == F_LEFT )
			j0 = 1;

		if( side == F_RIGHT )
			j1 = bim->width -2;

		bp = ( bImage_type *)IMAGE_PIXEL( bim, i, j0);
		for( j = j0; j <= j1 ; j++, bp++ ){

			bp->r = bp0->r;
			bp->g = bp0->g;
			bp->b = bp0->b;
			bp->n = 1;
		}
	}


	return( 1 );
}