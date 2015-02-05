/***************************
***   ArDetectGrad.cpp   ***
****************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include "Uvec/Vec3d.h"
#include "Umath/Matrix2Type.h"

#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageMark/ImageMark.h"
#include "ImageDump/ImageDump.h"






#ifdef _DEBUG
#define _DUMP 
#endif




static int	pEdge_centering_1( image_type *eim, image_type *mim, float T, vec2d *P, vec2d *V );




//void
//ridge_centering( image_type *dim, image_type *gim, image_type *eim, float T )
//{
//pEdge_type	*dp;
//int	i,	j;
//
//	dp = (pEdge_type *)dim->data;
//	for( i = 0 ; i < dim->height ; i++ ){
//		for( j = 0 ; j < dim->width ; j++, dp++ ){
//			if( dp->state == 0 )	continue;
//
//			if( ridge_centering_1( gim, eim, T, &dp->p, &dp->v ) < 0 )
//				dp->state = 0;
//
//		}
//	}
//
//}



int
ridge_centering_1( image_type *eim, float uT, float vT, vec2d *P, vec2d *V )
{
int	i,	j,	no;
u_char	*sp;
vec2d	v;
float	t,	u,	w;

matrix2_type	m;
vec2d_type	D,	X;
float	a,	b;
box2i_type	box;



	box.x0 = P->x - vT;
	box.x1 = P->x + vT;

	box.y0 = P->y - vT;
	box.y1 = P->y + vT;

	if( box.x0 < 0 )	box.x0 = 0;
	if( box.x1 > eim->width )	box.x1 = eim->width;

	if( box.y0 < 0 )	box.y0 = 0;
	if( box.y1 > eim->height )	box.y1 = eim->height;



//	v = *V;
	v.x = V->y;
	v.y = -V->x;

	m.a00 = m.a01 = m.a10 = m.a11 = 0;
	D.x = D.y = 0;

	vec2f_type	dv;
	for( i = box.y0, no = 0 ; i < box.y1 ; i++ ){
		sp = (u_char *)IMAGE_PIXEL( eim, i, box.x0 );
		

		dv.y = ( i+ 0.5) - P->y;
		for( j = box.x0 ; j < box.x1 ; j++, sp++){
			

			dv.x = ( j+ 0.5) - P->x;

			t = dv.x * v.y - dv.y * v.x;
			u = dv.x * v.x + dv.y * v.y;
			//u = dv.x * v.y - dv.y * v.x;
			//t = dv.x * v.x + dv.y * v.y;


			if( ABS(u) > uT )	continue;
			if( ABS(t) > vT )	continue;

	
			w = *sp / 50.0;


			m.a00 += w*t*t;
			m.a01 += w*t;
			m.a10 += w*t;
			m.a11 += w;

			D.x += w*t*u;
			D.y += w*u;

			no++;
		}
	}

	if( no < 5 )	return( -1 );



	matrix2S_solve( &m, &D, &X );
	a = X.x;
	b = X.y;
	if( ABS(b) > uT )
		return( -1 );

	if( a < -0.2 || a > 0.2 )
		return( -1 );


	vec2f_type	n,	p;
	t = hypot( -a, 1 );
	n.x = -a / t;
	n.y = 1/ t;

	t = b/t;
	p.x = t* n.x;
	p.y = t* n.y;

	P->x -= p.x * v.y + p.y*v.x;
	P->y -= p.x * -v.x + p.y*v.y;

	vec2f_type v1;
	v1.x = n.x * v.y + n.y*v.x;
	v1.y = n.x * -v.x + n.y*v.y;


	V->x = -v1.y;
	V->y = v1.x;


	return( 1 );
}
float
ridge_centering_2( image_type *eim, float uT, float vT, vec2d *P, vec2d *V, float *val );


float
ridge_centering_3( image_type *eim, float uT, float vT, float d, int color, vec2f_type *P, vec2f_type *V, float *val, float *dx )
{
vec2f_type	v,	p,	p0;
float	d0;
int	i;
float	tmp,	tMax;
	v.x = V->y;
	v.y = -V->x;

	tMax = 0;
	float	f,	f0;

	for( i = -d ; i < d ; i++ ){
		p.x = P->x + i*v.x;
		p.y = P->y+ i*v.y;

		tmp = ridge_centering_2( eim, uT, vT, &p, V, &f );

		if( color == 2 && tmp > tMax || color == 1 &&  -tmp > -tMax ){
			tMax = tmp;
			p0 = p;
			f0 = f;
			d0 = i;
		}
	}

	if( color == 2 && tMax > 0 || color == 1 && tMax < 0 ){
		*P = p0;
		*val = f0;
		*dx = d0;
	}

	return( tMax  );
}


float
ridge_centering_2( image_type *eim, float uT, float vT, vec2d *P, vec2d *V, float *val )
{
	int	i,	j,	no;
	u_char	*sp;
	vec2d	v;
	float	t,	u;

	matrix2_type	m;
	vec2d_type	D;
	box2i_type	box;



	box.x0 = P->x - vT;
	box.x1 = P->x + vT;

	box.y0 = P->y - vT;
	box.y1 = P->y + vT;

	if( box.x0 < 0 )	box.x0 = 0;
	if( box.x1 > eim->width )	box.x1 = eim->width;

	if( box.y0 < 0 )	box.y0 = 0;
	if( box.y1 > eim->height )	box.y1 = eim->height;



	//	v = *V;
	v.x = V->y;
	v.y = -V->x;

	m.a00 = m.a01 = m.a10 = m.a11 = 0;
	D.x = D.y = 0;

	int	f = 0,	nF = 0;
	double sum = 0,	sum2 = 0,	su = 0;
	vec2f_type	dv;
	for( i = box.y0, no = 0 ; i < box.y1 ; i++ ){
		sp = (u_char *)IMAGE_PIXEL( eim, i, box.x0 );


		dv.y = ( i+ 0.5) - P->y;
		for( j = box.x0 ; j < box.x1 ; j++, sp++){


			dv.x = ( j+ 0.5) - P->x;

			t = dv.x * v.y - dv.y * v.x;
			u = dv.x * v.x + dv.y * v.y;


			if( ABS(u) > uT )	continue;
			if( ABS(t) > vT )	continue;

			u /= uT;

			sum += u*u * *sp;

			sum2 += *sp;

			su += u*u;

			no++;

			if( ABS(u) < 0.5*uT ){
				f += *sp;
				nF++;

			}

		}
	}

	sum = sum - su/no * sum2;

	sum /= no;

	*val = (float)f / nF;




	return( sum );
}


