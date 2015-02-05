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


#include "pEdgeType.h"



#ifdef _DEBUG
#ifndef WINCE
#define _DUMP 
#endif
#endif

image_type *	image_ar_edge_1( image_type *eim, image_type *lim, int T, image_type *im );

image_type *	image_ar_edge_11( image_type *eim, image_type *lim, int D, int T, image_type *im );



static void	pEdge_centering( image_type *dim, image_type *gim, image_type *eim, float T );

static int	pEdge_centering_1( image_type *eim, image_type *mim, float T, vec2d *P, vec2d *V );


static void	pEdge_centering_ridge( image_type *dim,image_type *eim );


static void	pEdge_filter( image_type *dim,int d );

static int	pEdge_filter_1( image_type *dim, pEdge_type *dp0 );





image_type *
pEdge_detector( image_type *sim, pEdgePrm_type *prm, image_type *im )
{
image_type *gim,	*eim;

	IMAGE_DUMP( sim, "e", 1, "src" );

	gim = image1_gridient_sobol( sim, NULL );




	eim = image1_gridient_to_value( gim, NULL );
	IMAGE_DUMP( eim, "e", 0, "VALUE" );



	image_type *lim = image1_local_max( eim, NULL );
	IMAGE_DUMP( lim, "e", 0, "LM" );


//	im = image_ar_edge_1( gim, lim, T,  im );
	im = image_ar_edge_11( gim, lim, prm->d, prm->gradT,  im );


	PEDGE_DUMP( im, "e", 0, NULL );



	pEdge_centering( im, gim, eim, prm->gradT );
	PEDGE_DUMP( im, "e", 1, "centring" );



	pEdge_filter( im, 2 );

	PEDGE_DUMP( im, "e", 2, "filter" );

//	pEdge_centering( im, gim, eim, prm->gradT );
	pEdge_centering_ridge( im, eim );



	PEDGE_DUMP( im, "e", 1, "filter" );


	image_destroy( eim, 1 );
	image_destroy( gim, 1 );
	image_destroy( lim, 1 );


	return( im ); 
}





image_type *
image_ar_edge_1( image_type *eim, image_type *lim, int T, image_type *im )
{
u_char	*sp,	*sp0;
pEdge_type	*tp;
int	i,	j;
int	k,	n,	k0,	n0,	align;
int	sum;
int	no;


	if( im == NULL ){
		im = image_recreate( im, lim->height/4, lim->width/4, sizeof(pEdge_type), 1 );
		tp = (pEdge_type *)im->data;
		for( i = 0 ; i < im->height ; i++ )
			for( j = 0; j < im->width ; j++, tp++ )
				tp->state = 0;
	}


	int	i1;
	align = lim->width - 4;
	no = 0;
	tp = (pEdge_type *)im->data;
	for( i = 8, i1 = 2 ; i+4+8 < lim->row ; i += 4, i1++ ){
		sp0 = (u_char *)IMAGE_PIXEL( lim, i, 8 );
		tp = (pEdge_type *)IMAGE_PIXEL( im, i1, 2 );
		for( j = 8 ; j+4+8 < lim->column ; j += 4, sp0 += 4, tp++ ){



			sum = 0;
			for( k = 0, sp = sp0 ; k < 4 ; k++, sp += align ){
				for( n = 0 ; n < 4 ; n++, sp++ ){
					if( sum < *sp ){
						sum = *sp;
						k0 = k;
						n0 = n;
					}
				}
			}


			if( sum < T ){
				tp->state = 0;
				continue;
			}


			s_char *ep = (s_char *)IMAGE_PIXEL( eim, i+k0, j+n0 );

			tp->v.x = *ep;
			tp->v.y = *(ep+1);

#ifdef WINCE
			float t = hypotA( tp->v.x, tp->v.y );
#else
			float t = hypotA( tp->v.x, tp->v.y );
#endif

			tp->v.x /= t;
			tp->v.y /= t;


			t = (n0-2) * tp->v.x + (k0-2)*tp->v.y; 

			tp->p.x = j + 2 + 0.5 + t*tp->v.x;
			tp->p.y = i + 2 + 0.5 + t*tp->v.y;
			
			tp->state = 1;
			tp->g = sum;
		}
	}


	return( im );
}

