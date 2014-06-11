/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/**********************
 ***   Pt2dType.h   ***
 **********************/

#ifndef	_PT2D_TYPE_
#define _PT2D_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include "Uvec/vec2d.h"
#include "Uln/LnType.h"
#include "Ubox/Box2d.h"

#include "UPoly/Poly1d4Type.h"

#define	PT2D_VERSION	1

#define	PT2D_2		0

#define	PT2D_4		1

#define	PT2D_4V		2


#define PT2D_AXIS_XY	0		// NORMAL
#define PT2D_AXIS_YX	1		// VIM




	// p2d	-  2d point

typedef struct pt2d_type {
	vec2f_type	p;

	vec2f_type	n;

	float	r;	// radius

	float	f;

	float	vitality;
	float	canopy;

	// aux
	int	id;
	int	group;

	int	i[2];

	struct pt2d_type	*np;
} pt2d_type ;



typedef struct pt2dA_type {
	union {
		int	nP;
		int nA;
	};

	union {
		int	NP;
		int	NA;
	};


	int	type;

	int	axis;

	int	nGroup;
	int	mGroup;

	box2f_type	box;

	int iFrame;


	struct pt2dG_type	*gpt;

	union {
		pt2d_type	*p;
		pt2d_type	*a;
	};

} pt2dA_type;


typedef struct pt2dF_type {

	int	NA;

	int	nA;

	pt2dA_type	**a;

} pt2dF_type;


typedef struct pt2dG_type {
	int	Ni,	Nj;

	float	d;
	float	inv_d;

	box2f_type	box;

	pt2d_type	**p;

} pt2dG_type;







	// Pt2dATool.c
pt2dA_type *	pt2dA_alloc( int n );

pt2dA_type *	pt2dA_realloc( pt2dA_type*apt, int n );

void	pt2dA_destroy( pt2dA_type *av );

pt2dA_type *pt2dA_set( pt2dA_type *apt, vec2fA_type *av );


int	pt2dA_write_to_file( pt2dA_type *av, char *file );

int	pt2dA_read_from_file( pt2dA_type **av, char *file );

int	pt2dA_write_to_file_V( vec2f_type p[], int nP, char *file );



void	pt2dA_dump( pt2dA_type *apt, char *prefix, int index, char *suffix );

pt2dA_type *	pt2dA_copy( pt2dA_type *apt, pt2dA_type *capt );

pt2dA_type *	pt2dA_copy_box( pt2dA_type *apt, box2i *box, pt2dA_type *capt );

pt2dA_type *	pt2dA_from_point( vec2f_type p[], int nP );


pt2dA_type *	pt2dA_copy_group( pt2dA_type *apt, int group, int no );

pt2dA_type *	pt2dA_copy_neighbor ( pt2dA_type *apt, vec2f_type *p, float D, pt2dA_type *capt );

pt2dA_type *	pt2dA_copy_neighbor_id( pt2dA_type *apt, vec2f_type *p, float D, int mId, pt2dA_type *capt );

void	pt2dA_nGroup( pt2dA_type *apt );

void	pt2dA_set_default_group( pt2dA_type *apt );

int		pt2dA_select( pt2dA_type *apt, vec2f_type *p, float dis );

int		pt2dA_distance( pt2dA_type *apt, int ignore, vec2d *p, float *dis );


int		pt2dA_neighbor ( pt2dA_type *apt, vec2f_type *p, float D );

void	pt2dA_remove( pt2dA_type *apt, int k );

int		pt2dA_neighbor_remove( pt2dA_type *apt, vec2f_type *p, float D );

void	pt2dA_remove_box( pt2dA_type *apt, box2i *box );

void pt2dA_remove_outbox( pt2dA_type *apt, box2i *box );

void pt2dA_remove_group( pt2dA_type *apt, int group );

int		pt2dA_no( pt2dA_type *apt, box2f_type *b );
int		pt2dA_nCircle( pt2dA_type *apt, vec2f_type *p, float r );


void	pt2dA_translate( pt2dA_type *apt, float x, float y );

void	pt2dA_scale( pt2dA_type *apt, float scale );

