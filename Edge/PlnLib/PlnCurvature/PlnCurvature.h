
/*************************
 ***  PlnCureveture.h  ***
 *************************/


#ifndef _pln_curvature_H_
#define _pln_curvature_H_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnLibD.lib" )
#else
#pragma comment( lib, "PlnLib.lib" )
#endif



#include "Uln/PlnType.h"

typedef	struct plnCurvaturePrm_type {

	int		on;

	float dt;



//	float	angle;	

	//float	r;
	//float	r0;

	//float gapC;

	float slT;

	float slT1;
	float slLen;

	// Aux
//	float	aT;

} plnCurvaturePrm_type;






int	pln_curvature( pln_type *pl, plnCurvaturePrm_type *prm, float *curvature );

int	pt2dA_curvature( pt2dA_type *apt, int cyclic, int N );


vl2fA_type *	plnA_straight( plnA_type *apl, plnCurvaturePrm_type *prm, int nT, vl2fA_type *avl );

int	plnA_straight( plnA_type *apl, int i0, int i1, plnCurvaturePrm_type *prm, int nT, vl2fA_type *avl );

int	pln_straight( pln_type *pl, plnCurvaturePrm_type *prm, int nT, vl2fA_type *avl );


		
	// PlnConnect.cpp
plnCurvaturePrm_type *	plnCurvaturePrm_alloc();

void	plnCurvaturePrm_set( plnCurvaturePrm_type *prm, float dT, float angle, float radius );

void	plnCurvaturePrm_aux( plnCurvaturePrm_type *prm );

int		plnCurvaturePrm_read(  plnCurvaturePrm_type *prm, char *file );

int		plnCurvaturePrm_read(  plnCurvaturePrm_type *prm, struct xmlTag_type *pTag );

#endif