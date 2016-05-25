/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/******************
 ***  PtLine.h  ***
 ******************/
#ifndef _PT_LINE_
#define _PT_LINE_

#ifdef __cplusplus
extern "C" {
#endif

#include "Uvl/Vl2fType.h"
#include "Ucamera/Pt2dType.h"
#include "Ucolor/ColorType.h"


#define	PL_VERSION	4
#define CL_VERSION  1	

#define PLN_OPEN	1
#define PLN_CLOSE	2
#define PL_TRANSPARENT	4

#define PLN_WEEK	0x04


#define PLN_PARALLEL_R 0x10
#define PLN_PARALLEL_L 0x20
#define PLN_PARALLEL 0x30


typedef struct pln_type {

	vec2f_type	ctr;
	ln_type *link;

	float	len;

	int	state;
	int	step;
	int	type;


	int	group;

	int	id;

	float	qulity;
	float	height;
	float	size;
	
	// RGB
	color_type	color[2];		// rgb value

	box2f_type	b;


	struct ellipse_type *e;
	struct  crPlnA_type *ac;
	struct intA_type	*ai;
	struct floatA_type	*af;


} pln_type;





typedef struct plnA_type {
	int	NA;

	int	nA;
	int	nGroup;	// group number
	int mGroup;	// max group id

	int iLine;	// selected line;

	int	iFrame;

	float	qulity;

	// axis
	vec2d	p;
	float	angle;
	float	scale;

	vec2f_type	v;




	pln_type	**a;

} plnA_type;


//typedef struct plnB_type {
//	int	NA;
//
//	int	nA;
//
//	plnA_type	**a;
//
//} plnB_type;


typedef struct plnF_type {
	int	NA;

	int	nA;

	int	frame0;

	plnA_type	**a;

} plnF_type;



typedef struct pPln_type {		//use to define parallel relation between pln
	pln_type	*pl;
	int			iPl;

	float	gt;
	float	u;

	float	sgt;
	vec2f_type	v;
	vec2f_type	p;

	float	d;

} pPln_type;


#include "PlnItrator.h"


pln_type *pln_alloc( int no );

void	pln_destroy( pln_type *pl );

pln_type *	pln_copy( pln_type *spl );


int		pln_set( pln_type *pl, vec2f_type *ctr, ln_type *l );

void	pln_set_length( pln_type *pl );


void	pln_inverse( pln_type *pl );
int		plnA_length_filter( plnA_type *apl, float minLen );

void	plnA_set_id( plnA_type *apl, int id );


int		pln_create( pln_type *pl, pt2dA_type *apt );

void	pln_trim( pln_type *pl, int direct, float gt );

pln_type *	pln_copy_sub( pln_type *spl, float gt0, float gt1 );

pln_type *	pln_copy_subD( pln_type *spl, float gt0, float gt1, float D );


pln_type *pln_copy_subR( pln_type *spl, float gt0, float gt1 );


void	pln_appendL( pln_type *pl, vec2f_type *ctr, ln_type *link );

pln_type *	pln_append( pln_type *pl, pln_type *pl1 );

void	pln_end_point( pln_type *pl, vec2d *p );

void	pln_remove_zero( pln_type *pl );


void	pln_gt2p( pln_type *pl, float gt, vec2f_type *p );


void	pln_gt2lt( pln_type *pl, float gt, vec2f_type *p, ln_type **l, float *t );


void	pln_tanget( pln_type *pl, float gt, vec2f_type *v );
void	pln_Ntanget( pln_type *pl, float gt, vec2f_type *v );


void	pln_tangetP( pln_type *pl, float gt, vec2f_type *p, vec2f_type *v );



pln_type *	pln_from_point( vec2d p[], int no, int Fclose );


pln_type *	pln_from_box( box2f_type *b );

pln_type *	pln_from_boxi( box2i_type *b );

plnA_type *	plnA_from_boxi( box2i_type ab[], int nB );



pln_type *	pln_from_circle( vec2f_type *ctr, float R, float dr );

pln_type *	pln_from_sub_circle( vec2f_type *ctr0, float R, vec2f_type *p0, vec2f_type *p1, float dr );

pln_type *	pln_from_vl_N( struct vl2f_type *vl );

pln_type *	pln_from_vl( struct vl2f_type *vl );
plnA_type *	plnA_from_vlA( vl2fA_type *avl, plnA_type *apl );


void	pln_to_vl_1( pln_type *pl, struct vl2f_type *v );
vl2fA_type *	plnA_to_vlA( plnA_type *apl, vl2fA_type *avl );



float	plnA_group_length( plnA_type *aP, int group );




pln_type *	pln_from_lnA( ln_type al[], int nAl );

void	pln_scale( pln_type *pl, float scale );

void	pln_scaleP( pln_type *pl, vec2d *point, float scale );


void	pln_translate( pln_type *pl, float x, float y );

void	plnA_clear( plnA_type *apl );

void	plnA_destroy_pl( plnA_type *apl, int i0 );

void	plnA_destroy_group( plnA_type *apl, int group );


plnA_type *	plnA_copy( plnA_type *apl, int fData, plnA_type *capl );

plnA_type *	plnA_copy_len( plnA_type *apl, float dT, int fData, plnA_type *capl );


pln_type *	plnA_get( plnA_type *apl, int i0, int detouch );





int	pln_distance( pln_type *pl, vec2f_type *p, dPln_type *d );

int	pln_distanceG( pln_type *pl, float gt0, float gt1, vec2f_type *p, dPln_type *d );


int	pln_distance_pln( pln_type *bpl, pln_type *pl, dPln_type *md );

int	pln_distance_pln_u( pln_type *bpl, pln_type *pl, dPln_type *md );

int	pln_distance_plnM( pln_type *bpl, pln_type *pl, float *m0, float *m1 );

	// 1 pl in bpl,   3 bpl in pl,    2 pl cut bpl,    -1 separate
int	plnC_intersection( pln_type *bpl, pln_type *pl, float *dis );

void	pln_box( pln_type *pl, box2f_type *box );

void	pln_set_box( pln_type *pl );

void	plnA_set_box( plnA_type *apl );


plnA_type *		plnA_alloc( int n );

plnA_type *		plnA_realloc( plnA_type *apl, int n );


void	plnA_destroy( plnA_type *apl );

void	plnA_destroy_header( plnA_type *apl );

plnA_type *	plnA_from_pl( pln_type *pl );


int		plnA_decrease( plnA_type *apl );

int		plnA_select( plnA_type *apl, vec2f_type *v, float dis );

int		plnA_selectI( plnA_type *apl, vec2f_type *v, int ignore, float mDis, dPln_type *dp );


float	plnA_length( plnA_type *apl );

float	plnA_max_length( plnA_type *aP );


int		plnA_distance( plnA_type *apl, vec2f_type *p, float D, pln_type **spl, dPln_type *sd );


int	plnA_add( plnA_type *apl, pln_type *pl );

void	plnA_addA( plnA_type *apl, plnA_type *apl1 );



void	plnA_translate( plnA_type *aP, float x, float y );

void	plnA_scale( plnA_type *aP, float scale );


void	pln_sample( pln_type *pl, float t0, float r, int n, int direct, pt2dA_type *apt );

pt2dA_type *	plnA_sample( plnA_type *apl, pt2dA_type *apt );

pt2dA_type *	pln_sampleN( pln_type *pl, float D, float r, pt2dA_type *apt );

pt2dA_type * pln_sampleP( pln_type *pl, float gt0, float gt1, float dt, pt2dA_type *apt );

pt2dA_type *	pln_samplePV( pln_type *pl, float gt0, float gt1, float dt, pt2dA_type *apt );


pt2dA_type *	pln_sampleC( pln_type *pl, float gt0, int n, float dt, pt2dA_type *apt );


pln_type *	pln_split( pln_type *spl, float gt, float dt );


void	pln_tangetS( pln_type *pl, float gt, vec2f_type *v );

void	pln_pv( pln_type *pl, float gt, float dt, int nDt, vec2f_type *pd, vec2f_type *vd, float *d );

void	pln_pv_d( pln_type *pl, float gt0, float gt1, float dt, struct vl2f_type *vl );

void	pln_app_vl( pln_type *pl, float gt, float dt, vl2f_type *vl );


int		pln_pv_p( pln_type *pl, vec2f_type *p, float dt, int nDt, struct vl2f_type *vl );



void	pln_lt2( pln_type *pl, lt2_type *lt );


void	plnA_lt2( plnA_type *apl, lt2_type *lt );

pln_type *	pln_affine_lt( pln_type *bpl, lt2_type *lt, pln_type *pl);




void	plnA_iLine_set( plnA_type *apl, int iLine );

int		plnA_iLine_get( plnA_type *apl );


	// PlnWrite.c
void	plnF_dump( plnF_type *apl, char *prefix, int index, char *suffix );

void	plnA_dump( plnA_type *apl, char *prefix, int index, char *suffix );

void	pln_dump( pln_type *pl, char *prefix, int index, char *suffix );

int		plnA_write( plnA_type *apl, char *file );

void	pln_write( pln_type *pl, FILE *fp );


//int		plnA_read( char *file, plnA_type **apl );
int		plnA_read( plnA_type **apl, char *file );

int	pln_read( pln_type **pl,  FILE *fp );



int		pln_write_to_file( pln_type *pl, char *file );



int	plnF_read(  plnF_type **vpl, char *file );

int	plnF_write( plnF_type *fpl, char *file );




// PlnParallel.c
int	pln_parallel2(  pln_type *bpl, pln_type *pl, int i, float aT, float T0, float T1, pPln_type ap[] );

int	pln_parallel( pln_type *bpl, pln_type *pl, int i, float gt, float aT, float T0, float T1, pPln_type *pp );

int	pln_parallel_distance(  pln_type *bpl, pln_type *pl, float dt, float aT, float D0, float D1, float *u );

float	pln_straightline_measure( pln_type *pl, float gt, float dt, int n );

int		pln_straightline( pln_type *pl, float gt0, float gt1, float dt, vl2f_type *vl );

plnA_type *	plnA_straightline( plnA_type *apl, plnA_type *apl1 );



void	pPln_reorder( pPln_type a[], int n );

void	pPln_inverse( pPln_type *p );


	// PlnAParallelDistance.cpp
float	plnA_parallel_distance(  plnA_type *apl, float r0, float r1 );

float	plnA_parallel_distanceH(  plnA_type *apl, float r0, float r1 );



plnA_type *
plnA_get_group( plnA_type *apl, int group, int Fdetouch, plnA_type *capl );


plnA_type *plnA_copy_group( plnA_type *apl, int group, int fCopy, plnA_type *capl );

plnA_type *	plnA_copy_step( plnA_type *apl, int step, plnA_type *capl );

void	plnAG_set_step( plnA_type *apl, int group, int step );

plnA_type *	plnA_move_group( plnA_type *apl, int group, plnA_type *gapl );

plnA_type *	plnA_append( plnA_type *tapl, plnA_type *apl );


void	plnA_set_groupId( plnA_type *apl, int group );

void	plnA_set_qulity( plnA_type *apl, float qulity );


void	pln_close( pln_type *pl, float T );

void	pln_close_test( pln_type *pl, float T );


float	pln_radius( pln_type *pl, vec2f_type *p0 );

void	plnA_reorder_length( plnA_type *apl );

pln_type *	plnA_group_get_pl( plnA_type *apl, int group );



typedef struct lnFit_type {
	float gt;
	lt2_type	lt;

	float	dis;
	float cover;

	float cover1;

} lnFit_type;


	// PlnFit.cpp
int	pln_fit( pln_type *pl, pln_type *bpl0, float gt0, float gt1, int fside, int cycle, float T, lnFit_type *f );

int	pln_fit_local( pln_type *pl, pln_type *bpl, float gt, float dt, int side, int cycle, lnFit_type *f );


int	pln_fit_step( pln_type *pl, pln_type *bpl, float gt0, float gt1, int fside, lt2_type *lt );


int	pln_fit_compare( pln_type *pl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis );


	// PlnFitAa.cpp
int	plnA_fit( plnA_type *apl, pln_type *bpl0, float gt0, float gt1, int cycle, float T, lnFit_type *f );

int	plnA_fitT( plnA_type *apl, pln_type *bpl0, float gt0, float gt1, int cycle, float T, lnFit_type *f );

int	plnA_fitV( plnA_type *apl, pln_type *bpl0, float gt0, float gt1, int cycle, vec2f_type *V, float T, lnFit_type *f );


int	plnA_fit_compare( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis );

int	plnA_fit_compareN( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float *cover, float *dis );

int	plnA_fit_compareW( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float W, float *cover, float *dis );

int	plnA_fit_compareG( plnA_type *apl, pln_type *bpl, float gt0, float gt1, float dT, float W, float *cover, float *dis );


	// PlnGroup.c

void	plnA_group( plnA_type *apl, float dT );

void plnA_group_parallel( plnA_type *apl, float dt, float aT, float D0, float D1 );

int	plnA_group_max( plnA_type *apl, int *iG, int *no );

pt2dGroupA_type *	pt2dGroupA_set_pln( pt2dGroupA_type *ag, plnA_type *apl );


int	plnA_group_append( plnA_type *apl, plnA_type *gapl, float dT );



	// PlnBounding.cpp
pln_type *	plnA_bounding_convex( plnA_type *apl, float radius );

pln_type *	pln_bounding_convex( pln_type *bpl, float radius );

pln_type *	plnA_bounding( plnA_type *apl, float radius );


	// PlnCrop.cpp
plnA_type *	plnA_crop_box( plnA_type *apl, box2f_type *b, plnA_type *tapl );

pln_type *	pln_crop_box( pln_type *pl, box2f_type *b );


// PlnParallelDistance.c
float	plnA_parallel_distance(  plnA_type *apl, float r0, float r1 );

float	plnA_parallel_distanceH(  plnA_type *apl, float r0, float r1 );


	// PlnApproximate.cpp
int	pln_approximateA( pt2dA_type *apt, int fClose, pln_type **pl );

int	pln_approximate( gapp_type *gapp, int fClose, pln_type **pl );


	// lnFromGapp.c
pln_type *pln_from_gapp( gapp_type *gapp );

pln_type *	pln_from_gappA( gapp_type *gapp, int i0, int i1 );


void	pln_to_gapp( pln_type *pl, float dt, gapp_type *gapp );



	// PlnEigen.cpp
int	pln_eigen( pln_type *pl, float dt, struct eigen2d_type *e );

int	pln_eigenS( pln_type *pl, float gt0, float gt1, float dt, struct eigen2d_type *e );

int	plnA_eigen( plnA_type *apl, struct eigen2d_type *e );


pt2dA_type * pln_apt( pln_type *pl, float dt );

void	plnA_eigen_axis( plnA_type *apl );



	// PlnSide.c 
// if the interior of the close contour pl is on right  return 1 otherwise  0
int		pln_interior_side( pln_type *pl );


void	plnA_interior_force_right( plnA_type *apl );

void	pln_interior_force_right( pln_type *pl );

void		pln_interior_force_left( pln_type *pl );



// return 1 if p inside p2, else 0
int		pln_inside( pln_type *p, pln_type *p2 );

int		pln_insideA( pln_type *p, pln_type *p2 );

float	pln_insideP( pln_type *pl, pln_type *bpl );



// if the point  is in the right side of the close contour pl  return 1  otherwise 0
int		pln_point_side( pln_type *pl, vec2d *p );


	// PlnSmooth.cpp
int		plnA_smooth( plnA_type *apl );

pln_type *	pln_smooth( pln_type *pl );


int	plnA_smoothN( plnA_type *apl, int dr );

pln_type * pln_smoothN( pln_type *pl, int dr );


	// PlnSmoothA.cpp
int	plnA_smoothA( plnA_type *apl, int d );

pln_type *	pln_smoothA( pln_type *pl, int d );



	// PlnSmoothSegment.cpp
int	pln_smooth_segment( pln_type *pl, float minLen, gapp_type *gapp, float ag[], int *nG );


	// PlnIn.cpp
float pln_in_box( pln_type *pl, box2f_type *b );


	// PlnBoxV.cpp;
int	plnA_boxV( plnA_type *apl, vec2f_type *p0, vec2f_type *v, box2f_type *b );

int	pln_boxV( pln_type *pl, vec2f_type *p0, vec2f_type *v, box2f_type *b );

int	plnA_boxVV( plnA_type *apl, vec2f_type *p0, vec2f_type *v, box2f_type *b );

int	pln_boxVV( pln_type *pl, vec2f_type *p0, vec2f_type *v, box2f_type *b );


int	plnA_boxVx( plnA_type *apl, float d, vec2f_type *p0, vec2f_type *v, box2f_type *b );


int	plnA_copy_boxV( plnA_type *apl, float d, vl2f_type *vl, float dy, int fCopy, plnA_type *tapl );


	// PlnFTool.cpp
plnF_type *		plnF_alloc( int n );

void	plnF_destroy( plnF_type *fpl );

void	plnF_add( plnF_type *vpl, plnA_type *apl, int iFrame );

void	plnF_append( plnF_type *fpl, plnA_type *apl, int iFrame );


plnA_type *	plnF_get( plnF_type *vpl, int iFrame );

void	plnF_clear( plnF_type *vpl, int iFrame );

void	plnF_destroy_empty( plnF_type *fpl );


	// PlnPolygon.cpp
vec2fA_type *	pln_to_polygon( pln_type *pl, float DT );



	// PlnStraightlineSplit.cpp
plnA_type *	plnA_straightline_split( plnA_type *apl, float minLen, plnA_type *apl1 );

plnA_type * plnA_straightline_remove( plnA_type *apl, float minLen, plnA_type *apl1 );



	// PlnFilterParallelIn.cpp
int	plnF_filter_parallel_in( plnF_type *fpl, float dT );

int	plnA_filter_parallel_in( plnA_type *apl, float dT );

int	plnA_filter_parallel_in_1( plnA_type *apl, int i0, float dT );



	// PlnSample.cpp
vl2fA_type *	plnA_sample_vl( plnA_type *apl, vl2fA_type *avl );

vl2fA_type *	pln_sample_vl( pln_type *pl, int d, vl2fA_type *avl );


	// PlnDistance.cpp
int	pln_distanceC( pln_type *pl, vec2f_type *p, dPln_type *d );

int	pln_distance_plnC( pln_type *bpl, pln_type *pl, float *m0, float *m1 );



#define PLNA_DESTROY( apl )	{ \
								if( apl != NULL ){ \
									plnA_destroy( apl ); \
									apl = NULL; \
								} \
							}


#ifdef _DUMP
#define PLNF_DUMP( fpl, name, index, ext )  plnF_dump( fpl, name, index, ext )
#define PLNA_DUMP( apl, name, index, ext )  plnA_dump( apl, name, index, ext )
#define PLN_DUMP( pl, name, index, ext )  pln_dump( pl, name, index, ext )
#define PLNA_DUMPF( apl, name, index, ext, flag )  if( flag )	plnA_dump( apl, name, index, ext )

#else
#define PLNF_DUMP( fpl, name, index, ext ) {};
#define PLNA_DUMP( apl, name, index, ext )	{};
#define PLN_DUMP( pl, name, index, ext )	{};
#define PLNA_DUMPF( apl, name, index, ext, flag ) {};
#endif



#ifdef __cplusplus
}
#endif

#endif