
#include	<string.h>
#include	<math.h>

#ifdef _DEBUG
#define _DUMP 
#endif


#include	"Ulog/Log.h"
#include "Ubox/Box2d.h"
#include "Uln/PlnType.h"
#include "Uvl/TfType.h"



#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"

//#include "Png/PngCodec/Png/ImageWritePng.h"


#include "ImageMbt/ImageMbt.h"



static int	pln_cut_point( pln_type *pl, vec2f_type *p0, vec2f_type *v, vec2f_type *pm );


static int	pln_cut_point( pln_type *pl, vec2f_type *p0, vec2f_type *v, float R, vec2f_type *pm );


pt2dA_type *	pt2dA_step( pt2dA_type *apt, vec2f_type *p0, float T, pt2dA_type *capt );

pt2dA_type *	pt2dA_step_opsite( pt2dA_type *apt, vec2f_type *p0, float T, pt2dA_type *capt );


void	vec2f_dump( vec2f_type *p, vec2f_type *p1, vec2f_type *p2, char *prefix, int index,  char *suffix );


pt2dA_type *
morohing_pln_map( plnA_type *apl, plnA_type *aplt, int width, int height, int dxy, float t, pt2dA_type *apt )
{
int	i,	j,	k;
vec2f_type	p0;

	p0 = apl->p;

	
	float R = pln_radius( apl->a[0], &p0 );


	int n = (width/dxy + 1 )*(height/ dxy + 1);
	apt = pt2dA_realloc( apt, n );

	vec2f_type dp;
	dp.x = apl->p.x - aplt->p.x;
	dp.y = apl->p.y - aplt->p.y;
	plnA_translate( aplt, dp.x, dp.y );
	PLNA_DUMP( aplt, "APL", 1, "TRANSLATE" );



	for( i = 0 ; i <= height ; i += dxy ){
		for( j = 0, k = 0 ; j <= width ; j += dxy, k++ ){

			
			pt2d_type *pt = &apt->a[apt->nA++];
			pt->p.x = i;
			pt->p.y = j;



			vec2f_type	v;
			v.x = pt->p.x - p0.x;
			v.y = pt->p.y - p0.y;

			VEC2D_NORMALIZE( v );

			vec2f_type	p1,	p2;
			//if( pln_cut_point( apl->a[0], &p0, &v, &p1 ) < 0 )
			//	continue;

			if( pln_cut_point( apl->a[0], &p0, &v, R, &p1 ) < 0 )
				continue;

	//	p1 = p11;

			//p11.x -= p1.x;
			//p11.y -= p1.y;


			if( pln_cut_point( aplt->a[0], &p0, &v, &p2 ) < 0 )
				continue;



				vec2f_dump( &pt->p, &p1, &p2, "FF", apt->nA-1, NULL );

			float	a = VEC2D_INNER( v, p0 );

			float	a1 = VEC2D_INNER( v, p1 ) - a;
			float	a0 = VEC2D_INNER( v, p2 ) - a;

			float	a2 = VEC2D_INNER( v, pt->p ) - a;
			

			pt->r = a0/a1;
			if( a0 > a1 && a2 > a0 )
				pt->r = a1/a0;

			pt->f = a2;



			pt->n = v;

			vec2f_type	dp;
			dp.x = p1.x - pt->p.x;
			dp.y = p1.y - pt->p.y;
			float f = VEC2D_INNER( dp, v );

			pt->id = ( f < 0 )? 0 : 1;
		}
	}


	plnA_translate( aplt, -dp.x, -dp.y );


	int wi = width/dxy + 1;
	pt2d_type *pt = &apt->a[0];
	
	for( i = 0 ; i < height ; i += dxy, pt += 1 ){
		pt2d_type *pt1 = pt + wi;
		for( j = 0 ; j < width ; j += dxy, pt++, pt1++ ){

			pt->group = pt[0].id + pt[1].id +  pt1[0].id + pt1[1].id;
		}
	}

	apt->axis = PT2D_AXIS_YX;
	apt->type = PT2D_4V;
	PT2DA_DUMP( apt, "MOR", k, "A" );
	return( apt );
}


