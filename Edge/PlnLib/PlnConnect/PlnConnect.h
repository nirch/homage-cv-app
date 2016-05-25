
/**********************
 ***  PlnConnect.h  ***
 *********************/


#ifndef _PLN_CONNECT_H_
#define _PLN_CONNECT_H_


#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "PlnLibD.lib" )
#else
#pragma comment( lib, "PlnLib.lib" )
#endif



#include "Uln/PlnType.h"

typedef	struct plnConnectPrm_type {

	int		on;

	float dt;



	float	angle;	

	float	r;
	float	r0;

	float gapC;

	// Aux
	float	aT;

} plnConnectPrm_type;


typedef	struct dPlnConnect_type {

	int	bi;

	vl2f_type bvl;

	int	bdirect;



	int	i;

	int direct;

	vl2f_type	vl;

	float	d;

} dPlnConnect_type;



int	pln_connect( plnA_type *apl, plnConnectPrm_type *prm );

int	pln_connect( pln_type *bpl, plnConnectPrm_type *prm, plnA_type *apl );

int	pln_connect_connect_1( pln_type *bpl, int direct, plnConnectPrm_type *prm, plnA_type *apl );


int	pln_connect_find( pln_type *bpl, int direct, plnConnectPrm_type *prm, plnA_type *apl, dPlnConnect_type *dc  );

int	pln_connect_find( pln_type *bpl, vl2f_type *bvl, plnConnectPrm_type *prm, plnA_type *apl, dPlnConnect_type *dc );



		
	// PlnConnect.cpp
plnConnectPrm_type *	plnConnectPrm_alloc();

void	plnConnectPrm_set( plnConnectPrm_type *prm, float dT, float angle, float radius );

void	plnConnectPrm_aux( plnConnectPrm_type *prm );

int		plnConnectPrm_read(  plnConnectPrm_type *prm, char *file );

int		plnConnectPrm_read(  plnConnectPrm_type *prm, struct xmlTag_type *pTag );

#endif