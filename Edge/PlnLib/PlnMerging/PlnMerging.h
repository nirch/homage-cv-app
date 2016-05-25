
/**********************
 ***  PlnMerging.h  ***
 *********************/


#ifndef _PLN_MARGING_H_
#define _PLN_MARGING_H_



#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnLibD.lib" )
#else
#pragma comment( lib, "PlnLib.lib" )
#endif


#include "Uln/PlnType.h"


#include "Uln/PlnType.h"

typedef	struct plnMergingPrm_type {

	int		on;

	float dU;
	float dT;


} plnMergingPrm_type;




plnA_type *	plnA_merging( plnA_type *apl, plnA_type *tapl, plnMergingPrm_type *prm ); //float dU );

int	plnA_merging( plnA_type *apl, pln_type *bpl, plnMergingPrm_type *prm );  //float dU );



	// PlnMergingScale.cpp
plnA_type *		plnA_merging_scale( plnA_type *apl, plnA_type *tapl, plnMergingPrm_type *prm );



// PlnMerging.cpp
plnMergingPrm_type *	plnMergingPrm_alloc();

void	plnMergingPrm_set( plnMergingPrm_type *prm, float dU, float dT );

void	plnMergingPrm_aux( plnMergingPrm_type *prm );

int		plnMergingPrm_read(  plnMergingPrm_type *prm, char *file );

int		plnMergingPrm_read(  plnMergingPrm_type *prm, struct xmlTag_type *pTag );


#endif