static int
pln_cut_point( pln_type *pl, vec2f_type *p0, vec2f_type *v, vec2f_type *pm )
{
vec2f_type	bu,	p;

float	gt,	uMin;

PLN_DUMP( pl, "AA", 1, NULL );
	
	VEC2D_RIGHT( *v, bu );

	uMin = 100;
	for( gt = 0 ; gt < pl->len ; gt += 1.0 ){
		pln_gt2p( pl, gt,  &p );

		vec2f_type	dp;
		dp.x = p.x - p0->x;
		dp.y = p.y - p0->y;

		float t = VEC2D_INNER( dp, *v );
		if( t < 0 )	continue;
		
		float u = VEC2D_INNER( dp, bu );

		if( u < 0 )	u = -u;


	


		if( ABS(u) < uMin  ){
			*pm = p;
			uMin = u;
			continue;
		}
	}

	if( uMin < 1.0 )
		return( 1 );


	return( -1 );
}



static int
pln_cut_point( pln_type *pl, vec2f_type *p0, vec2f_type *v, float R, vec2f_type *pm )
{
	vec2f_type	p;
	int	i;

//	PLN_DUMP( pl, "AA", 1, NULL );

	float	uMin,	tMin;
	uMin = tMin = 0;

	int	step = 32;
	float t = R;
	for( i = 0, t = R ; i < step ; i++ ){
		p.x = p0->x + t*v->x;
		p.y = p0->y + t*v->y;

		dPln_type d;
		pln_distanceC( pl, &p, &d );

		if( ABS(d.u) < 0.1 ){
			*pm = p;
			return( 1 );
		}


		if( i == 0 || ABS(d.u) < ABS(uMin) ){
			uMin = d.u;
			tMin = t;
			*pm = p;
		}

		t += d.u;
	}

	return( 1 );
}


void
vec2f_dump( vec2f_type *p, vec2f_type *p1, vec2f_type *p2, char *prefix, int index,  char *suffix )
{
	pt2dA_type *apt = pt2dA_alloc( 5 );
	apt->axis = PT2D_AXIS_YX;


	apt->nA = 0;
	pt2d_type *pt0 = &apt->a[apt->nA++];
	pt0->p = *p;

	pt0 = &apt->a[apt->nA++];
	pt0->p = *p1;

	 pt0 = &apt->a[apt->nA++];
	pt0->p = *p2;
	//capt->axis = PT2D_AXIS_YX;
	PT2DA_DUMP( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}



pt2dA_type *
morohing_pln_map_step( pt2dA_type *apt, vec2f_type *p0, float T, pt2dA_type *capt )
{
	int	i;

	capt = pt2dA_realloc( capt, apt->nA );
	capt->nA = 0;

	for( i = 0 ; i < apt->nA ; i++ ){
		pt2d_type *pt = &apt->a[i];
		pt2d_type *cpt = &capt->a[capt->nA++];


		cpt->group = pt->group;

		float t = pt->f*( pt->r - 1)*T + pt->f;
		cpt->p.x = p0->x + t*pt->n.x;
		cpt->p.y = p0->y + t*pt->n.y;
	}

	capt->axis = PT2D_AXIS_YX;
	capt->type = PT2D_4V;

	return( capt );
}

#ifdef _AA_

pt2dA_type *
	pt2dA_step_opsite( pt2dA_type *apt, vec2f_type *p0, float T, pt2dA_type *capt )
{
	int	i;

	capt = pt2dA_realloc( capt, apt->nA );
	capt->nA = 0;

	for( i = 0 ; i < apt->nA ; i++ ){
		pt2d_type *pt = &apt->a[i];
		pt2d_type *cpt = &capt->a[capt->nA++];


		cpt->group = pt->group;

		//float t = pt->f*( pt->r - 1)*T + pt->f;

		float	t = pt->f*( 1- pt->r)*T + pt->f*pt->r;

		cpt->p.x = p0->x + t*pt->n.x;
		cpt->p.y = p0->y + t*pt->n.y;
	}

	capt->axis = PT2D_AXIS_YX;
	capt->type = PT2D_4V;

	return( capt );
}
#endif









