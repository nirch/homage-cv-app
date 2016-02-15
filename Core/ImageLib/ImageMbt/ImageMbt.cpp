
#include	<string.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Ulog/Log.h"
#include "Ubox/Box2d.h"
#include "Uln/PlnType.h"



#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"


#include "ImageMbt.h"



image_type *
image_mbt_create( int width, int height, int dxy )
{
int	i,	j;



	int h = height/dxy + 1;
	int w = width/dxy + 1;
	image_type *im = image_create( h, w, sizeof(mbt_type), 1, NULL );

	int	x,	y;
	for( i = 0, y = 0 ; i < im->height ; i += 1, y+= dxy ){
		mbt_type *m = (mbt_type *)IMAGE_PIXEL( im, i, 0 );
		for( j = 0, x = 0 ; j < im->width ; j += 1, x += dxy, m++ ){
			m->p.x = x;
			m->p.y = y;

			m->tp.x = x;
			m->tp.y = y;
		}
	}

	return( im );
}



void	image_mbt_1( image_type *sim, mbt_type *m0, mbt_type *m1, mbt_type *m2, image_type *im );

static vec2f_type Dp;

image_type *
image_mbt( image_type *sim, image_type *mbt, int d, vec2f_type *dp, image_type *im )
{
int	i,	j;

	if( dp == NULL ){
		Dp.x = Dp.y = 0;
	}
	else	Dp = *dp;

	im = image_reallocL( sim, im );
	image4_const( im, 0xFF000000 );
		image4_const( im, 0 );


	mbt_type *m = (mbt_type *)IMAGE_PIXEL( mbt, 16, 16 );
	mbt_type *m1 = m+mbt->width;



	for( i = 0 ; i < mbt->height-1 ; i++ ){
		mbt_type *m = (mbt_type *)IMAGE_PIXEL( mbt, i, 0 );
		mbt_type *m1 = m+mbt->width;
		for( j = 0 ; j < mbt->width-1 ; j++, m++, m1++ ){
	
			if( m->transparency == 0 )	continue;
				
			image_mbt_1( sim, m, m+1, m1, im );

			image_mbt_1( sim, m+1, m1+1, m1, im );
		}
	}


	return( im );
}


#define BOX2D_CROP_IMAGE( b, im )	\
	{ \
		if( b.x0 < 0 )	b.x0 = 0; \
		if( b.x1 > im->width )	b.x1 = im->width; \
		if( b.y0 < 0 )	b.y0 = 0; \
		if( b.y1 > im->height )	b.y1 = im->height; \
	}


static void	mbt_lt2( mbt_type *m0, mbt_type *m1, mbt_type *m2, lt2_type *lt );

