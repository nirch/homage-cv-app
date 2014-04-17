/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/******************
 ***  PtLine.h  ***
 ******************/
#ifndef _CR_PT_LINE_
#define _CR_PT_LINE_


#ifdef __cplusplus
extern "C" {
#endif





typedef struct crPln_type {
	int	iPl;
	pln_type	*pl;


	float	u;
	float	du;
	dPln_type	d[2];

	int		type;
	int		end;

} crPln_type;




int	crPln_create( pln_type *bpl, pln_type *pl, int iPl, float r1, float r2, crPln_type *cr );

int	crPln_createN( pln_type *bpl, pln_type *pl, int iPl, float r0, float r1, crPln_type cr[] );


void	crPln_inverse( crPln_type *c, int fPl );


int	crPln_duplicate( plnA_type *apl, int i0, float r, crPln_type acr[] );


	// get order side
int	crLineA_oreder_side( crPln_type acr[], int nAcr, int side, crPln_type *ad[], int *nD );

int	crPlnA_union( crPln_type acr[], int *nAcr );



#ifdef __cplusplus
}
#endif


#endif