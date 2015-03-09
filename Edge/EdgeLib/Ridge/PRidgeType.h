/***
 ***    author:		Yoram Elichai   yorame@gmail.com
 ***    Copyright (C) 2000 - All Rights Reserved
 ***/

/************************
 ***   pRidgeType.h   ***
 ************************/

#ifndef	_LINE_DETECT__
#define _LINE_DETECT__


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "EdgeLibD.lib" )
#else
#pragma comment( lib, "EdgeLib.lib" )
#endif



#include "Uvec/Vec2d.h"
#include "Uvec/Vec3d.h"
#include "Uprm/prm.h"

#include "ImageType/ImageType.h"

#include "pRidgePrm/pRidgePrm.h"



typedef struct pRidge_type	{
	short	state;

	float	f;
	float	lm;

	float d;

	vec2f_type	v;

	vec2f_type	p;

	short	ng[2];

} pRidge_type;







	// pRidgeDetector.cpp
image_type *	pRidge_detector( image_type *sim, image_type *mim, image_type *rmim[], int nRmim, pRidgeDetectPrm_type *prm, image_type *im );

int				pRidge_detector( image_type *sim, int i0, int j0, pRidge_type *rp, struct jet2App_type *ja, pRidgeDetectPrm_type *prm );

	// pRidgeLinking.cpp
void	pRidge_linking( image_type *rim );

void	pRidge_linking_union( image_type *rim );


pt2dA_type *	pt2dA_from_image_pRidge( image_type *dim );

void	image_pRidge_write( image_type *rim, char *file );


void	image_pRidge_dump( image_type *rim, char *prefix, int index, char *suffix );



	// pRidgeEdgeMask.cpp
void	pRidge_edge_mask( image_type *sim, image_type *rim );

image_type *	pRidge_weight( image_type *rim, image_type *wim );


	// pRidegMask.cpp
int	pRidge_mask( image_type *dim, int d,  image_type *mim );



	// pRidgeLine.c
//int	pRidge_line( image_type *dim, int d, struct plnA_type *aPl );
int	pRidge_line( image_type *dim, int d,  pRidgeLinePrm_type *prm, struct plRidgeA_type *ar );


int	pRidge_line_1( image_type *rim, pRidge_type *dp00, pRidge_type *ad[], int *nD );

int	pRidge_approximate_pl( pRidge_type *ad[200], int nD, struct pln_type *pl[] );


	// pRidgeLineN.c
int	pRidge_lineN( image_type *dim, int d, pRidgeLinePrm_type *prm, struct plRidgeA_type *aRidge );





#ifdef _DUMP
#define PRIDGE_DUMP( rim, prefix, index,suffix )     image_pRidge_dump( rim, prefix, index, suffix )
#else
#define PRIDGE_DUMP( rim, prefix, index,suffix ) 
#endif



#endif