void	pt2d_tanget_line( pt2d_type *pt0, pt2d_type *pt1, vec2f_type *p0, vec2f_type *p1, vec2f_type *v );

void	pt2dA_moment( pt2dA_type *apt, vec2f_type *p, vec2f_type *v, float *e1, float *e2 );

pt2dA_type	*	pt2dA_append( pt2dA_type *apt, pt2dA_type *sapt );

void	pt2dA_set_r( pt2dA_type *apt, float r );

void	pt2dA_set_groupId( pt2dA_type *apt, int iGroup );

void	pt2dA_box( pt2dA_type *apt );

void	pt2dA_axis_swap( pt2dA_type *apt );



	// Pt2dGroup.cpp
void	pt2dA_groupM( pt2dA_type *apt, float dis );

void	pt2dA_group( pt2dA_type *apt, float dis );

void	pt2dA_group_radius( pt2dA_type *apt, float dis );


void	pt2dA_group_n( pt2dA_type *apt, float angle );

void	pt2dA_group_nv( pt2dA_type *apt, int iGroup, vec2f_type *nv );
void	pt2dA_group_nv3( pt2dA_type *apt, int iGroup, int iGroup0, int iGroup1, vec2f_type *nv );



int		pt2dA_group_max( pt2dA_type *apt, int *iG, int *no );

void	pt2dA_group_set_id( pt2dA_type *apt, int no, int cgroup, int group );




	// Pt2dMask.cpp
void	pt2dA_mask( pt2dA_type *apt, pt2dA_type *mapt, float dis );


	// Pt2dTriangleFilter.cpp
pt2dA_type *	pt2d_triangle_filter( pt2dA_type *apt, float R, float T );


#include "Pt2dGroupType.h"


	//Pt2dApproximateLine.c
int	pt2d_approximate_line( pt2dA_type *apt, int i0, int di, vec2f_type *p0, float dis, vec2f_type *p, vec2f_type *v );

int	pt2d_approximate_line3( pt2dA_type *apt, int i0, int di, vec2f_type *p0, vec2f_type *u0, float dis, vec2f_type *p, vec2f_type *v );

	
	//Pt2dApproximateAxB.c
int	pt2d_approximate_Ax_B( pt2dA_type *apt, int i0, int i1, float *a, float *b );
float	pt2d_approximate_Ax_B_test( pt2dA_type *apt, int i0, int i1, float a, float b );



	//Pt2dApproximatePv.c
int		pt2d_approximate_line_vl( pt2dA_type *apt, int i0, int i1, struct vl2f_type *vl );

int		pt2d_approximate_line_vl_test( pt2dA_type *apt, int i0, int i1, struct vl2f_type *vl, float *av, float *max );

int		pt2d_approximate_line_vl_check( pt2dA_type *apt, int i0, int i1, struct vl2f_type *vl );


int		pt2d_approximate_line_vl_split( pt2dA_type *apt, int i0, int i1, struct vl2f_type *vl, float *m, float *sm );


int		pt2d_approximate_line_pv( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *d, float *e );

float	pt2d_approximate_line_pv_test( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float D, int *nD );

int		pt2d_approximate_line_pv_split( pt2dA_type *apt, int i0, int i1, vec2f_type *p, vec2f_type *v, float *m, float *sm );

int		pt2dA_ellipse( pt2dA_type *apt, int i0, int i1, struct ellipse_type *e );


	//Pt2dApproximateLink.c
void	pt2d_approximate_linkO( pt2dA_type *apt, vec2f_type *ctr, ln_type **link );

//void	pt2d_approximate_link( pt2dA_type *apt, vec2f_type *ctr, ln_type **gelm );
void	pt2d_approximate_link( pt2dA_type *apt, int i0, int i1, vec2d *ctr, ln_type **link );

pt2dA_type *	pt2d_from_polylink(vec2f_type *ctr, ln_type *link, float d );


void pt2d_distance_from_polylink( pt2dA_type *apt, vec2f_type *ctr, ln_type *link );

void	pt2d_to_polylink( pt2dA_type *apt, vec2d *ctr, ln_type **link );


	// Pt2ApproximatyePolynim.c
