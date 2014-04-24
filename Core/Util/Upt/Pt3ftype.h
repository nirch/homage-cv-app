/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/**********************
 ***   Pt3fType.h   ***
 **********************/

#ifndef	_PT3F_TYPE_
#define _PT3F_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include "Uvec/vec3d.h"
#include "Ubox/Box3d.h"



#define PT3_OPEN    0
#define PT3_CLOSE	1




	// p2d	-  2d point

typedef struct pt3f_type {
	vec3f_type	p;

	vec3f_type	n;


	// aux
	int	id;
	int	group;

	int	i[2];

} pt3f_type ;



typedef struct pt3fA_type {

	int nA;

	int	NA;


	int	type;
	int state;	 // PT3_OPEN    PT3_CLOSE

	pt3f_type	*a;


} pt3fA_type;






	// Pt2dATool.c
pt3fA_type *	pt3fA_alloc( int n );

pt3fA_type *	pt3fA_realloc( pt3fA_type*apt, int n );

void	pt3fA_destroy( pt3fA_type *av );

pt3fA_type *pt3fA_set( pt3fA_type *apt, vec3fA_type *av );

pt3f_type *pt3fA_add( pt3fA_type *av, vec3f_type *p );


pt3fA_type *	pt3fA_copy( pt3fA_type *apt, pt3fA_type *capt );

pt3fA_type *	pt3fA_copy_box( pt3fA_type *apt, box3i_type *box, pt3fA_type *capt );


pt3fA_type *	pt3fA_copy_group( pt3fA_type *apt, int group, int no );

pt3fA_type *	pt3fA_copy_neighbor ( pt3fA_type *apt, vec3f_type *p, float D, pt3fA_type *capt );

pt3fA_type *	pt3fA_copy_neighbor_id( pt3fA_type *apt, vec3f_type *p, float D, int mId, pt3fA_type *capt );


int		pt3fA_no( pt3fA_type *apt, box3f_type *b );
int		pt3fA_nCircle( pt3fA_type *apt, vec3f_type *p, float r );


void	pt3fA_translate( pt3fA_type *apt, float x, float y, float z );

void	pt3fA_scale( pt3fA_type *apt, float scale );

double	pt3dA_distance2( pt3fA_type *apt, vec3f_type *p );




	// pt3Io.c

void	pt3fA_dump( pt3fA_type *apt, char *prefix, int index, char *suffix );

//#define PT3_VERSION	1

int	pt3fA_write_to_file( pt3fA_type *apt, char *file );

int	pt3fA_read_from_file( pt3fA_type **av, char *file );




#ifdef _DUMP
#define pt3fA_DUMP( apt, name, index, ext )  pt3fA_dump( apt, name, index, ext )

#else
#define pt3fA_DUMP( apt, name, index, ext )

#endif



#ifdef __cplusplus
}
#endif

#endif
