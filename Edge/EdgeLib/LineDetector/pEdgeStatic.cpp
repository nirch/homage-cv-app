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



static int	pEdge_compare( pEdge_type *sp, pEdge_type *tp );



image_type *
pEdge_static( image_type *sim, image_type *im )
{
pEdge_type *sp,	*tp;
vec2f_type dv;
int	i,	j;
float	t;

	if( sim == NULL || sim->width != im->width || sim->height != im->height ){
		sim = image_recreate( sim, im->height, im->width, sizeof(pEdge_type), 1 );
		tp = (pEdge_type *)im->data;
		sp = (pEdge_type *)sim->data;
		for( i = 0 ; i < im->height ; i++ ){
			for( j = 0; j < im->width ; j++, tp++, sp++ ){
					tp->nStatic = 0;
					*sp = *tp;
			}
		}

		return( sim );
	}



	sp = (pEdge_type *)sim->data;
	tp = (pEdge_type *)im->data;

	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0; j < im->width ; j++, sp++, tp++ ){

			if( tp->state == 0 ){
				tp->nStatic = 0;

				if( sp->nStatic < 10 ){
					sp->nStatic = 0;
					sp->state = 0;
				}

				continue;
			}

			if( sp->state == 0 ){
				tp->nStatic = 0;
				*sp = *tp;
				sp->nStatic = 0;
				continue;
			}

			t = sp->v.x * tp->v.x + sp->v.y * tp->v.y;
			if( t < 0.95 ){
				tp->nStatic = 0;
				if( sp->nStatic < 10 )
					sp->nStatic = 0;
				continue;
			}

			dv.x =  tp->p.x - sp->p.x;
			dv.y =  tp->p.y - sp->p.y;

			t = dv.x * sp->v.y + dv.y * -sp->v.x;

			if( ABS(t) > 0.25 ){
				tp->nStatic = 0;
				if( sp->nStatic < 10 )
					sp->nStatic = 0;
				continue;
			}

			sp->nStatic ++;
			tp->nStatic = sp->nStatic;
		}
	}

	return( sim );
 
}




image_type *
pEdge_staticN( image_type *sim, image_type *im )
{
pEdge_type *sp,	*tp;
int	i,	j;


	if( sim == NULL || sim->width != im->width || sim->height != im->height ){
		sim = image_recreate( sim, im->height, im->width, 2*sizeof(pEdge_type), 1 );
		tp = (pEdge_type *)im->data;
		sp = (pEdge_type *)sim->data;
		for( i = 0 ; i < im->height ; i++ ){
			for( j = 0; j < im->width ; j++, sp += 2, tp++ ){
				tp->nStatic = 0;
				sp[0] = *tp;
				sp[1] = *tp;
			}
		}

		return( sim );
	}



	sp = (pEdge_type *)sim->data;
	tp = (pEdge_type *)im->data;

	for( i = 0 ; i < im->height ; i++ ){
		for( j = 0; j < im->width ; j++, sp += 2, tp++ ){

			if( tp->state == 0 ){

				sp[1] = *tp;
				sp[1].nStatic = 0;
				tp->nStatic = 0;

				continue;
			}



			if( sp[1].state == 0  || pEdge_compare( &sp[1], tp ) < 0 ){
				sp[1] = *tp;
				sp[1].nStatic = 0;

				tp->nStatic = 0;
			}
			else sp[1].nStatic++;



			if( pEdge_compare( &sp[0], &sp[1] ) < 0 ){
				if( sp[1].nStatic < sp[0].nStatic ){
					tp->nStatic = 0;
					continue;
				}

				sp[0] = sp[1];
			}
			else	sp[0].nStatic++;


			tp->nStatic = sp[0].nStatic;
		}
	}

	return( sim );

}


static int 
pEdge_compare( pEdge_type *sp, pEdge_type *tp )
{
vec2f_type	dv;
float	t;

	t = sp->v.x * tp->v.x + sp->v.y * tp->v.y;
	if( t < 0.95 )
		return( -1 );


	dv.x =  tp->p.x - sp->p.x;
	dv.y =  tp->p.y - sp->p.y;

	t = dv.x * sp->v.y + dv.y * -sp->v.x;

	if( ABS(t) > 0.25 )
		return( -1 );

	return( 1 );
}