image_type *
image_ar_edge_11( image_type *eim, image_type *lim, int D, int T, image_type *im )
{
	u_char	*sp,	*sp0;
	pEdge_type	*tp;
	int	i,	j;
	int	k,	n,	k0,	n0,	align;
	int	sum;
	int	no;



//	if( im == NULL ){
		im = image_recreate( im, lim->height/D, lim->width/D, sizeof(pEdge_type), 1 );
		tp = (pEdge_type *)im->data;
		for( i = 0 ; i < im->height ; i++ )
			for( j = 0; j < im->width ; j++, tp++ )
				tp->state = 0;
//	}


	int	i1;
	align = lim->width - D;
	no = 0;

	int	D2 = 2*D;
	float	d = 0.5*D;
	tp = (pEdge_type *)im->data;
	for( i = D2, i1 = 2 ; i+D+D2 < lim->row ; i += D, i1++ ){
		sp0 = (u_char *)IMAGE_PIXEL( lim, i, D2 );
		tp = (pEdge_type *)IMAGE_PIXEL( im, i1, 2 );
		for( j = D2 ; j+D+D2 < lim->column ; j += D, sp0 += D, tp++ ){



			sum = 0;
			for( k = 0, sp = sp0 ; k < D ; k++, sp += align ){
				for( n = 0 ; n < D ; n++, sp++ ){
					if( sum < *sp ){
						sum = *sp;
						k0 = k;
						n0 = n;
					}
				}
			}


			if( sum < T ){
				tp->state = 0;
				continue;
			}


			s_char *ep = (s_char *)IMAGE_PIXEL( eim, i+k0, j+n0 );

			tp->v.x = *ep;
			tp->v.y = *(ep+1);

#ifdef WINCE
			float t = hypotA( tp->v.x, tp->v.y );
#else
			float t = hypotA( tp->v.x, tp->v.y );
#endif

			tp->v.x /= t;
			tp->v.y /= t;


			t = (n0+0.5-d) * tp->v.x + (k0+0.5-d)*tp->v.y; 

			tp->p.x = j + d + t*tp->v.x;
			tp->p.y = i + d + t*tp->v.y;

			tp->state = 1;
			tp->g = sum;
		}
	}


	return( im );
}


static void
pEdge_centering( image_type *dim, image_type *gim, image_type *eim, float T )
{
pEdge_type	*dp;
int	i,	j;

	dp = (pEdge_type *)dim->data;
	for( i = 0 ; i < dim->height ; i++ ){
		for( j = 0 ; j < dim->width ; j++, dp++ ){
			if( dp->state == 0 )	continue;

			if( pEdge_centering_1( gim, eim, T, &dp->p, &dp->v ) < 0 )
				dp->state = 0;

		}
	}

}



