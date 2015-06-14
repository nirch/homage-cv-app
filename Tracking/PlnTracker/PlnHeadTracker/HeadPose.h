
#ifndef _HEAD_POSE_H_
#define _HEAD_POSE_H_


#include "Uvec/Vec2d.h"
#include "Ubox/Box2d.h"



typedef struct headPose_type {
	int iFrame;

	int	state;

	vec2f_type	p;

	float angle;

	float scale;

	float	qulity;

	int		smooth;

} headPose_type;



typedef struct headPoseA_type {
	int NA;
	int	nA;

	headPose_type	**a;

} headPoseA_type;


headPose_type *headPose_alloc();

void	headPose_destroy( headPose_type *pose );

headPose_type *	headPose_copy( headPose_type *pose, headPose_type *f );

headPoseA_type *headPoseA_alloc( int no );

headPoseA_type *	headPoseA_realloc( headPoseA_type *ah, int no );


void headPoseA_destroy( headPoseA_type *af );

void	headPose_set( headPose_type *hp, int iFrame, int state, float x, float y, float scale, float angle );


void	headPoseA_clear( headPoseA_type *af );

void	headPoseA_clear( headPoseA_type *af, int i0, int i1 );

int		headPoseA_clear( headPoseA_type *af, int iFrame );

void	headPoseA_icycle( headPoseA_type *af );

headPose_type *	headPoseA_get( headPoseA_type *af, int iFrame );

void	headPoseA_set( headPoseA_type *af, headPose_type *pose, int iFrame, int fCopy );

int	headPoseA_write( headPoseA_type *af, char *file );
void	headPose_write( headPose_type *pose, FILE *fp );

int	headPoseA_read( headPoseA_type **ah, char *file );

int		headPose_read( headPose_type *pose, FILE *fp );


headPoseA_type *	headPoseA_copy( headPoseA_type *af );

void	headPoseA_fill( headPoseA_type *af );

void	headPoseA_box( headPoseA_type *af, box2f_type *b );



#endif


