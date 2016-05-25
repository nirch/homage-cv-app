
/**********************
 ***  PlnFeature.h  ***
 **********************/


#ifndef _pln_feature_H_
#define _pln_feature_H_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnLibD.lib" )
#else
#pragma comment( lib, "PlnLib.lib" )
#endif



#include "Uln/PlnType.h"

typedef	struct plnFeaturePrm_type {

	int		on;

	float	cornerDis;
	float	cornerLen;

	float	featureLen;


	float	angle;

	// Aux
	float	aT;

} plnFeaturePrm_type;



int	plnA_feature( plnA_type *apl, plnFeaturePrm_type *prm );




		
	// PlnConnect.cpp
plnFeaturePrm_type *	plnFeaturePrm_alloc();

void	plnFeaturePrm_set( plnFeaturePrm_type *prm, float dT, float len, float angle );


void	plnFeaturePrm_aux( plnFeaturePrm_type *prm );

int		plnFeaturePrm_read(  plnFeaturePrm_type *prm, char *file );

int		plnFeaturePrm_read(  plnFeaturePrm_type *prm, struct xmlTag_type *pTag );

#endif