int	pt2d_approximate_polynom_1( pt2dA_type *apt, int i0, int di, vec2d *p0, vec2d *v0, float maxDv, poly1d4_type *pl );

int	pt2d_approximate_polynom_2( pt2dA_type *apt, vec2f_type *p0, vec2f_type *v0, int i0, int i1, poly1d4_type *pl );

int	pt2d_approximate_polynom_2O( pt2dA_type *apt, int i0, int di, vec2f_type *p0, vec2f_type *u0, float dis, poly1d4_type *pl );



	// Pt2dCurvtur.c
void	pt2dA_curvtur( pt2dA_type *apt );

float	pt2dA_curvtur_1( pt2dA_type *apt, vec2f_type *p0,int i0, int i1, vec2f_type *n, float *curvtur );

pt2dA_type *	pt2dA_smooth( pt2dA_type *apt, float r, pt2dA_type	*tapt );


	// Pt2dLine.c
pt2dA_type *	pt2dA_get_pt( pt2dA_type *apt, int i0, pt2dA_type *capt );

int				pt2dA_get_ptN( pt2dA_type *apt, int i0, int i1, int nP, pt2dA_type *capt );

int	pt2dA_get_prev( pt2dA_type *apt, int i0 );

int	pt2dA_get_next( pt2dA_type *apt, int i0 );

int	pt2dA_get_nextN( pt2dA_type *apt, int i0, int prev );



	// Pt2dDensity.c
void	pt2dA_density_filter( pt2dA_type *apt, float T );

void	pt2dA_density_filter_r( pt2dA_type *apt, float T );

void	pt2dA_density_merge( pt2dA_type *apt, pt2dA_type *tapt, float T );

void	pt2dA_density_filter_vitality( pt2dA_type *apt, float T );




	// Pt2dCircle.cpp
pt2dA_type *	pt2dA_circle( vec2f_type *ctr, float r, float dt, pt2dA_type *apt );





	//   Pt2dApproximateAb.c

// y = 0*x + b
int	pt2dA_app_b( pt2dA_type *apt, int i0, int i1, float a0, float b0, float maxY, float *b );

// y = a*x + b
int	pt2dA_app_ab( pt2dA_type *apt, int i0, int i1, float a0, float b0, float maxY, float *a, float *b );

int	pt2dA_app_ab_test( pt2dA_type *apt, int i0, int i1, float a0, float b0, float x0, float x1, float maxY, float *ax0, float *ax1 );


// p2tA_componnet
struct plnA_type *	pt2dA_componnet( pt2dA_type *apt, float dT, int nT );

struct pln_type *	pt2dA_bounding_contour( pt2dA_type *apt );



	// Pt2dGroupV.c
struct pt2dGroupA_type *	pt2dA_group_v( pt2dA_type *apt, float d );
struct pt2dGroupA_type *	pt2dA_group_v1( pt2dA_type *apt, float d );

pt2dGroupA_type *	pt2dA_group_vG( pt2dA_type *apt, float d );




	// Pt2dGToole.c
pt2dG_type *pt2dG_alloc( box2f_type *b, float d );

void	pt2dG_destroy( pt2dG_type *gpt );

void	pt2dG_clear( pt2dG_type *gpt );

int		pt2dG_create( pt2dA_type *apt, float d );

int		pt2dG_reset( pt2dA_type *apt );

pt2dA_type *	pt2dG_copy_neighbor( pt2dA_type *apt, vec2d *p, float D, pt2dA_type *capt );


	// Pt2dFtool.cpp
pt2dF_type *	pt2dF_alloc( int n );

void	pt2dF_destroy( pt2dF_type *vpl );

void	pt2dF_add( pt2dF_type *vpl, pt2dA_type *apl, int iFrame );

void	pt2dF_clear( pt2dF_type *vpl, int iFrame );

int	pt2dF_read( pt2dF_type **fpt, char *file );

FILE *	pt2dF_write_open( char *file );

int	pt2dF_write_header( FILE *fp );



#ifdef _DUMP
#define PT2DA_DUMP( apt, name, index, ext )  pt2dA_dump( apt, name, index, ext )

#else
#define PT2DA_DUMP( apt, name, index, ext )

#endif



#ifdef __cplusplus
}
#endif

#endif
