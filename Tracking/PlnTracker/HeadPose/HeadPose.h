
#ifndef _HEAD_POSE_H_
#define _HEAD_POSE_H_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnTrackerD.lib" )
#else
#pragma comment( lib, "PlnTracker.lib" )
#endif


#include "Uvec/Vec2d.h"
#include "Ubox/Box2d.h"

struct pln_type;



typedef struct headPose_type {
	int iFrame;

	int	state;

	vec2f_type	p;

	float angle;

	float scale;

	float qulity;

	int		smooth;

	int		group;

} headPose_type;



typedef struct headPoseA_type {
	int NA;
	int	nA;


	headPose_type	**a;

	// Aux
	float	scale;
	float qulity;
} headPoseA_type;


typedef struct headPoseB_type {
	int NA;
	int	nA;

	headPoseA_type	**a;

} headPoseB_type;


headPose_type *headPose_alloc();

void	headPose_destroy( headPose_type *pose );

headPose_type *	headPose_copy( headPose_type *pose, headPose_type *f );



headPoseA_type *headPoseA_alloc( int no );

headPoseA_type *	headPoseA_realloc( headPoseA_type *ah, int no );

void headPoseA_destroy( headPoseA_type *ah, int fData = 1 );




headPoseB_type *	headPoseB_alloc( int no );

headPoseB_type *	headPoseB_realloc( headPoseB_type *ah, int no );

void				headPoseB_destroy( headPoseB_type *af, int fData );

void				headPoseB_decrease( headPoseB_type *bh );


void				headPoseB_add( headPoseB_type *af, headPoseA_type *ah, int i0 = -1 );


headPoseB_type *	headPoseB_create(  headPoseA_type *ah );
headPoseB_type *	headPoseB_createG(  headPoseA_type *ah );



void	headPose_set( headPose_type *hp, int iFrame, int state, float x, float y, float scale, float angle );

void	headPoseA_add( headPoseA_type *ah, headPose_type *h, int i0 );



void	headPoseA_clear( headPoseA_type *af );

void	headPoseA_clear( headPoseA_type *af, int i0, int i1 );

int		headPoseA_clear( headPoseA_type *af, int iFrame );

void	headPoseA_aux( headPoseA_type *ah );


void	headPoseA_icycle( headPoseA_type *af );

headPose_type *	headPoseA_get( headPoseA_type *af, int iFrame );

void	headPoseA_set( headPoseA_type *af, headPose_type *pose, int iFrame, int fCopy );

void	headPoseA_append( headPoseA_type *ah, headPoseA_type *sah, int fCopy );

void	headPoseA_set_iFrame( headPoseA_type *ah, int iFrame );

void	headPoseA_set_group( headPoseA_type *ah, int group );

void	headPoseA_set_state( headPoseA_type *ah, int state );

headPoseA_type *	headPoseA_merge( headPoseA_type *a[], int nA );


int	headPoseA_write( headPoseA_type *af, char *file );
void	headPose_write( headPose_type *pose, FILE *fp );

int	headPoseA_read( headPoseA_type **ah, char *file );
int	headPoseA_readG( headPoseA_type **ah, char *file );

int		headPose_read( headPose_type *pose, int version, FILE *fp );

headPose_type *	headPoseA_getG( headPoseA_type *ah, int iGroup, int iFrame );


headPoseA_type *	headPoseA_copy( headPoseA_type *af );
headPoseA_type *	headPoseA_copy_group( headPoseA_type *ah, int group, int fCopy );
headPoseA_type *	headPoseA_copy_groupG( headPoseA_type *ah, int group, int fCopy );

void	headPoseA_fill( headPoseA_type *af );

void	headPoseA_box( headPoseA_type *af, box2f_type *b );

int		headPoseA_decrease( headPoseA_type *ah );


int	headPoseA_insert( headPoseA_type *bah, headPoseA_type *ah );

int	headPoseA_insert( headPoseA_type *ah, headPose_type *h );

int	headPoseA_frame_index( headPoseA_type *ah, int i0, int iFrame );


struct plnA_type *	headPoseA_to_plnA( headPoseA_type *ah, pln_type *bpl );

struct plnF_type *	headPoseA_to_plnF(  headPoseA_type *ah, pln_type *bpl );


pln_type *	headPose_pl( headPose_type *h, pln_type *bpl );


float	headPoseA_qulity( headPoseA_type *ah );

int		headPoseA_ellipse( headPoseA_type *ah, struct ellipse_type *e );



void	headPoseA_dump( headPoseA_type *ah, char *prefix, int index, char *suffix );

void	headPoseA_dump_pl( headPoseA_type *ah, pln_type *bpl, char *prefix, int index, char *suffix );

void	headPoseA_dump_plF( headPoseA_type *ah, pln_type *bpl, char *prefix, int index, char *suffix );

void	headPoseA_dump( headPoseA_type *ag[], int nG, pln_type *hpl, char *prefix, int index, char *suffix );


void	headPoseB_dump( headPoseB_type *bh, pln_type *hpl, char *prefix, int index, char *suffix );



#ifdef _DUMP
#define HEADPOSEA_DUMP( ah, name, index, ext )  headPoseA_dump( ah , name, index, ext )
#define HEADPOSEA_DUMP_PL( ah, bpl, name, index, ext )  headPoseA_dump_pl( ah, bpl , name, index, ext )
#define HEADPOSEA_DUMP_PLF( ah, bpl, name, index, ext )  headPoseA_dump_plF( ah, bpl , name, index, ext )
#define HEADPOSEB_DUMP( ah, bpl, name, index, ext )  headPoseB_dump( ah, bpl, name, index, ext )
#else
#define HEADPOSEA_DUMP( ah, name, index, ext ) {};
#define HEADPOSEA_DUMP_PL( ah, bpl, name, index, ext ) {};
#define HEADPOSEA_DUMP_PLF( ah, bpl, name, index, ext ) {};
#define HEADPOSEB_DUMP( ah, bpl, name, index, ext ) {};
#endif



#endif


