
/*************************
 ***  PlnUnion.h  ***
 *************************/


#ifndef _PLN_UNION_H_
#define _PLN_UNION_H_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnLibD.lib" )
#else
#pragma comment( lib, "PlnLib.lib" )
#endif



#include "Uln/PlnType.h"

typedef	struct plnUnionPrm_type {

	int		on;

	float dt;

} plnUnionPrm_type;




pln_type *	plnC_union( pln_type *bpl, pln_type *pl );


	// PlnUnionPrm.cpp
plnUnionPrm_type *	plnUnionPrm_alloc();

void	plnUnionPrm_set( plnUnionPrm_type *prm, float dT, float angle, float radius );

void	plnUnionPrm_aux( plnUnionPrm_type *prm );

int		plnUnionPrm_read(  plnUnionPrm_type *prm, char *file );

int		plnUnionPrm_read(  plnUnionPrm_type *prm, struct xmlTag_type *pTag );

#endif