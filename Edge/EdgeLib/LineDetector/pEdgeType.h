

#ifndef	_PEDGE_TYPE___
#define _PEDGE_TYPE___

#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"
#include "Uprm/prm.h"

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "EdgeLibD.lib" )
#else
#pragma comment( lib, "EdgeLib.lib" )
#endif


#include "Uln/PlnType.h"

#include "ImageType/ImageType.h"


typedef struct pEdgePrm_type {

	int	scale;

	int	d;

	float	gradT;


	float	lenMin;


	float	linking_A;


	float	straightLine;

	vec2f_type directV;
	float	   directAngle;

} pEdgePrm_type;




typedef	signed short		s_short;

typedef struct pEdge_type	{
	s_short	g;
	s_short	state;

	vec2d	p;

	vec2d	v;

	s_short	ng[2];

	int	nStatic;
} pEdge_type;


	// pEdgeDetector.cpp
image_type *pEdge_detector( image_type *sim, pEdgePrm_type *prm, image_type *im );


	// pEdgeLinking.cpp
void	pEdge_linking( image_type *rim );

int	pEdge_linking_get( image_type *rim, pEdge_type *dp00, pEdge_type *ad[] );


	// pEdgeLine.cpp
int		pEdge_line( image_type *dim, int d, pEdgePrm_type *prm, struct plEdgeA_type *aPl );

	// pEdgeStraightLine.cpp
int		pEdge_straight_line( image_type *dim, int d, pEdgePrm_type *prm, plEdgeA_type *aPl );

int		pEdge_straight_line( image_type *rim, pEdge_type *rp0,  pEdge_type *rp1, pEdgePrm_type *prm, struct vl2f_type *vl );


// pEdgePrm.h
void	pEdgePrm_get_lprm( GP_prm ** prm, int *prm_no, void (**check) ());

void	pEdgePrm_update( pEdgePrm_type *prm );

void	pEdgePrm_set( pEdgePrm_type *prm );

pEdgePrm_type *	pEdgePrm_alloc();

void	pEdgePrm_destroy( pEdgePrm_type *rPrm );


int	pEdgePrm_read( pEdgePrm_type *prm, char *file );

int	pEdgePrm_read( struct xmlTag_type *pTag, pEdgePrm_type *prm );


int	pEdgePrm_write( pEdgePrm_type *prm, char *file );


int	pEdgePrm_write( pEdgePrm_type *prm, int align, FILE *fp );



	// pEdgeDump.cpp
void	pEdge_dump( image_type *dim, char *prefix, int index, char *suffix );
void	pEdge_dump_T( image_type *dim, float x, float y, char *prefix, int index, char *suffix );


void	pEdge_write_to_file( image_type *dim, char *file );


pt2dA_type *	pEdge_to_pt2dA( image_type *dim );



	// pEdgeStatic.cpp
image_type *	pEdge_static( image_type *sim, image_type *im );
image_type *	pEdge_staticN( image_type *sim, image_type *im );




#ifdef _DUMP
#define PEDGE_DUMP( im, name, index, ext )  pEdge_dump( im, name, index, ext )
#define PEDGE_DUMPT( im, x, y, name, index, ext )  pEdge_dump_T( im, x, y, name, index, ext )
#else
#define PEDGE_DUMP( im, name, index, ext )
#define PEDGE_DUMPT( im, x, y, name, index, ext )
#endif


#endif