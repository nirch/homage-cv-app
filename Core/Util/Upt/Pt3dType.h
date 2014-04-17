/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/**********************
 ***   Pt3dType.h   ***
 **********************/

#ifndef	_PT3D_TYPE_
#define _PT3D_TYPE_

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "Uvec/vec3d.h"
#include "Ubox/Box3d.h"



#define PT3_OPEN    0
#define PT3_CLOSE	1




	// p2d	-  2d point

typedef struct pt3d_type {
	vec3d_type	p;

	vec3d_type	n;


	// aux
	int	id;
	int	group;

	int	i[2];

} pt3d_type ;



typedef struct pt3dA_type {

	int nA;

	int	NA;


	int	type;
	int state;	 // PT3_OPEN    PT3_CLOSE

	pt3d_type	*a;


} pt3dA_type;



typedef struct pt3dG_type {

	int nA;

	int	NA;


	pt3dA_type	**a;

} pt3dG_type;




	// Pt2dATool.c
pt3dA_type *	pt3dA_alloc( int n );

pt3dA_type *	pt3dA_realloc( pt3dA_type*apt, int n );

void	pt3dA_destroy( pt3dA_type *av );

pt3dA_type *pt3dA_set( pt3dA_type *apt, vec3dA_type *av );

pt3d_type *pt3dA_add( pt3dA_type *av, vec3d_type *p );


pt3dA_type *	pt3dA_copy( pt3dA_type *apt, pt3dA_type *capt );

pt3dA_type *	pt3dA_copy_box( pt3dA_type *apt, box3i_type *box, pt3dA_type *capt );


pt3dA_type *	pt3dA_copy_group( pt3dA_type *apt, int group, int no );

pt3dA_type *	pt3dA_copy_neighbor ( pt3dA_type *apt, vec3f_type *p, float D, pt3dA_type *capt );

pt3dA_type *	pt3dA_copy_neighbor_id( pt3dA_type *apt, vec3f_type *p, float D, int mId, pt3dA_type *capt );


int		pt3dA_no( pt3dA_type *apt, box3f_type *b );
int		pt3dA_nCircle( pt3dA_type *apt, vec3f_type *p, float r );


void	pt3dA_translate( pt3dA_type *apt, float x, float y, float z );

void	pt3dA_scale( pt3dA_type *apt, float scale );

double	pt3dA_distance2( pt3dA_type *apt, vec3d_type *p );

float	pt3dA_length( pt3dA_type *apt );

int	pt3dA_t2p( pt3dA_type *apt, float t, vec3f_type *p );

pt3dG_type *pt3dG_alloc( int n );

void	pt3dG_destroy( pt3dG_type *apt );



	// pt3Io.c

void	pt3dA_dump( pt3dA_type *apt, char *prefix, int index, char *suffix );

//#define PT3_VERSION	1
int	pt3dG_write_to_file( pt3dG_type *apt, char *file );
int	pt3dA_write_to_file( pt3dA_type *apt, char *file );
int	pt3dA_write( pt3dA_type *apt, FILE *fp );


int	pt3dA_read_from_file( pt3dA_type **av, char *file );
int	pt3dA_read( pt3dA_type **apt, FILE *fp );




#ifdef _DUMP
#define pt3dA_DUMP( apt, name, index, ext )  pt3dA_dump( apt, name, index, ext )

#else
#define pt3dA_DUMP( apt, name, index, ext )

#endif



//#ifdef __cplusplus
//}
//#endif

#endif
