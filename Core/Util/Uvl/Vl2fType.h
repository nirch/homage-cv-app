/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/********************
 ***   VlType.h   ***
 ********************/

#ifndef	_VL_TYPE_
#define _VL_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include "Ucolor/ColorType.h"

#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"

// straight line segemnet 

// p + t*v		-d < t < d 


typedef struct vl2f_type {

	vec2f_type	p;

	vec2f_type	v;

	float d;


	vec2f_type	ev;
	float		e;

	int	id;

	int	group;

	color_type	color[2];

	short	ng[2];

} vl2f_type;



typedef struct vl2fA_type {

	int nA;
	int	NA;

	int	iFrame;
	int fit;

	vl2f_type	*a;

} vl2fA_type;


#define VL2F_ALLOC()	( vl2f_type *)malloc( sizeof(vl2f_type) );

#define VL2F_DESTROY( vl )	free( vl );


void	vl2f_swap( vl2f_type *vl );

float	vl2f_distance( vl2f_type *vl, vec2f_type *p );

float	vl2f_distance2( vl2f_type *vl, vec2f_type *p );

int		vl2f_lt2( vl2f_type *s0, vl2f_type *s1, vl2f_type *t0, vl2f_type *t1, struct lt2_type *lt );


void	vl2f_compute_lt2( vl2f_type *vl, struct lt2_type *lt );

int		vl2f_middle( vl2f_type *vl0, vl2f_type *vl1, vl2f_type *vl );



vl2fA_type *	vl2fA_alloc( int n );

vl2fA_type *	vl2fA_realloc( vl2fA_type *avl, int n );

vl2fA_type *	vl2fA_copy( vl2fA_type *bavl, vl2fA_type *avl );


void	vl2fA_destroy( vl2fA_type *avl );

int		vl2fA_select( vl2fA_type *Avl, vec2f_type *p, float D );

void	vl2f_points( vl2f_type *vl, vec2f_type *p0, vec2f_type *p1 );

void	vl2f_box( vl2f_type *vl, struct box2f_type *b );


void	vl2f_set( vl2f_type *vl, vec2f_type *p0, vec2f_type *p1 );

void	vl2fA_reorder( vl2fA_type *avl, struct pt2dA_type *apt, float dis );

void	vl2f_cutpoint( vl2f_type *vl0, vl2f_type *vl1, vec2f_type *p );

void	vl2fA_inverse_direction( vl2fA_type *avl );

void		vl2fA_set_group( vl2fA_type *avl, int iGroup );

vl2fA_type *	vl2fA_from( vl2f_type *av[], int nV, vl2fA_type *avl );



vl2fA_type *	vl2fA_transform_lt2( vl2fA_type *bavl, struct lt2_type *lt, vl2fA_type *avl );

void	vl2f_transform_lt2( vl2f_type *bvl, struct lt2_type *lt, vl2f_type *vl );



	// Vl2fMatch.cpp
int	vl2fA_match( vl2fA_type *bavl, vl2fA_type *avl );

int	vl2fA_match_lt( vl2fA_type *bavl, struct lt2_type *lt, float aT, float dT, vl2fA_type *avl );




int	vl2fA_match_no( vl2fA_type *avl, int ng );

int	vl2fA_match_set( vl2fA_type *bavl, vl2fA_type *avl, int ng );

int	vl2fA_match_set_inv( vl2fA_type *bavl, vl2fA_type *avl, int ng );

vl2fA_type *	vl2fA_match_field( vl2fA_type *bavl, vl2fA_type *avl1, int ng, vl2fA_type *avl );

vl2fA_type *	vl2fA_match_field_ev( vl2fA_type *bavl, int ng, vl2fA_type *avl );

int			vl2fA_match_connect( vl2fA_type *bavl, vl2fA_type *avl, int ng );



	// Vl2fGroup.cpp
struct pt2dGroupA_type *	vl2fA_group_v( vl2fA_type *avl, float d );

void	vl2fA_group_replace_id( vl2fA_type *avl, int no, int cgroup, int group );





	// Vl2fFitLt.cpp
int	vl2fA_fit_lt_smart( vl2fA_type *bavl,  vl2fA_type *avl, int g, struct lt2_type *lt );
int	vl2fA_fit_lt( vl2fA_type *bavl,  vl2fA_type *avl, struct lt2_type *lt );

int	vl2fA_fit_affine_lt( vl2fA_type *bavl,  vl2fA_type *avl, int g, struct lt2_type *lt );

vl2fA_type *	vl2fA_fit_lt_field( vl2fA_type *bavl,  struct lt2_type *lt, vl2fA_type *avl );


	// Vl2fProncipalDirection.cpp
int	vl2fA_principal_direction( vl2fA_type *avl, vl2f_type *rvl );


struct pt2dA_type *	vl2fA_to_pt2dA( vl2fA_type *avl );

vl2fA_type *	vl2fA_from_pt2dA( struct pt2dA_type *apt );


int	vl2fA_read( vl2fA_type **avl, char *file );

int	vl2fA_write( vl2fA_type *avl, char *file );

int	vl2fA_write_pt( vl2fA_type *avl, char *file );


int	vl2fA_dump( vl2fA_type *avl, char *prefix, int index, char *suffix );



#ifdef _DUMP
#define VL2FA_DUMP( apt, name, index, ext )  vl2fA_dump( apt, name, index, ext )
#define VL2FA_DUMPF( apl, name, index, ext, flag )  if( flag )	vl2fA_dump( apl, name, index, ext )
#else
#define VL2FA_DUMP( apt, name, index, ext )
#define VL2FA_DUMPF( apl, name, index, ext, flag )
#endif


#ifdef __cplusplus
}
#endif

#endif
