/*************************
 ***   Image1Level.c   ***
 *************************/

#include "ImageType/ImageType.h"
#include "ImageType/ImageTool.h"




image_type *
imageM_level( image_type *sim, image_type *mim, float hp, int nLevel, float level[], image_type *im )
{


	if( sim->format == IMAGE_FORMAT_F ){
		im = imageFM_level( sim, mim, 0.01, nLevel, level, im );
		return( im );
	}

	if( im->format == IMAGE_FORMAT_F ){
		im = image1M_level( sim, mim, 0.01, nLevel, level, im );
		return( im );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}

image_type *
image1_level( image_type *sim, box2i *box, float hp, int nRange, float range[], image_type *im )
{
	int	h[256];
	int	i,	j,	iR;
	float	r0,	r1,	r,	dr;
	u_char	*sp,	*tp;

	image1_histogram( sim, box, h, 1 );

	histogram_range( h, 256, hp, hp, &r0, &r1 );


	r = r1 - r0;
	dr = (r1 - r0 ) / nRange;
	for( i = 0, j = 0, r = r0 ; i < 256 ; ){
		iR = r + 0.5*dr + 0.5;
		range[j++] = iR;
		r = r + dr;

		if( r >= r1 - 0.1 )	r = 256;
		for( ; i < r ; i++ )
			h[i] = j;// iR;
	}


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	

	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){

			*tp = h[*sp];
		}

		return( im );
}

image_type *
image1M_level( image_type *sim, image_type *mim, float hp, int nRange, float range[], image_type *im )
{
int	h[256];
int	i,	j,	iR;
float	r0,	r1,	r,	dr;
u_char	*sp,	*mp,	*tp;

	image1M_histogram( sim, mim, h );

	histogram_range( h, 256, hp, hp, &r0, &r1 );

	
	r = r1 - r0;
	dr = (r1 - r0 ) / nRange;
	for( i = 0, j = 0, r = r0 ; i < 256 ; ){
		iR = r + 0.5*dr + 0.5;
		range[j++] = iR;
		r = r + dr;

		if( r >= r1 - 0.1 )	r = 256;
		for( ; i < r ; i++ )
			h[i] = j;// iR;
	}


	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data;
	mp = mim->data;

	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++, tp++ ){
			if( *mp == 0 ){
				*tp = 0;
				continue;
			}
			
			*tp = h[*sp];
		}

	return( im );
}



image_type *
imageFM_levelO( image_type *sim, image_type *mim, float hp, int nRange, float range[], image_type *im )
{
float	h[4096];

int	i,	j;
float	r0,	r1,	r,	dr;
u_char	*mp,	*tp;
float	*sp;
int	tmp;
float	iR;

	imageFM_minmax( sim, mim, &r0, &r1 );

	imageFM_minmaxP( sim, mim, 0.01, &r0, &r1 );

	if( r1 - r0 <= 0 )	return( NULL );


	r = r1 - r0;
	dr = (r1 - r0 ) / nRange;

	for( i = 0, j = 0, r = r0 ; i < 4096 ; ){
		range[j++] = r + 0.5*dr;
		r = r + dr;


		iR = 4095*(r -r0 )/( r1 - r0 );// + 0.5;
		if( iR >= 4095 )	iR = 4096;

		for( ; i < iR ; i++ )
			h[i] = j;// iR;
	}


	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data_f;
	mp = mim->data;

	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++, tp++ ){
			if( *mp == 0 ){
				*tp = 0;
				continue;
			}

			tmp = 4095*(*sp -r0 )/( r1 - r0 );// + 0.5;
			tmp = PUSH_TO_RANGE( tmp, 0, 4095 );

			*tp = h[tmp];
		}

		return( im );
}



image_type *
imageFM_level( image_type *sim, image_type *mim, float hp, int nRange, float range[], image_type *im )
{
int	i,	j,	k;
float	r0,	r1,	r,	dr;
u_char	*mp,	*tp;
float	*sp;
float	ar[256];

	imageFM_minmax( sim, mim, &r0, &r1 );

	//imageFM_minmaxP( sim, mim, 0.01, &r0, &r1 );

	if( r1 - r0 <= 0 )	return( NULL );


	r = r1 - r0;
	dr = (r1 - r0 ) / nRange;

	for( j = 0, r = r0 ; j < nRange ; j++, r += dr ){
		ar[j] = r;
		range[j] = r + 0.5*dr;
	}

	ar[j] = r1;



	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data_f;
	mp = mim->data;

	tp = im->data;
	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++, tp++ ){
			if( *mp == 0 ){
				*tp = 0;
				continue;
			}

			for( k = 1 ; k <  nRange ; k++ )
				if( *sp < ar[k] )	break;

			*tp = k;
		}

		return( im );
}





image_type *	imageM1_levelN( image_type *sim, image_type *mim, float aL[], int nL, image_type *im );

image_type *	imageMF_levelN( image_type *sim, image_type *mim, float aL[], int nL, image_type *im );



image_type *
imageM_levelN( image_type *sim, image_type *mim, float aL[], int nL, image_type *im )
{
	if( sim->depth == IMAGE_FORMAT_UC ){
		im = imageM1_levelN( sim, mim, aL, nL, im );
		return( im );
	}

	if( sim->format == IMAGE_FORMAT_F){

		im = imageMF_levelN( sim, mim, aL, nL, im );
		return( im );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}


image_type *
imageM1_levelN( image_type *sim, image_type *mim, float aL[], int nL, image_type *im )
{
	int	i,	j,	k;
	u_char	*tp,	*mp;
	u_char	*sp;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	mp = mim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++, tp++ ){

			if( *mp == 0 ){
				*tp = 0;
				continue;
			}
			for( k = 0 ; k < nL ; k++ )
				if( *sp < aL[k] )	break;

			*tp = k+1;
		}

		return( im );
}


image_type *
imageMF_levelN( image_type *sim, image_type *mim, float aL[], int nL, image_type *im )
{
	int	i,	j,	k;
	u_char	*tp,	*mp;
	float	*sp;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data_f;
	mp = mim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, mp++, tp++ ){

			if( *mp == 0 ){
				*tp = 0;
				continue;
			}

			for( k = 0 ; k  < nL ; k++ )
				if( *sp < aL[k] )	break;


			*tp = k+1;
		}

		return( im );
}


image_type *	image1_levelN( image_type *sim, float aL[], int nL, image_type *im );

image_type *	imageF_levelN( image_type *sim, float aL[], int nL, image_type *im );



image_type *
image_levelN( image_type *sim, float aL[], int nL, image_type *im )
{
	if( sim->depth == 1 ){
		im = image1_levelN( sim, aL, nL, im );
		return( im );
	}

	if( sim->depth == 4 ){
		im = imageF_levelN( sim, aL, nL, im );
		return( im );
	}

	if( im != NULL )
		image_destroy( im, 1 );

	return( NULL );
}


image_type *
image1_levelN( image_type *sim, float aL[], int nL, image_type *im )
{
	int	i,	j,	k;
	u_char	*tp;
	u_char	*sp;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){

			for( k = 0 ; k < nL ; k++ )
				if( *sp < aL[k] )	break;

			*tp = k+1;
		}

		return( im );
}


image_type *
imageF_levelN( image_type *sim, float aL[], int nL, image_type *im )
{
	int	i,	j,	k;
	u_char	*tp;
	float	*sp;


	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data_f;
	tp = im->data;

	for( i = 0 ; i < im->row ; i++ )
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){

			for( k = 0 ; k  < nL ; k++ )
				if( *sp < aL[k] )	break;

			*tp = k+1;
		}

		return( im );
}