static int
pEdge_centering_1( image_type *eim, image_type *mim, float T, vec2d *P, vec2d *V )
{
int	i,	j,	no;
s_char	*sp;
u_char	*mp;
vec2d	v;
float	t,	u,	w;

matrix2_type	m;
vec2d_type	D,	X;
float	a,	b;
int	i0,	j0;


	i0 = P->y;
	j0 = P->x;

	if( i0-4 < 0 || i0+4 >= eim->height )
		return( -1 );

	if( j0-4 < 0 || j0+4 >= eim->width )
		return( -1 );

	v = *V;

	m.a00 = m.a01 = m.a10 = m.a11 = 0;
	D.x = D.y = 0;

	vec2f_type	dv;
	for( i = -4, no = 0 ; i <= 4 ; i++ ){
		sp = (s_char *)IMAGE_PIXEL( eim, i0+i, j0-4 );
		mp = IMAGE_PIXEL( mim, i0+i, j0-4 );

		dv.y = ( i+i0+ 0.5) - P->y;
		for( j = -4 ; j <= 4 ; j++, sp += 2, mp++ ){
			if( *mp < T )	continue;

			dv.x = ( j+j0+ 0.5) - P->x;

			t = dv.x * v.y - dv.y * v.x;
			u = dv.x * v.x + dv.y * v.y;
			if( ABS(u) > 2 )	continue;
			if( ABS(t) > 4 )	continue;

			w = *sp * v.x + *(sp+1)*v.y;

			if( w*w  < (sp[0]*sp[0] + sp[1]*sp[1])*0.65*0.65  )
				continue;

			if( w < 8 )	continue;

//			w = 1.0 / w;
			w = 256 - w;


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
	if( ABS(b) > 2 )
		return( -1 );

	if( a < -0.2 || a > 0.2 )
		return( -1 );


	vec2f_type	n,	p;
	t = hypotA( -a, 1 );
	n.x = -a / t;
	n.y = 1/ t;

	t = b/t;
	p.x = t* n.x;
	p.y = t* n.y;

	P->x += p.x * v.y + p.y*v.x;
	P->y += p.x * -v.x + p.y*v.y;

	V->x = n.x * v.y + n.y*v.x;
	V->y = n.x * -v.x + n.y*v.y;

	return( 1 );
}




static void
pEdge_filter( image_type *dim,int d )
{
	pEdge_type	*dp;
	int	i,	j;

	dp = (pEdge_type *)dim->data;
	for( i = d ; i < dim->height-d ; i++ ){
		dp = (pEdge_type *)IMAGE_PIXEL( dim, i, d );
		for( j = d ; j < dim->width-d ; j++, dp++ ){
			if( dp->state != 1 )	continue;

			pEdge_filter_1( dim, dp );
		}
	}

}




static int
pEdge_filter_1( image_type *dim, pEdge_type *dp0 )
{
pEdge_type	*dp;

float	t;


	dp = dp0 + 1;
	if( dp->state == 1 ){
		if( dp->v.x * dp0->v.x + dp->v.y * dp0->v.y > 0.900 ){

			t = (dp->p.x - dp0->p.x) * dp0->v.y + (dp->p.y - dp0->p.y) * -dp0->v.x;

			if( ABS(t) < 1.25 ){
				if( dp0->g < dp->g )
					dp0->state = 0;
				else	dp->state = 0;

				return( 1 );
			}
		}
	}


	dp = dp0 + dim->width;
	if( dp->state == 1 ){
		if( dp->v.x * dp0->v.x + dp->v.y * dp0->v.y > 0.900 ){

			t = (dp->p.x - dp0->p.x) * dp0->v.y + (dp->p.y - dp0->p.y) * -dp0->v.x;

			if( ABS(t) < 1.25 ){
				if( dp0->g < dp->g )
					dp0->state = 0;
				else	dp->state = 0;

				return( 1 );
			}
		}
	}

	return( 0 );

}




#include "ImageApp/ImageApp.h"
#include "EdgeLib/Ridge/PRidgeType.h"


static void
pEdge_centering_ridge( image_type *dim,image_type *eim )
{
	pEdge_type	*dp;
	int	i,	j;

	pRidge_type	rp;


	pRidgeDetectPrm_type *prm = pRidgeDetectPrm_alloc();
	prm->sign = 1;
	jet2App_type *ja = jet2App_init( prm->size, prm->weight );



	dp = (pEdge_type *)dim->data;
	for( i = 0 ; i < dim->height ; i++ ){
		for( j = 0 ; j < dim->width ; j++, dp++ ){
			if( dp->state == 0 )	continue;

			int j0 = dp->p.x;
			int i0 = dp->p.y;

			if( pRidge_detector( eim, i0, j0, &rp, ja, prm ) < 0 )
				continue;
			
			dp->p = rp.p;
			if( VEC2D_INNER( dp->v, rp.v) < 0 ){
				dp->v.x = -rp.v.x;
				dp->v.y = -rp.v.y;
			}
			else dp->v = rp.v;
		}
	}

	pRidgeDetectPrm_destroy( prm );
	jet2App_destroy( ja );
}