void
image_mbt_1( image_type *sim, mbt_type *m0, mbt_type *m1, mbt_type *m2, image_type *im )
{
vec2f_type	u0,	u1,	u2;
	u0.y =   m1->tp.x - m0->tp.x;
	u0.x = -(m1->tp.y - m0->tp.y);
	float d0 = VEC2D_INNER( u0, m0->tp );

	u1.y =   m2->tp.x - m1->tp.x;
	u1.x = -(m2->tp.y - m1->tp.y);
	float d1 = VEC2D_INNER( u1, m1->tp );

	u2.y =   m0->tp.x - m2->tp.x;
	u2.x = -(m0->tp.y - m2->tp.y);
	float d2 = VEC2D_INNER( u2, m2->tp );

	box2i_type b;
	BOX2D_INIT( b, m0->tp.x, m0->tp.y );
	BOX2D_UPDATE( b, m1->tp.x, m1->tp.y );
	BOX2D_UPDATE( b, m2->tp.x, m2->tp.y );
	b.x1++;
	b.y1++;
	if( b.x0 < 0 )	b.x0 = 0;
	if( b.y0 < 0 )	b.y0 = 0;
	if( b.x1 > im->width )	b.x1 = im->width;
	if( b.y1 > im->height )	b.y1 = im->height;


	lt2_type	lt;
	mbt_lt2( m0, m1, m2, &lt );

#ifdef _AA_
	vec2f_type	q0,	q1,	q2;
	LT2_F( lt, m0->tp, q0 );
	LT2_F( lt, m1->tp, q1 );
	LT2_F( lt, m2->tp, q2 );

	q0.x -= m0->p.x;
	q0.y -= m0->p.y;

	q1.x -= m1->p.x;
	q1.y -= m1->p.y;

	q2.x -= m2->p.x;
	q2.y -= m2->p.y;
#endif

	lt.c0 += Dp.y;
	lt.c1 += Dp.x;

	int	i,	j;
	for( i = b.y0 ; i < b.y1 ; i++ ){
		float y0 = i+0.5;

		u_char	*tp = IMAGE_PIXEL( im, i, b.x0 );
		for( j = b.x0 ; j < b.x1 ; j++ ){
			float x0 = j+0.5;

			if( x0*u0.x + y0*u0.y - d0 < 0 || 
					x0*u1.x + y0*u1.y - d1 < 0 ||
					x0*u2.x + y0*u2.y - d2 < 0 ){
						tp += 4;
						continue;
			}




			float x = LT2_F1( lt, x0, y0 );
			float y = LT2_F2( lt, x0, y0 );

			int ix = x-0.5;
			int iy = y-0.5;



			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}

			

			float dx = x - (ix+0.5);
			float dy = y - (iy+0.5);

			u_char *sp = IMAGE_PIXEL( sim, iy, ix );
			u_char *sp1 = sp + 4*sim->column;



			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			int	s1,	s2,	s;
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
}


#include "Umath/Matrix3Type.h"
static void
mbt_lt2( mbt_type *m0, mbt_type *m1, mbt_type *m2, lt2_type *lt )
{
	matrix3_type	m;
	vec3d_type	D,	X;

	m.a00 = m0->tp.x;
	m.a01 = m0->tp.y;
	m.a02 = 1;
	D.x = m0->p.x;

	m.a10 = m1->tp.x;
	m.a11 = m1->tp.y;
	m.a12 = 1;
	D.y = m1->p.x;

	m.a20 = m2->tp.x;
	m.a21 = m2->tp.y;
	m.a22 = 1;
	D.z = m2->p.x;


	matrix3_solve( &m, &D, &X );
	lt->a0 = X.x;
	lt->b0 = X.y;
	lt->c0 = X.z;



	D.x = m0->p.y;
	D.y = m1->p.y;
	D.z = m2->p.y;


	matrix3_solve( &m, &D, &X );
	lt->a1 = X.x;
	lt->b1 = X.y;
	lt->c1 = X.z;

}




void
image_mbt_set( image_type *mbt, pt2dA_type *capt, int transparency )
{
	int	i,	j,		k;



	mbt_type *m = (mbt_type *)mbt->data;
	for( i = 0, k = 0 ; i < mbt->height ; i++ ){
		for( j = 0 ; j < mbt->width ; j++, m++, k++ ){
			pt2d_type *pt = &capt->a[k];

			m->tp.y = pt->p.x;
			m->tp.x = pt->p.y;



			if( transparency == 1 )
				m->transparency = pt->group;

		}
	}

}



void
image_mbt_transparency( image_type *mbt, int d, int dx, int dy, image_type *sim )
{
	int	i,	j,	k,	n,	no;


	int	align = sim->width - d;

	for( i = 0 ; i < mbt->height-1 ; i++ ){
		mbt_type *m = (mbt_type *)IMAGE_PIXEL( mbt, i, 0 );
		for( j = 0 ; j < mbt->width-1 ; j++, m++ ){

			if( j*d -dx < 0 || j*d - dx +d > sim->width ||  i*d - dy < 0 || i*d-dy + d > sim->height ){
				m->transparency = 0;
				continue;
			}

			u_int *sp = (u_int * )IMAGE_PIXEL( sim, i*d - dy, j*d - dx );
			for( k = 0, no = 0 ; k < d ; k++, sp += align ){
				for( n = 0 ; n < d ; n++, sp++ ){
					//if( (*sp & 0xFF000000) != 0xFF000000 )
					if( (*sp & 0xFF000000) != 0 )
						no++;
				}
			}


			if( no == 0 ){
				m->transparency = 0;
				continue;
			}

			if( no == d*d )
				m->transparency = 2;
			else m->transparency = 1; 

		}
	}


}