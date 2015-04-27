/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/********************
 ***   VlType.h   ***
 ********************/

#ifndef	_MAP_TYPE_
#define _MAP_TYPE_

#ifdef __cplusplus
extern "C" {
#endif


#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"
#include "Umath/LT2sType.h"

// straight line segemnet 

// p + t*v		-d < t < d 


typedef struct map2d_type {

	vec2d_type	sp;

	vec2d_type	tp;

	int	use;

} map2d_type;



typedef struct map2dA_type {

	int nA;
	int	NA;

	map2d_type	*a;

} map2dA_type;


map2dA_type *	map2dA_alloc( int n );

map2dA_type *	map2dA_realloc( map2dA_type *avl, int n );

void	map2dA_destroy( map2dA_type *avl );

int		map2dA_select( map2dA_type *Avl, vec2f_type *p, float D );

void	map2d_set_lt2(map2d_type *m, lt2_type *lt, double x, double y );


int		map2dA_similarity_lt2( map2dA_type *am, lt2_type *lt );

int		map2dA_affine_lt2( map2dA_type *am, lt2_type *lt );

double	map2dA_measure_lt2( map2dA_type *am, lt2_type *lt );



struct pt2dA_type *	map2dA_to_pt2dA( map2dA_type *avl );

map2dA_type *	map2dA_from_pt2dA( struct pt2dA_type *apt );


int	map2dA_read( map2dA_type **avl, char *file );

int	map2dA_write( map2dA_type *avl, char *file );


int	map2dA_dump( map2dA_type *avl, char *prefix, int index, char *suffix );



#ifdef _DUMP
#define map2dA_DUMP( apt, name, index, ext )  map2dA_dump( apt, name, index, ext )

#else
#define map2dA_DUMP( apt, name, index, ext )

#endif


#ifdef __cplusplus
}
#endif

#endif
