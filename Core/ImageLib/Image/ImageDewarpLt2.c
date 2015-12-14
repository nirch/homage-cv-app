/****************************
 ***   ImageDewarpLt2.c   ***
 ****************************/
#include	<math.h>

#include "Umath/LT2Type.h"

#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"



static image_type *	image1_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im );

static image_type *	image3_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im );

image_type *	image4_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im );




// tim(x,y) = sim( lt(x,y) )

image_type *
image_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im )
{

	if( sim->depth == 1 ){
		im = image1_dewarp_lt2( sim, lt, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_dewarp_lt2( sim, lt, im );
		return( im );
	}

	if( sim->depth == 4 ){
		im = image4_dewarp_lt2( sim, lt, im );
		return( im );
	}

	return( NULL );
}




// tim(x,y) = sim( lt(x,y) )

static image_type *
image1_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im )
{
int	i,	j;
u_char	*sp,	*sp1;
u_char	*tp;
float	y,	x,	dx,	dy;
float	s1,	s2;
int	s;
int	iy,	ix;
float	x0,	y0;



	if( im == NULL )
		im = image_recreate( im, sim->height, sim->width, 1, 1 );



	for( i = 0 ; i < im->row ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );
		y0 = i+0.5;
		for( j = 0 ; j < im->column ; j++ ){

			x0 = j+0.5;

			x = LT2_F1( *lt, x0, y0 );
			y = LT2_F2( *lt, x0, y0 );

			ix = x-0.5;
			iy = y-0.5;



			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				continue;
			}


			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			sp = IMAGE_PIXEL( sim, iy, ix );
			sp1 = sp + sim->column;

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			// Red
			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+1));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

		}
	}

	return( im );
}



// tim(x,y) = sim( lt(x,y) )
image_type *
image3_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im )
{
int	i,	j;
u_char	*sp,	*sp1;
u_char	*tp;
float	y,	x,	dx,	dy;
float	s1,	s2;
int	s;
int	iy,	ix;
float	x0,	y0;


	
	if( im == NULL )
		im = image_recreate( im, sim->height, sim->width, 3, 1 );



	for( i = 0 ; i < im->row ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );
		y0 = i+0.5;
		for( j = 0 ; j < im->column ; j++ ){

			x0 = j+0.5;

			x = LT2_F1( *lt, x0, y0 );
			y = LT2_F2( *lt, x0, y0 );

			ix = x-0.5;
			iy = y-0.5;



			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}

			
			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			sp = IMAGE_PIXEL( sim, iy, ix );
			sp1 = sp + 3*sim->column;

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			// Red
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			//Green
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			// Blue
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;
		}
	}

	return( im );
}



// tim(x,y) = sim( lt(x,y) )
image_type *
image4_dewarp_lt2( image_type *sim, lt2_type *lt, image_type *im )
{
	int	i,	j;
	u_char	*sp,	*sp1;
	u_char	*tp;
	float	y,	x,	dx,	dy;
	float	s1,	s2;
	int	s;
	int	iy,	ix;
	float	x0,	y0;



	if( im == NULL )
		im = image_realloc( im, sim->width, sim->height, sim->channel, IMAGE_TYPE_U8, 1 );



	for( i = 0 ; i < im->row ; i++ ){
		tp = IMAGE_PIXEL( im, i, 0 );
		y0 = i+0.5;
		for( j = 0 ; j < im->column ; j++ ){

			x0 = j+0.5;

			x = LT2_F1( *lt, x0, y0 );
			y = LT2_F2( *lt, x0, y0 );

			ix = x-0.5;
			iy = y-0.5;



			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}


			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			sp = IMAGE_PIXEL( sim, iy, ix );
			sp1 = sp + 4*sim->column;

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			// Red
			s1 = (1-dx)* (*sp) + dx * (*(sp+4));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+4));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			//Green
			s1 = (1-dx)* (*sp) + dx * (*(sp+4));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+4));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			// Blue
			s1 = (1-dx)* (*sp) + dx * (*(sp+4));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+4));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			// Alpah
			s1 = (1-dx)* (*sp) + dx * (*(sp+4));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+4));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;
		}
	}

	return( im );
}



