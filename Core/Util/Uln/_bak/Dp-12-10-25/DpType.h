
#ifndef __DP_TYPE_H__
#define __DP_TYPE_H__

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#include	"Uigp/igp.h"
#include "Uvec/Vec2d.h"

#include "Uln/PlnType.h"

typedef struct dp_type {
	int			no;
	vec2f_type	p;
	vec2f_type	v;
} dp_type;

typedef struct dpA_type {
	int			Ni,	Nj;
	int			NI,	NJ;

	vec2f_type	p0;
	float	dxy;
	float	inv_dxy;

	dp_type	*a;

} dpA_type;




#define DPA( ad, i, j )	( ad->a + i*ad->Nj + j )

#define DPA_I( ad, p )	(( (p).y - ad->p0.x) *ad->inv_dxy )
#define DPA_J( ad, p )	(( (p).x - ad->p0.y) *ad->inv_dxy )




dpA_type *	dpA_alloc( box2i *b, int dxy );


void	dpA_destroy( dpA_type *ad );

void	dpA_clear( dpA_type *ad );

int		dpA_no( dpA_type *ad );


int		dpA_set( dpA_type *ad,  plnA_type *al, float r );

int		dpA_update( dpA_type *ad, dpA_type *sad, float angle );



void	dpA_draw( struct gpw_type *gpw, dpA_type *ad );


#endif