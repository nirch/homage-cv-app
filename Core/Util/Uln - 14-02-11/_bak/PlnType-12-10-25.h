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

#include "Ucamera/Pt2dType.h"

#define PLN_OPEN	1
#define PLN_CLOSE	2
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

	int	group;

	int	id;

	float	qulity;

} pln_type;





typedef struct plnA_type {
	int	NA;

	int	nA;
	int	nGroup;	// group number
	int mGroup;	// max group id

	int iLine;	// selected line;

	float	qulity;

	pln_type	**a;

} plnA_type;




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





pln_type *pln_alloc( int no );

void	pln_destroy( pln_type *pl );

pln_type *	pln_copy( pln_type *spl );


int		pln_set( pln_type *pl, vec2f_type *ctr, ln_type *l );

void	pln_set_length( pln_type *pl );


void	pln_inverse( pln_type *pl );
int		plnA_length_filter( plnA_type *apl, float minLen );


int		pln_create( pln_type *pl, pt2dA_type *apt );

void	pln_trim( pln_type *pl, int direct, float gt );

pln_type *	pln_copy_sub( pln_type *spl, float gt0, float gt1 );

void	pln_appendL( pln_type *pl, vec2f_type *ctr, ln_type *link );

void	pln_append( pln_type *pl, pln_type *pl1 );

void	pln_end_point( pln_type *pl, vec2d *p );

void	pln_gt2p( pln_type *pl, float gt, vec2f_type *p );

void	pln_tanget( pln_type *pl, float gt, vec2f_type *v );


pln_type *	pln_from_point( vec2d p[], int no, int Fclose );

pln_type *	pln_from_circle( vec2f_type *ctr, float R, float dr );

pln_type *	pln_from_sub_circle( vec2f_type *ctr0, float R, vec2f_type *p0, vec2f_type *p1, float dr );

float	plnA_group_length( plnA_type *aP, int group );

void	plnA_destroy_group( plnA_type *apl, int group );



pln_type *	pln_from_lnA( ln_type al[], int nAl );

void	pln_scale( pln_type *pl, float scale );

void	plnA_clear( plnA_type *apl );

void	plnA_destroy_pl( plnA_type *apl, int i0 );






int	pln_distance( pln_type *pl, vec2f_type *p, dPln_type *d );

void	pln_box( pln_type *pl, box2f_type *box );


plnA_type *		plnA_alloc( int n );

void	plnA_destroy( plnA_type *apl );

void	plnA_destroy_header( plnA_type *apl );


int		plnA_decrease( plnA_type *apl );

int		plnA_select( plnA_type *apl, vec2f_type *v, float dis );

int		plnA_selectI( plnA_type *apl, vec2f_type *v, int ignore, float mDis, dPln_type *dp );


float	plnA_length( plnA_type *apl );

int		plnA_distance( plnA_type *apl, vec2f_type *p, float D, pln_type **spl, dPln_type *sd );

void	plnA_add( plnA_type *apl, pln_type *pl );


void	plnA_translate( plnA_type *aP, float x, float y );

void	plnA_scale( plnA_type *aP, float scale );


void	pln_sample( pln_type *pl, float t0, float r, int n, int direct, pt2dA_type *apt );

void	pln_sampleN( pln_type *pl, float D, float r, pt2dA_type *apt );


pln_type *	pln_split( pln_type *spl, float gt, float dt );


void	pln_tangetS( pln_type *pl, float gt, vec2f_type *v );

void	pln_pv( pln_type *pl, float gt, float dt, int nDt, vec2f_type *pd, vec2f_type *vd, float *d );

void	pln_pv_d( pln_type *pl, float gt0, float gt1, float dt, struct vl2f_type *vl );

int		pln_pv_p( pln_type *pl, vec2f_type *p, float dt, int nDt, struct vl2f_type *vl );



void	pln_lt2( pln_type *pl, lt2_type *lt );


void	plnA_lt2( plnA_type *apl, lt2_type *lt );



void	plnA_iLine_set( plnA_type *apl, int iLine );

int		plnA_iLine_get( plnA_type *apl );


	// PlnWrite.c
void	plnA_dump( plnA_type *apl, char *prefix, int index, char *suffix );

void	pln_dump( pln_type *pl, char *prefix, int index, char *suffix );

int		plnA_write( plnA_type *apl, char *file );

int		plnA_read( char *file, plnA_type **apl );

int		pln_write_to_file( pln_type *pl, char *file );







// PlnParallel.c
int	pln_parallel2(  pln_type *bpl, pln_type *pl, int i, float aT, float T0, float T1, pPln_type ap[] );

int	pln_parallel( pln_type *bpl, pln_type *pl, int i, float gt, float aT, float T0, float T1, pPln_type *pp );

int	pln_parallel_distance(  pln_type *bpl, pln_type *pl, float dt, float aT, float D0, float D1, float *u );

float	pln_straightline_measure( pln_type *pl, float gt, float dt, int n );



void	pPln_reorder( pPln_type a[], int n );

void	pPln_inverse( pPln_type *p );


	// PlnAParallelDistance.cpp
float	plnA_parallel_distance(  plnA_type *apl, float r0, float r1 );


plnA_type *
plnA_get_group( plnA_type *apl, int group, int Fdetouch, plnA_type *capl );

plnA_type *plnA_copy_group( plnA_type *apl, int group, plnA_type *capl );

plnA_type *	plnA_copy_step( plnA_type *apl, int step, plnA_type *capl );

void	plnAG_set_step( plnA_type *apl, int group, int step );

plnA_type *	plnA_move_group( plnA_type *apl, int group, plnA_type *gapl );

void	plnA_append( plnA_type *tapl, plnA_type *apl );


void	plnA_set_groupId( plnA_type *apl, int group );

void	pln_close( pln_type *pl, float T );


	// PlnFit.cpp
int	pln_fit( pln_type *pl, float gt, float wt, pln_type *bpl, lt2_type *lt );

int	pln_fit_step( pln_type *pl, float gt, float wt, pln_type *bpl, lt2_type *lt );

	// PlnGroup.c
void plnA_group_parallel( plnA_type *apl, float dt, float aT, float D0, float D1 );

int	plnA_group_max( plnA_type *apl, int *iG, int *no );

pt2dGroupA_type *	pt2dGroupA_set_pln( pt2dGroupA_type *ag, plnA_type *apl );



#ifdef _DUMP
#define PLNA_DUMP( apl, name, index, ext )  plnA_dump( apl, name, index, ext )
#define PLN_DUMP( pl, name, index, ext )  pln_dump( pl, name, index, ext )

#else
#define PLNA_DUMP( apl, name, index, ext )
#define PLN_DUMP( pl, name, index, ext )

#endif



#ifdef __cplusplus
}
#endif

#endif