/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/**********************
 ***   Vl3dType.h   ***
 **********************/

#ifndef	_VL3D_TYPE_
#define _VL3D_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>


#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"

// 3D  straight line segemnet 

// p + t*v		t0 < t < t1 


typedef struct vl3d_type {

	vec3d_type	p;

	vec3d_type	v;

	double t0,	t1;


	vec3d_type	ev;


	int	id;

	int	group;

} vl3d_type;



typedef struct vl3dA_type {

	int nA;
	int	NA;

	vl3d_type	*a;

} vl3dA_type;


#define VL3D_ALLOC()	( vl3d_type *)malloc( sizeof(vl3d_type) );

#define VL3D_DESTROY( vl )	free( vl );



vl3dA_type *	vl3dA_alloc( int n );


vl3dA_type *	vl3dA_realloc( vl3dA_type *avl, int n );


void	vl3dA_destroy( vl3dA_type *avl );


void	vl3dA_add( vl3dA_type *avl, vl3d_type *vl );

void	vl3dA_remove( vl3dA_type *avl, int i0 );


void	vl3d_points( vl3d_type *vl, vec3d_type *p0, vec3d_type *p1 );

void	vl3d_set( vl3d_type *vl, vec3d_type *w0, vec3d_type *w1 );



double	vl3d_distane2( vl3d_type *vl, vec3f_type *p );
double	vl3dA_distance2( vl3dA_type *av, vec3f_type *p );

vl3dA_type *vl3dA_create( vec3fA_type *a );



int		vl3dA_write( vl3dA_type *avl, char *file );

int		vl3d_write_to_file( vl3d_type *pl, char *file );

int		vl3dA_read( char *file, vl3dA_type **avl );


void	vl3dA_dump( vl3dA_type *avl, char *prefix, int index, char *suffix );

void	vl3d_dump( vl3d_type *pl, char *prefix, int index, char *suffix );


void	vl3d_write( vl3d_type *vl, FILE *fp );

void	vl3d_read( vl3d_type *vl, FILE *fp );

void	vl3d_writeb(vl3d_type *vl, FILE *fp );

void	vl3d_readb(vl3d_type *vl, FILE *fp );


	// Vl3dStl.c
int	vl3dA_write_stl( vl3dA_type *avl, char *file );



#ifdef __cplusplus
}
#endif


#endif
